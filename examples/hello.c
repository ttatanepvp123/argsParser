#include <stdio.h>
#include <stdbool.h>
#include "../parser.h"

int main(int argc, char const *argv[]){
    struct parser *parser = NULL;
    char *first = NULL;
    char *last = NULL;
    int tmp = 0;
    parser = parser_init();

    tmp = parser_add_option(parser, "--first", PARSER_STRING, &first, true, "your first name");
    parser_add_alias(parser, "-f", tmp);
    tmp = parser_add_option(parser, "--last", PARSER_STRING, &last, true, "your last name");
    parser_add_alias(parser, "-l", tmp);

    parser_add_example(parser, "--first lucas --last films");
    parser_add_example(parser, "-f lucas -l films");

    parser_set_description(parser,"program that says hello\ncopyright 2019-2030 x)");

    if(parser_parse(parser, argc, argv)){
        printf("Hello %s %s\n", first, last);
    } else {
        printf("hello\n");
    }

    parser_free(parser);

    return 0;
}