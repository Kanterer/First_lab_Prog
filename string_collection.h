#ifndef STRING_COLLECTION_H
#define STRING_COLLECTION_H
#include <stddef.h>
typedef int SCStatus;
#define SC_OK 0
#define SC_ERROR_NULL_POINTER 1
#define SC_ERROR_MEMORY 2
#define SC_ERROR_INDEX 3
typedef struct Element Element;

typedef struct {
    Element *(*clone)(const Element *element);
    void (*destroy)(Element *element);
    void (*print)(const Element *element);
    const char *(*as_cstring)(const Element *element);
} ElementVTable;

struct Element {
    const ElementVTable *vtable;
};

typedef struct {
    Element **data;
    size_t size;
    size_t capacity;
} StringCollection;

Element *sc_create_string_element(const char *value);
Element *sc_create_char_element(char value);
Element *element_clone(const Element *element);
void element_destroy(Element *element);
void element_print(const Element *element);
const char *element_as_cstring(const Element *element);
StringCollection *sc_create(void);
void sc_destroy(StringCollection *collection);
SCStatus sc_push(StringCollection *collection, Element *element);
SCStatus sc_get(const StringCollection *collection, size_t index, const Element **out);
size_t sc_size(const StringCollection *collection);
int sc_is_empty(const StringCollection *collection);
StringCollection *sc_slice(const StringCollection *collection, size_t i, size_t j);
void sc_print(const StringCollection *collection);
#endif
