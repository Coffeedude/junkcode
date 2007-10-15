/*******************************************************************
 * Test DNS SRV lookups
 * copyright Gerald Carter <jerry@samba.org>  2006
 * 
 * For some bizarre reason the ns_initparse(), et. al. routines
 * are not available in the shared version of libresolv.so.
 * 
 * To compile, run
 *    gcc -o dnstest -static query-srv.c -lresolv
 * 
 *******************************************************************/

/* standard system headers */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* resolver headers */

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include <netdb.h>


int main ( int argc, char *argv[] )
{
	unsigned char buffer[NS_PACKETSZ];
	int resp_len;
	ns_msg h;
	ns_rr rr;
	int rrnum;

	if ( argc != 2 ) {
		fprintf( stderr, "Usage: %s <hostname>\n", argv[0]);
		exit( 1 );
	}

	/* send the request */

	if ( (resp_len = res_query(argv[1], ns_c_in, ns_t_srv, buffer, sizeof(buffer))) < 0 ) {
		fprintf(stderr, "Query for %s failed!\n", argv[1]);
		exit( 1 );
	}

	/* now do the parsing */

	if ( ns_initparse( buffer, resp_len, &h ) ) {
		fprintf( stderr, "Failed to parse response buffer!\n");
		exit( 2 );
	}

	printf("%d records returned in the answer section.\n", ns_msg_count(h, ns_s_an));

	for ( rrnum=0; rrnum < ns_msg_count(h, ns_s_an); rrnum++ ) {
		if ( ns_parserr( &h, ns_s_an, rrnum, &rr ) ) {
			fprintf( stderr, "ns_parserr: %s\n", strerror(errno) );
			exit (2);
		}

		if ( ns_rr_type(rr) == ns_t_srv ) {
			char name[1024];
			int ret;
			struct in_addr ip;
			const unsigned char *p;

			ret = dn_expand( ns_msg_base(h), ns_msg_end(h), ns_rr_rdata(rr)+6, name, sizeof(name));

			if ( ret < 0 ) {
				fprintf( stderr, "Failed to uncompress name (%d)!\n", ret);
				exit(2);
			}
			printf( "%s\n", name );
		}
	}

	for ( rrnum=0; rrnum < ns_msg_count(h, ns_s_ar); rrnum++ ) {
		if ( ns_parserr( &h, ns_s_ar, rrnum, &rr ) ) {
			fprintf( stderr, "ns_parserr: %s\n", strerror(errno) );
			continue;
		}

		if ( ns_rr_type(rr) == ns_t_a ) {
			char name[1024];
			int ret;
			struct in_addr ip;
			const unsigned char *p;

                        p = ns_rr_rdata(rr);
                        ip.s_addr = (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];

                        printf( "%s has address %s\n", ns_rr_name(rr), inet_ntoa(ip));
		}
	}
	
	return 0;
}
