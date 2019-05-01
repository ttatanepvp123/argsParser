#ifndef PARSER_STRUCT_H
#define PARSER_STRUCT_H

#include <stdbool.h>

enum type_arg {
    PARSER_INTEGER = 0,
    PARSER_UNSIGNED_INTEGER,
    PARSER_STRING,
    PARSER_SET_TO_TRUE,
    PARSER_SET_TO_FALSE
};

struct option{
    int type;
    char *name;
    char *description;
    void **pointer;
    _Bool required;
};

struct alias{
    char *name;
    int id;
};


struct parser{
    struct option *options;
    int optionsNumber;
    struct alias *alias;
    int aliasNumber;
    char **examples;
    int examplesNumber;
    char *description;
};


#endif