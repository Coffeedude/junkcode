#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

char *GetComment(FILE *input)
{
	int alloc_size = 1024;
	int len = 0;	
	char *buffer = NULL;
	char *tmp= NULL;
	bool done = false;
	
	
	if (!input)
		return NULL;
	
	buffer = malloc(alloc_size);
	
	while (!feof(input) && !done) {
		/* See if we have space */
		if ((len+2) >= alloc_size) {
			alloc_size *= 2;			
			tmp = realloc(buffer, alloc_size);
			if (!tmp) {
				free(buffer);				
				return NULL;
			}
			
			buffer = tmp;
		}
		
		buffer[len] = fgetc(input);
		
		if ((len > 2) &&
		    (buffer[len-1] == '*') &&
		    (buffer[len] == '/'))
		{
			done = true;
		}
		
		len++;
	}

	/* Drop last two chatracters from comment */
	buffer[len-2] = '\0';

	return buffer;	
	
}

void PrintUsage(void)
{
	fprintf(stderr, "strip_comment <filename>\n");

	return;	
}



int main(int argc, char *argv[])
{
	FILE *input;
	char *comment;
	char scanChar;
	bool in_quote = false;	
	
	if (argc != 2) {
		PrintUsage();
		exit(1);		
	}

	input = fopen(argv[1], "r");
	if (!input) {
		fprintf(stderr, "Unable to open %s!\n",
			argv[1]);
		exit(2);
	}

	scanChar = '/';
	
	while (!feof(input)) {
		char c;

		c = fgetc(input);
		
		if (in_quote) {
			if (c == '"') {
				in_quote = false;
			}
			if (c != EOF)
				printf("%c", c);			
			continue;
		} else {
			if (c == '"')
				in_quote = true;
		}
		
		/* Find the beginning on the comment */

		if (c != '/') {
			if (c != EOF)
				printf("%c", c);			
			continue;
		}
		
		if ((c = fgetc(input)) != '*') {		       
			if (c != EOF)
				printf("/%c", c);			
			continue;
		}
		
		
		comment = GetComment(input);

		if (!comment) {
			fprintf(stderr, "Malformed C file.  Unable "
				"to find end of comment\n");
			exit(3);
		}

		if (!strcasestr(comment, "Editor Settings")
		    && !strcasestr(comment, "This program is free software"))
		{
			printf("/*%s*/", comment);
			continue;
		}
		
		free(comment);
		comment = NULL;		
		
	}

	return 0;	
}
