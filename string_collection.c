#include "string_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 4

static char *sc_strdup(const char *src) {
    if (src == NULL) {
        return NULL;
    }

    size_t len = strlen(src);
    char *copy = (char *)malloc((len + 1) * sizeof(char));

    if (copy == NULL) {
        return NULL;
    }

    strcpy(copy, src);
    return copy;
}

static SCStatus sc_reserve(StringCollection *collection, size_t new_capacity) {
    if (collection == NULL) {
        return SC_ERROR_NULL_POINTER;
    }

    if (new_capacity <= collection->capacity) {
        return SC_OK;
    }

    SCElement *new_data = (SCElement *)realloc(
        collection->data,
        new_capacity * sizeof(SCElement)
    );

    if (new_data == NULL) {
        return SC_ERROR_MEMORY;
    }

    collection->data = new_data;
    collection->capacity = new_capacity;

    return SC_OK;
}

StringCollection *sc_create(void) {
    StringCollection *collection = (StringCollection *)malloc(sizeof(StringCollection));

    if (collection == NULL) {
        return NULL;
    }

    collection->data = (SCElement *)calloc(INITIAL_CAPACITY, sizeof(SCElement));

    if (collection->data == NULL) {
        free(collection);
        return NULL;
    }

    collection->size = 0;
    collection->capacity = INITIAL_CAPACITY;

    return collection;
}

void sc_destroy(StringCollection *collection) {
    if (collection == NULL) {
        return;
    }

    for (size_t i = 0; i < collection->size; i++) {
        free(collection->data[i].value);
    }

    free(collection->data);
    free(collection);
}

SCStatus sc_push_string(StringCollection *collection, const char *str) {
    if (collection == NULL || str == NULL) {
        return SC_ERROR_NULL_POINTER;
    }

    if (collection->size == collection->capacity) {
        SCStatus status = sc_reserve(collection, collection->capacity * 2);

        if (status != SC_OK) {
            return status;
        }
    }

    char *copy = sc_strdup(str);

    if (copy == NULL) {
        return SC_ERROR_MEMORY;
    }

    collection->data[collection->size].type = SC_ELEMENT_STRING;
    collection->data[collection->size].value = copy;
    collection->size++;

    return SC_OK;
}

SCStatus sc_push_char(StringCollection *collection, char ch) {
    if (collection == NULL) {
        return SC_ERROR_NULL_POINTER;
    }

    char buffer[2];
    buffer[0] = ch;
    buffer[1] = '\0';

    if (collection->size == collection->capacity) {
        SCStatus status = sc_reserve(collection, collection->capacity * 2);

        if (status != SC_OK) {
            return status;
        }
    }

    char *copy = sc_strdup(buffer);

    if (copy == NULL) {
        return SC_ERROR_MEMORY;
    }

    collection->data[collection->size].type = SC_ELEMENT_CHAR;
    collection->data[collection->size].value = copy;
    collection->size++;

    return SC_OK;
}

SCStatus sc_get(const StringCollection *collection, size_t index, SCElement *out) {
    if (collection == NULL || out == NULL) {
        return SC_ERROR_NULL_POINTER;
    }

    if (index >= collection->size) {
        return SC_ERROR_INDEX;
    }

    out->type = collection->data[index].type;
    out->value = collection->data[index].value;

    return SC_OK;
}

size_t sc_size(const StringCollection *collection) {
    if (collection == NULL) {
        return 0;
    }

    return collection->size;
}

int sc_is_empty(const StringCollection *collection) {
    return collection == NULL || collection->size == 0;
}

void sc_print(const StringCollection *collection) {
    if (collection == NULL) {
        printf("Collection is NULL\n");
        return;
    }

    printf("[ ");

    for (size_t i = 0; i < collection->size; i++) {
        if (collection->data[i].type == SC_ELEMENT_CHAR) {
            printf("'%s'", collection->data[i].value);
        } else {
            printf("\"%s\"", collection->data[i].value);
        }

        if (i + 1 < collection->size) {
            printf(", ");
        }
    }

    printf(" ]\n");
}

char *sc_concat_strings(const char *left, const char *right) {
    if (left == NULL || right == NULL) {
        return NULL;
    }

    size_t left_len = strlen(left);
    size_t right_len = strlen(right);

    char *result = (char *)malloc((left_len + right_len + 1) * sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    strcpy(result, left);
    strcat(result, right);

    return result;
}

/*
    Подстрока с i по j включительно.
    Индексация с 0.

    Например:
    str = "abcdef", i = 1, j = 3
    результат = "bcd"
*/
char *sc_substring(const char *str, size_t i, size_t j) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);

    if (i >= len || j >= len || i > j) {
        return NULL;
    }

    size_t result_len = j - i + 1;

    char *result = (char *)malloc((result_len + 1) * sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    strncpy(result, str + i, result_len);
    result[result_len] = '\0';

    return result;
}

/*
    Получение элементов коллекции с i по j включительно.
*/
StringCollection *sc_slice(const StringCollection *collection, size_t i, size_t j) {
    if (collection == NULL) {
        return NULL;
    }

    if (collection->size == 0 || i >= collection->size || j >= collection->size || i > j) {
        return NULL;
    }

    StringCollection *result = sc_create();

    if (result == NULL) {
        return NULL;
    }

    for (size_t index = i; index <= j; index++) {
        SCStatus status;

        if (collection->data[index].type == SC_ELEMENT_CHAR) {
            status = sc_push_char(result, collection->data[index].value[0]);
        } else {
            status = sc_push_string(result, collection->data[index].value);
        }

        if (status != SC_OK) {
            sc_destroy(result);
            return NULL;
        }
    }

    return result;
}

/*
    Разбиение строки на слова.

    Разделителями считаются любые пробельные символы:
    пробел, табуляция, перевод строки и т.д.
*/
StringCollection *sc_split_words(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    StringCollection *result = sc_create();

    if (result == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    size_t i = 0;

    while (i < len) {
        while (i < len && isspace((unsigned char)str[i])) {
            i++;
        }

        if (i >= len) {
            break;
        }

        size_t start = i;

        while (i < len && !isspace((unsigned char)str[i])) {
            i++;
        }

        size_t end = i - 1;
        char *word = sc_substring(str, start, end);

        if (word == NULL) {
            sc_destroy(result);
            return NULL;
        }

        SCStatus status = sc_push_string(result, word);
        free(word);

        if (status != SC_OK) {
            sc_destroy(result);
            return NULL;
        }
    }

    return result;
}