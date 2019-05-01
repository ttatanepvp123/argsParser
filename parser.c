#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "parser_struct.h"
#include "defines.h"

#define COUNT_OF(ptr) (sizeof(ptr)/sizeof(ptr[0]))

struct parser *parser_init(){
    struct parser *parser = NULL;
    parser = calloc(sizeof(struct parser), 1);
    assert(parser != NULL);
    return parser;
}

void parser_display_help(struct parser *parser,char *argv[]){
    printf("%s [OPTION]...\n\n", argv[0]);
    for (size_t i = 0; i < parser->optionsNumber; i++){
        printf("\t%s", parser->options[i].name);
        for (size_t j = 0; j < parser->aliasNumber; j++){
            if (parser->alias[j].id == i){
                printf(", %s", parser->alias[j].name);
            }
        }
        if (parser->options[i].description != NULL){
            printf("\t%s ", parser->options[i].description);
        }

        if (parser->options[i].required){
            printf("( required )");
        }

        printf("\n");
    }

    if (parser->examplesNumber){
        printf("\nExamples :\n\n");
        for (size_t i = 0; i < parser->examplesNumber; i++){
            printf("\t%s\n", parser->examples[i]);
        }
    }

    if (parser->description != NULL)
        printf("\nDescription :\n\n%s\n", parser->description);
    
}

int parser_add_option(struct parser *parser,const char *name, int type, void *dest, _Bool required, const char *description){
    assert(parser != NULL);
    assert(name != NULL);
    assert(dest != NULL);
    const char *reserved[] = {
        "-h",
        "--help"
    };

    for (size_t i = 0; i < COUNT_OF(reserved); i++){
        if (!strcmp(reserved[i], name)){
            return FLAG_RESERVED;
        }
    }

    if (!parser->optionsNumber){
        parser->options = calloc(sizeof(struct option), 1);
        assert(parser->options != NULL);
        parser->options[0].pointer = dest;
        parser->options[0].type = type;
        parser->options[0].required = required;
        parser->options[0].name = calloc(sizeof(char), strlen(name)+1);
        assert(parser->options[0].name != NULL);
        strcpy(parser->options[0].name, name);
        if (description == NULL){
            parser->options[0].description = NULL;
        } else {
            parser->options[0].description = calloc(sizeof(char), strlen(description)+1);
            assert(parser->options[0].description != NULL);
            strcpy(parser->options[0].description, description);
        }
        parser->optionsNumber++;
    } else {
        parser->optionsNumber++;
        parser->options = realloc(parser->options,sizeof(struct option)*parser->optionsNumber);
        assert(parser->options != NULL);
        parser->options[parser->optionsNumber-1].pointer = dest;
        parser->options[parser->optionsNumber-1].type = type;
        parser->options[parser->optionsNumber-1].required = required;
        parser->options[parser->optionsNumber-1].name = calloc(sizeof(char), strlen(name)+1);
        assert(parser->options[parser->optionsNumber-1].name != NULL);
        strcpy(parser->options[parser->optionsNumber-1].name, name);
        if (description == NULL){
            parser->options[parser->optionsNumber-1].description = NULL;
        } else {
            parser->options[parser->optionsNumber-1].description = calloc(sizeof(char), strlen(description)+1);
            assert(parser->options[parser->optionsNumber-1].description != NULL);
            strcpy(parser->options[parser->optionsNumber-1].description, description);
        }
    }
    return parser->optionsNumber-1;
}

int parser_add_alias(struct parser *parser,const char *name, int id){
    assert(parser != NULL);
    assert(name != NULL);

    if (id < 0 || id > parser->optionsNumber){
        return INVALID_ID;
    }

    const char *reserved[] = {
        "-h",
        "--help"
    };

    for (size_t i = 0; i < COUNT_OF(reserved); i++){
        if (!strcmp(reserved[i], name)){
            return FLAG_RESERVED;
        }
    }

    if (parser->aliasNumber == 0){
        parser->alias = calloc(sizeof(struct alias), 1);
        assert(parser->alias != NULL);
        parser->alias[0].id = id;
        parser->alias[0].name = calloc(sizeof(char), strlen(name)+1);
        assert(parser->alias[0].name != NULL);
        strcpy(parser->alias[0].name, name);
        parser->aliasNumber++;
    } else {
        parser->aliasNumber++;
        parser->alias = realloc(parser->alias,sizeof(struct alias) * parser->aliasNumber);
        assert(parser->alias != NULL);
        parser->alias[parser->aliasNumber-1].id = id;
        parser->alias[parser->aliasNumber-1].name = calloc(sizeof(char), strlen(name)+1);
        assert(parser->alias[parser->aliasNumber-1].name != NULL);
        strcpy(parser->alias[parser->aliasNumber-1].name, name);
    }
    return parser->aliasNumber-1;
}

void parser_add_example(struct parser *parser, char *example){
    assert(parser != NULL);
    assert(example != NULL);
    if (!parser->examplesNumber){
        parser->examples = calloc(sizeof(char *), 1);
        assert(parser->examples != NULL);
        parser->examples[0] = calloc(sizeof(char), strlen(example)+1);
        assert(parser->examples[0] != NULL);
        strcpy(parser->examples[0], example);
        parser->examplesNumber++;
    } else {
        parser->examplesNumber++;
        parser->examples = realloc(parser->examples, sizeof(char *)*parser->examplesNumber);
        assert(parser->examples != NULL);
        parser->examples[parser->examplesNumber-1] = calloc(sizeof(char), strlen(example)+1);
        assert(parser->examples[parser->examplesNumber-1] != NULL);
        strcpy(parser->examples[parser->examplesNumber-1], example);
    }
}

void parser_set_description(struct parser *parser, char *description){
    assert(parser != NULL);
    assert(description != NULL);
    if (parser->description == NULL){
        parser->description = calloc(sizeof(char), strlen(description)+1);
        assert(parser->description != NULL);
        strcpy(parser->description, description);
    } else {
        free(parser->description);
        parser->description = calloc(sizeof(char), strlen(description)+1);
        assert(parser->description != NULL);
        strcpy(parser->description, description);
    }
}

void parser_free(struct parser *parser){
    assert(parser != NULL);
    for (size_t i = 0; i < parser->aliasNumber; i++){
        free(parser->alias[i].name);
    }
    if (parser->alias != NULL)
        free(parser->alias);
    for (size_t i = 0; i < parser->optionsNumber; i++){
        free(parser->options[i].name);
        if (parser->options[i].description != NULL)
            free(parser->options[i].description);
    }
    if (parser->options != NULL)
        free(parser->options);
    if (parser->description != NULL)
        free(parser->description);
    free(parser);
    
}

_Bool parser_parse(struct parser *parser, int argc, char *argv[]){
    int tmp = 0;
    for (size_t i = 0; i < argc; i++){
        if (!strcmp("-h", argv[i]) | !strcmp("--help", argv[i])){
            parser_display_help(parser, argv);
            exit(EXIT_SUCCESS);
        }
    }
    
    for (size_t i = 0; i < parser->optionsNumber; i++){
        tmp = 0;
        for (size_t j = 0; j < argc; j++){
            if (!strcmp(argv[j], parser->options[i].name)){
                if (parser->options[i].type == PARSER_INTEGER && i+1 < argc){
                    j++;
                    if (sscanf(argv[j],"%d", (int *)parser->options[i].pointer) == 1){
                        tmp = 1;
                    } else {
                        printf("[ ERROR ] : '%s' not parsed !", argv[j]);
                    }
                } else if (parser->options[i].type == PARSER_UNSIGNED_INTEGER && i+1 < argc){
                    j++;
                    if (sscanf(argv[j],"%u", (int *)parser->options[i].pointer) == 1){
                        tmp = 1;
                    } else {
                        printf("[ ERROR ] : '%s' not parsed !", argv[j]);
                    }
                } else if (parser->options[i].type == PARSER_STRING && i+1 < argc) {
                    j++;
                    tmp = 1;
                    *(parser->options[i].pointer) = argv[j];
                } else if (parser->options[i].type == PARSER_SET_TO_TRUE) {
                    *(parser->options[i].pointer) = true;
                } else if (parser->options[i].type == PARSER_SET_TO_FALSE) {
                    *(parser->options[i].pointer) = false;
                }
            }
        }
        if (!tmp && parser->options[i].required){
            for (size_t k = 0; k < parser->aliasNumber; k++){
                if (parser->alias[k].id == i){
                    for (size_t j = 0; j < argc; j++){
                        if (!strcmp(argv[j], parser->alias[k].name)){
                            if (parser->options[i].type == PARSER_INTEGER && i+1 < argc){
                                j++;
                                if (sscanf(argv[j],"%d", (int *)parser->options[i].pointer) == 1){
                                    tmp = 1;
                                } else {
                                    printf("[ ERROR ] : '%s' not parsed !", argv[j]);
                                }
                            } else if (parser->options[i].type == PARSER_UNSIGNED_INTEGER && i+1 < argc){
                                j++;
                                if (sscanf(argv[j],"%u", (int *)parser->options[i].pointer) == 1){
                                    tmp = 1;
                                } else {
                                    printf("[ ERROR ] : '%s' not parsed !", argv[j]);
                                }
                            } else if (parser->options[i].type == PARSER_STRING && i+1 < argc) {
                                j++;
                                tmp = 1;
                                *(parser->options[i].pointer) = argv[j];
                            } else if (parser->options[i].type == PARSER_SET_TO_TRUE) {
                                *(parser->options[i].pointer) = true;
                            } else if (parser->options[i].type == PARSER_SET_TO_FALSE) {
                                *(parser->options[i].pointer) = false;
                            }
                        }
                    }
                }
            }
            if (!tmp){
                printf("[ ERROR ] '%s' is required !\n", parser->options[i].name);
                return false;
            }
        }
    }
    return true;
}