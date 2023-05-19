#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "shell.h"

extern void print_prompt1(void);
char *read_cmd(void)
{
	char *ptr = NULL;
	size_t bufsize = 0;
	
	while (1)
	{
		ssize_t buflen = getline(&ptr, &bufsize, stdin);

		if (buflen == -1)
		{
			const char *errmsg = "error: failed to read input: ";
			write(STDERR_FILENO, errmsg, strlen(errmsg));
			write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
			write(STDERR_FILENO, "\n", 1);
			free(ptr);
			return (NULL);
		}
		if (ptr[buflen - 1] == '\n')
		{
			if (buflen == 1 || ptr[buflen - 2] != '\\')
			{
				ptr[buflen - 1] = '\0';
				return (ptr);
			}
			ptr[buflen - 2] = '\0';
			buflen -= 2;
			print_prompt1();
		}
		
	}
}
void print_prompt1(void)
{
	fprintf(stderr, "$ ");
}
