#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "node.h"
#include "parser.h"
#include "symtab.h"
#include <unistd.h>
#include <errno.h>
struct symtab_stack_s symtab_stack;
int    symtab_level;

void init_symtab(void)
{
	symtab_stack.symtab_count = 1;
	symtab_level = 0;
	struct symtab_s *global_symtab = malloc(sizeof(struct symtab_s));
	if(!global_symtab)
	{
		const char *errmsg = "fatal error: no memory for global symbol table\n";
		write(STDERR_FILENO, errmsg, strlen(errmsg));
		exit(EXIT_FAILURE);
	}
	 memset(global_symtab, 0, sizeof(struct symtab_s));
	 symtab_stack.global_symtab  = global_symtab;
	 symtab_stack.local_symtab   = global_symtab;
	 symtab_stack.symtab_list[0] = global_symtab;
	 global_symtab->level        = 0;
}
struct symtab_s *new_symtab(int level)
{
	struct symtab_s *symtab = malloc(sizeof(struct symtab_s));
	if(!symtab)
	{
		const char *errmsg = "fatal error: no memory for new symbol table\n";
		write(STDERR_FILENO, errmsg, strlen(errmsg));
		exit(EXIT_FAILURE);
	}
	memset(symtab, 0, sizeof(struct symtab_s));
	symtab->level = level;
	return (symtab);
}
void free_symtab(struct symtab_s *symtab)
{
	struct symtab_entry_s *entry = symtab->first;
	if(symtab == NULL)
	{
		return;
	}
	while(entry)
	{
		if(entry->name)
		{
			free(entry->name);
		}
		if(entry->val)
		{
			free(entry->val);
		}
		if(entry->func_body)
		{
			free_node_tree(entry->func_body);
		}
		struct symtab_entry_s *next = entry->next;
		free(entry);
		entry = next;
	}
	free(symtab);
}
struct symtab_entry_s *add_to_symtab(char *symbol)
{
	struct symtab_s *st = symtab_stack.local_symtab;
	struct symtab_entry_s *entry = NULL;
	entry = malloc(sizeof(struct symtab_entry_s));

	if (!symbol || symbol[0] == '\0')
	{
		return (NULL);
	}
	if ((entry = do_lookup(symbol, st)))
	{
		return (entry);
	}
	if (!entry)
	{
		const char *errmsg = "fatal error: no memory for new symbol table entry\n";
		write(STDERR_FILENO, errmsg, strlen(errmsg));
		exit(EXIT_FAILURE);
	}
	memset(entry, 0, sizeof(struct symtab_entry_s));
	entry->name = malloc(strlen(symbol) + 1);
	if (!entry->name)
	{
		const char *errmsg = "fatal error: no memory for new symbol table entry\n";
		write(STDERR_FILENO, errmsg, strlen(errmsg));
		exit(EXIT_FAILURE);
	}
	strcpy(entry->name, symbol);
	if (!st->first)
	{
		st->first = entry;
		st->last = entry;
	}
	else
	{
		st->last->next = entry;
		st->last = entry;
	}
	return (entry);
}
struct symtab_entry_s *do_lookup(char *str, struct symtab_s *symtable)
{
	struct symtab_entry_s *entry = symtable->first;
	if (!str || !symtable)
	{
		return (NULL);
	}
	while(entry)
	{
		if(strcmp(entry->name, str) == 0)
		{
			return entry;
		}
		entry = entry->next;
	}
	return (NULL);
}
struct symtab_entry_s *get_symtab_entry(char *str)
{
        int i = symtab_stack.symtab_count-1;

        do
        {
                struct symtab_s *symtab = symtab_stack.symtab_list[i];
                struct symtab_entry_s *entry = do_lookup(str, symtab);

                if(entry)
                {
                        return (entry);
                }
        }
        while(--i >= 0);

        return (NULL);
}
void symtab_entry_setval(struct symtab_entry_s *entry, char *val)
{
        if(entry->val)
        {
                free(entry->val);
        }
        if(!val)
        {
                entry->val = NULL;
        }
        else
        {
                char *val2 = malloc(strlen(val)+1);

                if(val2)
                {
                        strcpy(val2, val);
                }
                else
                {
                        const char *errmsg = "error: no memory for symbol table entry's value\n";
                        write(STDERR_FILENO, errmsg, strlen(errmsg));
                }
                entry->val = val2;
        }
}
void symtab_stack_add(struct symtab_s *symtab)
{
        symtab_stack.symtab_list[symtab_stack.symtab_count++] = symtab;
        symtab_stack.local_symtab = symtab;
}
struct symtab_s *symtab_stack_push(void)
{
        struct symtab_s *st = new_symtab(++symtab_level);
        symtab_stack_add(st);
        return (st);
}
struct symtab_s *symtab_stack_pop(void)
{
        if(symtab_stack.symtab_count == 0)
        {
                return NULL;
        }
        struct symtab_s *st = symtab_stack.symtab_list[symtab_stack.symtab_count-1];
        symtab_stack.symtab_list[--symtab_stack.symtab_count] = NULL;
        symtab_level--;
        if(symtab_stack.symtab_count == 0)
        {
                symtab_stack.local_symtab  = NULL;
                symtab_stack.global_symtab = NULL;
        }
        else
        {
                symtab_stack.local_symtab = symtab_stack.symtab_list[symtab_stack.symtab_count-1];
        }
        return (st);
}
struct symtab_s *get_local_symtab(void)
{
        return (symtab_stack.local_symtab);
}
struct symtab_s *get_global_symtab(void)
{
        return (symtab_stack.global_symtab);
}
struct symtab_stack_s *get_symtab_stack(void)
{
        return (&symtab_stack);
}
