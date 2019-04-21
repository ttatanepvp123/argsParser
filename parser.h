#ifndef PARSER_H
#define PARSER_H

#include "parser_struct.h"
#include "defines.h"

struct parser *parser_init();
int parser_add_option(struct parser *parser,const char *name, int type, void *dest, _Bool required, const char *description);
int parser_add_alias(struct parser *parser,const char *name, int id);
void parser_add_example(struct parser *parser, char *example);
void parser_set_description(struct parser *parser, char *description);
void parser_display_help(struct parser *parser,char *argv[]);
_Bool parser_parse(struct parser *parser, int argc, char const *argv[]);
void parser_free(struct parser *parser);

#endif