#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "source.h"
#include "parser.h"
#include "executor.h"

int main(int argc, char **argv)
{
	char *cmd;

	do
	{
		print_prompt1();

		cmd = read_cmd();

		if(!cmd)
		{
			exit(EXIT_SUCCESS);
		}

		if(cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
		{
			free(cmd);
			continue;
		}

		if(strcmp(cmd, "exit\n") == 0)
		{
			free(cmd);
			break;
		}

		struct source_s src;
		src.buffer   = cmd;
		src.bufsize  = strlen(cmd);
		src.curpos   = INIT_SRC_POS;
		parse_and_execute(&src);

		free(cmd);

		} while(1);

	exit(EXIT_SUCCESS);
}


int parse_and_execute(struct source_s *src)
{
	skip_white_spaces(src);

	struct token_s *tok = tokenize(src);

	if(tok == &eof_token)
	{
		return 0;
	}

	while(tok && tok != &eof_token)
	{
		struct node_s *cmd = parse_simple_command(tok);

        	if(!cmd)
		{
			break;
		}

		do_simple_command(cmd);
		free_node_tree(cmd);
		tok = tokenize(src);
	}

	return 1;
}
