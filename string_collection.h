#ifndef STRING_COLLECTION_H
#define STRING_COLLECTION_H

#include <stddef.h>

typedef enum {
    SC_OK = 0,
    SC_ERROR_NULL_POINTER,
    SC_ERROR_MEMORY,
    SC_ERROR_INDEX,
    SC_ERROR_EMPTY
} SCStatus;

typedef enum {
    SC_ELEMENT_CHAR,
    SC_ELEMENT_STRING
} SCElementType;

typedef struct {
    SCElementType type;
    char *value;
} SCElement;

typedef struct {
    SCElement *data;
    size_t size;
    size_t capacity;
} StringCollection;

/* Базовые функции коллекции */
StringCollection *sc_create(void);
void sc_destroy(StringCollection *collection);
SCStatus sc_push_string(StringCollection *collection, const char *str);
SCStatus sc_push_char(StringCollection *collection, char ch);
SCStatus sc_get(const StringCollection *collection, size_t index, SCElement *out);
size_t sc_size(const StringCollection *collection);
int sc_is_empty(const StringCollection *collection);
void sc_print(const StringCollection *collection);

/* Операции по варианту */
char *sc_concat_strings(const char *left, const char *right);
char *sc_substring(const char *str, size_t i, size_t j);
StringCollection *sc_slice(const StringCollection *collection, size_t i, size_t j);
StringCollection *sc_split_words(const char *str);

/* Тесты */
void run_all_tests(void);

#endif