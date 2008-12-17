#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc , char *argv[])
{
	struct addrinfo hints;
	struct addrinfo *ailist = NULL;
	struct addrinfo *res = NULL;
	int ret = -1;
	int i = 0;

	memset(&hints, 0x0, sizeof(hints));

	hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
	hints.ai_flags = AI_ADDRCONFIG;
#else
	hints.ai_flags = 0;
#endif
	hints.ai_family = AF_INET;
	

	ret = getaddrinfo(argv[1],
			NULL,
			&hints,
			&ailist);
	if (ret) {
		printf("getaddrinfo failed for name %s [%s]\n",
		       argv[1], 
		       gai_strerror(ret));
	}

	for (res = ailist; res; res = res->ai_next) {
		struct sockaddr_storage ss;

		if (!res->ai_addr || res->ai_addrlen == 0) {
			continue;
		}

		memset(&ss, '\0', sizeof(ss));
		memcpy(&ss, res->ai_addr, res->ai_addrlen);
		printf("foo\n");		

	}
	

	return 0;	
}

#if 0
static NTSTATUS resolve_hosts(const char *name, int name_type,
			      struct ip_service **return_iplist,
			      int *return_count)
{
	struct addrinfo hints;
	struct addrinfo *ailist = NULL;
	struct addrinfo *res = NULL;
	int ret = -1;
	int i = 0;

	if ( name_type != 0x20 && name_type != 0x0) {
		DEBUG(5, ("resolve_hosts: not appropriate "
			"for name type <0x%x>\n",
			name_type));
		return NT_STATUS_INVALID_PARAMETER;
	}

	*return_iplist = NULL;
	*return_count = 0;

	DEBUG(3,("resolve_hosts: Attempting host lookup for name %s<0x%x>\n",
				name, name_type));

	ZERO_STRUCT(hints);
	/* By default make sure it supports TCP. */
	hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
	hints.ai_flags = AI_ADDRCONFIG;
#else
	hints.ai_flags = 0;
#endif

#if !defined(HAVE_IPV6)
	/* Unless we have IPv6, we really only want IPv4 addresses back. */
	hints.ai_family = AF_INET;
#endif

	ret = getaddrinfo(name,
			NULL,
			&hints,
			&ailist);
	if (ret) {
		DEBUG(3,("resolve_hosts: getaddrinfo failed for name %s [%s]\n",
			name,
			gai_strerror(ret) ));
	}

	for (res = ailist; res; res = res->ai_next) {
		struct sockaddr_storage ss;

		if (!res->ai_addr || res->ai_addrlen == 0) {
			continue;
		}

		memset(&ss, '\0', sizeof(ss));
		memcpy(&ss, res->ai_addr, res->ai_addrlen);

		*return_count += 1;

		*return_iplist = SMB_REALLOC_ARRAY(*return_iplist,
						struct ip_service,
						*return_count);
		if (!*return_iplist) {
			DEBUG(3,("resolve_hosts: malloc fail !\n"));
			freeaddrinfo(ailist);
			return NT_STATUS_NO_MEMORY;
		}
		(*return_iplist)[i].ss = ss;
		(*return_iplist)[i].port = PORT_NONE;

		i++;
	}
	if (ailist) {
		freeaddrinfo(ailist);
	}
	if (*return_count) {
		return NT_STATUS_OK;
	}
	return NT_STATUS_UNSUCCESSFUL;
}

#endif
