#include <unistd.h>
#include <string.h>
#include "symtab.h"

struct symtab_stack_s symtab_stack;
int    symtab_level;

void dump_local_symtab(void)
{
	struct symtab_s *symtab = symtab_stack.local_symtab;
	int i = 0;
	int indent = symtab->level * 4;
	const char *header1 = "%*sSymbol table [Level %d]:\r\n";
	const char *header2 = "%*s===========================\r\n";
	const char *header3 = "%*s  No               Symbol                    Val\r\n";
	const char *header4 = "%*s------ -------------------------------- ------------\r\n";
	const char *entry_format = "%*s[%04d] %-32s '%s'\r\n";
	const char *footer = "%*s------ -------------------------------- ------------\r\n";
	int count;

	count = write(STDERR_FILENO, header1, strlen(header1));
	count += write(STDERR_FILENO, " ", indent);
	count += write(STDERR_FILENO, " ", symtab->level);
	count += write(STDERR_FILENO, header2, strlen(header2));
	count += write(STDERR_FILENO, " ", indent);
	count += write(STDERR_FILENO, header3, strlen(header3));
	count += write(STDERR_FILENO, " ", indent);
	count += write(STDERR_FILENO, header4, strlen(header4));
	count += write(STDERR_FILENO, " ", indent);

	struct symtab_entry_s *entry = symtab->first;

	while (entry)
	{
		count += write(STDERR_FILENO, entry_format, strlen(entry_format));
		count += write(STDERR_FILENO, " ", indent);
		count += write(STDERR_FILENO, " ", i++);
		count += write(STDERR_FILENO, " ", strlen(entry->name));
		count += write(STDERR_FILENO, " ", strlen(entry->val));
		entry = entry->next;
	}
	count += write(STDERR_FILENO, footer, strlen(footer));
	count += write(STDERR_FILENO, " ", indent);
}
