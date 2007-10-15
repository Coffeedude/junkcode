#include <stdio.h>
#include <pwd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <passwd|group> <name>\n",
			argv[0]);
		return 1;		
	}
	
	if (strcmp(argv[1], "passwd") == 0) {
		struct passwd *pw = getpwnam(argv[2]);
		
		if (!pw) {
			fprintf(stderr, "Unable to find %s\n", argv[2]);
			return 2;
		}
		
		printf("%s:%s:%d:%d:%s:%s:%s\n",
		       pw->pw_name, pw->pw_passwd, pw->pw_uid,
		       pw->pw_gid, pw->pw_gecos, pw->pw_dir,
		       pw->pw_shell);

		return 0;
	} 
		
	if (strcmp(argv[1], "group") == 0) {
		printf("Not implemented\n");
		return 2;		
	} 

	fprintf(stderr, "Usage: %s <passwd|group> <name>\n",
		argv[0]);
	
	return 1;
}
