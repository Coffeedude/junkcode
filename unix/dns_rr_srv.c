/* 
   Unix SMB/CIFS implementation.
   DNS utility library
   Copyright (C) Gerald (Jerry) Carter           2006.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* to compile: gcc -o dnstest dns_rr_srv.c -lresolv */

/* #include "includes.h" */

/* standard system headers */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>


/* resolver headers */

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <netdb.h>

#define CVAL(buf,pos) ((unsigned)(((const unsigned char *)(buf))[pos]))
#define PVAL(buf,pos) (CVAL(buf,pos))


#define SVAL(buf,pos) (PVAL(buf,pos)|PVAL(buf,(pos)+1)<<8)
#define IVAL(buf,pos) (SVAL(buf,pos)|SVAL(buf,(pos)+2)<<16)

#define SREV(x) ((((x)&0xFF)<<8) | (((x)>>8)&0xFF))
#define IREV(x) ((SREV(x)<<16) | (SREV((x)>>16)))
#define RSVAL(buf,pos) SREV(SVAL(buf,pos))
#define RIVAL(buf,pos) IREV(IVAL(buf,pos))

#define QSORT_CAST (int (*)(const void *, const void *))


typedef int NTSTATUS;
typedef int TALLOC_CTX;
typedef unsigned char uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

#define PSTRING_LEN 1024
#define FSTRING_LEN 256

typedef char pstring[PSTRING_LEN];
typedef char fstring[FSTRING_LEN];


struct dns_rr_srv {
        const char *hostname;
        uint16 priority;
        uint16 weight;
        uint16 port;
	struct in_addr ip;
};

struct dns_query {
	const char *hostname;
	uint16 type;
	uint16 in_class;
};

struct dns_rr {
	const char *hostname;
	uint16 type;
	uint16 in_class;
	uint32 ttl;
	uint16 rdatalen;
	uint8 *rdata;	
};


#define NT_STATUS_OK			0
#define NT_STATUS_UNSUCCESSFUL 		1
#define NT_STATUS_INVALID_PARAMETER	2
#define NT_STATUS_NO_MEMORY		3

#define TALLOC_ARRAY(ctx, type, count) (type*)malloc( count * sizeof(type))
#define talloc_strdup(x,y) strdup(y)
#define DEBUG( x, y ) printf y ;
#define SAFE_FREE( x ) free( x );

#define PTR_DIFF(p1,p2) ((ptrdiff_t)(((const char *)(p1)) - (const char *)(p2)))
#define ZERO_STRUCT(x) memset((char *)&(x), 0, sizeof(x))
#define ZERO_STRUCTP(x) do { if ((x) != NULL) memset((char *)(x), 0, sizeof(*(x))); } while(0)

#define MAX_DNS_PACKET_SIZE 0xffff

typedef int BOOL;
#define False (0)
#define True (1)

/*********************************************************************
*********************************************************************/

static BOOL ads_dns_parse_query( TALLOC_CTX *ctx, uint8 *start, uint8 *end,
                          uint8 **ptr, struct dns_query *q )
{
	uint8 *p = *ptr;
	pstring hostname;
	int namelen;

	ZERO_STRUCTP( q );
	
	if ( !start || !end || !q || !*ptr)
		return False;

	/* See RFC 1035 for details. If this fails, then return. */

	namelen = dn_expand( start, end, p, hostname, sizeof(hostname) );
	if ( namelen < 0 ) {
		return False;
	}
	p += namelen;
	q->hostname = talloc_strdup( ctx, hostname );

	/* check that we have space remaining */

	if ( PTR_DIFF(p+4, end) > 0 )
		return False;

	q->type     = RSVAL( p, 0 );
	q->in_class = RSVAL( p, 2 );
	p += 4;

	*ptr = p;

	return True;
}

/*********************************************************************
*********************************************************************/

static BOOL ads_dns_parse_rr( TALLOC_CTX *ctx, uint8 *start, uint8 *end,
                       uint8 **ptr, struct dns_rr *rr )
{
	pstring answer_name, dcname;
	uint8 *p = *ptr;
	pstring hostname;
	int namelen;

	if ( !start || !end || !rr || !*ptr)
		return -1;

	ZERO_STRUCTP( rr );
	/* pull the name from the answer */

	namelen = dn_expand( start, end, p, hostname, sizeof(hostname) );
	if ( namelen < 0 ) {
		return -1;
	}
	p += namelen;
	rr->hostname = talloc_strdup( ctx, hostname );

	/* check that we have space remaining */

	if ( PTR_DIFF(p+10, end) > 0 )
		return False;

	/* pull some values and then skip onto the string */

	rr->type     = RSVAL(p, 0);
	rr->in_class = RSVAL(p, 2);
	rr->ttl      = RIVAL(p, 4);
	rr->rdatalen = RSVAL(p, 8);
	
	p += 10;

	/* sanity check the available space */

	if ( PTR_DIFF(p+rr->rdatalen, end ) > 0 ) {
		return False;

	}

	/* save a point to the rdata for this section */

	rr->rdata = p;
	p += rr->rdatalen;

	*ptr = p;

	return True;
}

/*********************************************************************
*********************************************************************/

static BOOL ads_dns_parse_rr_srv( TALLOC_CTX *ctx, uint8 *start, uint8 *end,
                       uint8 **ptr, struct dns_rr_srv *srv )
{
	struct dns_rr rr;
	uint8 *p;
	pstring dcname;
	int namelen;

	if ( !start || !end || !srv || !*ptr)
		return -1;

	/* Parse the RR entry.  Coming out of the this, ptr is at the beginning 
	   of the next record */

	if ( !ads_dns_parse_rr( ctx, start, end, ptr, &rr ) ) {
		DEBUG(1,("ads_dns_parse_rr_srv: Failed to parse RR record\n"));
		return False;
	}

	if ( rr.type != ns_t_srv ) {
		DEBUG(1,("ads_dns_parse_rr_srv: Bad answer type (%d)\n", rr.type));
		return False;
	}

	p = rr.rdata;

	srv->priority = RSVAL(p, 0);
	srv->weight   = RSVAL(p, 2);
	srv->port     = RSVAL(p, 4);

	p += 6;

	namelen = dn_expand( start, end, p, dcname, sizeof(dcname) );
	if ( namelen < 0 ) {
		DEBUG(1,("ads_dns_query_dcs: Failed to uncompress name!\n"));
		return NT_STATUS_UNSUCCESSFUL;
	}
	srv->hostname = talloc_strdup( ctx, dcname );

	return True;
}


/*********************************************************************
 Sort SRV record list based on weight and priority.  See RFC 2782.
*********************************************************************/

static int dnssrvcmp( struct dns_rr_srv *a, struct dns_rr_srv *b )
{
	/* higher weights should be sorted lower for more of 
	   a chance of use */

	if ( a->priority == b->priority ) {
		if ( a->weight > b->weight )
			return -1;
		
		return 1;
	}
		
	if ( a->priority < b->priority )
		return -1;

	return 1;
}

/*********************************************************************
 Simple wrapper for a DNS SRV query
*********************************************************************/

NTSTATUS ads_dns_lookup_srv( TALLOC_CTX *ctx, const char *name, struct dns_rr_srv **dclist, int *numdcs )
{
	uint8 *buffer = NULL;
	size_t buf_len;
	int resp_len = NS_PACKETSZ;
	struct dns_rr_srv *dcs = NULL;
	int query_count, answer_count, auth_count, additional_count;
	uint8 *p = buffer;
	pstring hostname;
	int namelen;
	int rrnum;
	int idx = 0;

	if ( !ctx || !name || !dclist ) {
		return NT_STATUS_INVALID_PARAMETER;
	}
	
	/* Send the request.  May have to loop several times in case 
	   of large replies */

	do {
		if ( buffer )
			SAFE_FREE( buffer );
		
		buf_len = resp_len * sizeof(uint8);

		if ( (buffer = (uint8 *) malloc(buf_len)) == NULL ) {
			DEBUG(0,("ads_dns_query_dcs: malloc failed!\n"));
			return NT_STATUS_NO_MEMORY;
		}

		if ( (resp_len = res_query(name, ns_c_in, ns_t_srv, buffer, buf_len)) < 0 ) {
			DEBUG(1,("ads_dns_query_dcs: Failed to resolve %s (%s)\n", name, strerror(errno)));
			SAFE_FREE( buffer );
			return NT_STATUS_UNSUCCESSFUL;
		}
	} while ( buf_len < resp_len && resp_len < MAX_DNS_PACKET_SIZE );

	p = buffer;

	/* For some insane reason, the ns_initparse() et. al. routines are only
	   available in libresolv.a, and not the shared lib.  Who knows why....
	   So we have to parse the DNS reply ourselves */

	/* Pull the answer RR's count from the header.  Use the NMB ordering macros */

	query_count      = RSVAL( p, 4 );
	answer_count     = RSVAL( p, 6 );
	auth_count       = RSVAL( p, 8 );
	additional_count = RSVAL( p, 10 );

	DEBUG(4,("ads_dns_query_dcs: %d records returned in the answer section.\n", 
		answer_count));
		
	if ( (dcs = TALLOC_ARRAY(ctx, struct dns_rr_srv, answer_count)) == NULL ) {
		DEBUG(0,("ads_dns_query_dcs: talloc() failure for %d char*'s\n", 
			answer_count));
		return NT_STATUS_NO_MEMORY;
	}

	/* now skip the header */

	p += NS_HFIXEDSZ;

	/* parse the query section */

	for ( rrnum=0; rrnum<query_count; rrnum++ ) {
		struct dns_query q;

		if ( !ads_dns_parse_query( ctx, buffer, buffer+resp_len, &p, &q ) ) {
			DEBUG(1,("ads_dns_query_dcs: Failed to parse query record!\n"));
			return NT_STATUS_UNSUCCESSFUL;
		}
	}

	/* now we are at the answer section */

	for ( rrnum=0; rrnum<answer_count; rrnum++ ) {
		if ( !ads_dns_parse_rr_srv( ctx, buffer, buffer+resp_len, &p, &dcs[rrnum] ) ) {
			DEBUG(1,("ads_dns_query_dcs: Failed to parse answer record!\n"));
			return NT_STATUS_UNSUCCESSFUL;
		}		
	}
	idx = rrnum;

	/* Parse the authority section */
	/* just skip these for now */

	for ( rrnum=0; rrnum<auth_count; rrnum++ ) {
		struct dns_rr rr;

		if ( !ads_dns_parse_rr( ctx, buffer, buffer+resp_len, &p, &rr ) ) {
			DEBUG(1,("ads_dns_query_dcs: Failed to parse authority record!\n"));
			return NT_STATUS_UNSUCCESSFUL;
		}
	}

	/* Parse the additional records section */

	for ( rrnum=0; rrnum<additional_count; rrnum++ ) {
		struct dns_rr rr;
		int i;

		if ( !ads_dns_parse_rr( ctx, buffer, buffer+resp_len, &p, &rr ) ) {
			DEBUG(1,("ads_dns_query_dcs: Failed to parse additional records section!\n"));
			return NT_STATUS_UNSUCCESSFUL;
		}

		/* only interested in A records as a shortcut for having to come 
		   back later and lookup the name */

		if ( (rr.type != ns_t_a) || (rr.rdatalen != 4) ) 
			continue;

		for ( i=0; i<idx; i++ ) {
			if ( strcmp( rr.hostname, dcs[i].hostname ) == 0 ) {
				uint8 *buf = (uint8*)&dcs[i].ip.s_addr;
				memcpy( buf, rr.rdata, 4 );
			}
		}
	}

	qsort( dcs, idx, sizeof(struct dns_rr_srv), QSORT_CAST dnssrvcmp );
	
	*dclist = dcs;
	*numdcs = idx;
	
	return NT_STATUS_OK;
}

/********************************************************************
********************************************************************/

NTSTATUS ads_dns_query_dcs( TALLOC_CTX *ctx, const char *domain, struct dns_rr_srv **dclist, int *numdcs )
{
	pstring name;

	snprintf( name, sizeof(name), "_ldap._tcp.dc._msdcs.%s", domain );

	return ads_dns_lookup_srv( ctx, name, dclist, numdcs );
}

/********************************************************************
********************************************************************/

int main( int argc, char *argv[] )
{
	int ctx; 
	struct dns_rr_srv *dclist = NULL;
	int numdcs = 0;
	int i;

	if ( argc != 2 ) {
		fprintf( stderr, "Usage: %s <domain>\n", argv[0]);
		exit( 1 );
	}

	if ( ads_dns_query_dcs( &ctx, argv[1], &dclist, &numdcs ) != 0 ) {
		fprintf( stderr, "Failed to locate any domain controllers for %s\n", 
			argv[1] );
		exit( 2 );
	}

	for ( i=0; i<numdcs; i++ ) {
		printf( "%s (%s)\n", dclist[i].hostname,
			dclist[i].ip.s_addr ? inet_ntoa(dclist[i].ip) : "UNKNOWN" );
		free( dclist[i].hostname );
	}
	free( dclist );
	
	return 0;
}

