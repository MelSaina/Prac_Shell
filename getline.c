#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void print_prompt1(void);
char *read_cmd(void)
{
	char buf[1024];
	char *ptr = NULL;
	char ptrlen = 0;
	
	while (fgets(buf, 1024, stdin))
	{
		int buflen = strlen(buf);

		if (!ptr)
		{
			ptr = malloc(buflen + 1);
		}
		else
		{
			char *ptr2 = realloc(ptr, ptrlen + buflen + 1);
			if (ptr2)
			{
				ptr = ptr2;
			}
			else
			{
				free(ptr);
				ptr = NULL;
			}
		}
		if (!ptr)
		{
			char errmsg[1024];
			int len = snprintf(errmsg, sizeof(errmsg), "error: failed to alloc buffer: %s\n", strerror(errno));
			write(STDERR_FILENO, errmsg, len);
			return NULL;
		}
		strcpy(ptr + ptrlen, buf);
		if (buf[buflen - 1] == '\n')
		{
			if (buflen == 1 || buf[buflen - 2] != '\\')
			{
				return ptr;
			}
			ptr[ptrlen + buflen - 2] = '\0';
			buflen -= 2;
			print_prompt1();
		}
		ptrlen += buflen;
	}
	return ptr;
}
void print_prompt1(void)
{
	fprintf(stderr, "$ ");
}
