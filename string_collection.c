#include "string_collection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4

typedef struct {
    Element base;
    char *value;
} StringElement;

typedef struct {
    Element base;
    char value;
    char string_value[2];
} CharElement;

static char *copy_string(const char *src) {
    if (src == NULL) {
        return NULL;
    }
    size_t len = strlen(src);
    char *copy = malloc((len + 1) * sizeof(char));
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, src);
    return copy;
}

Element *element_clone(const Element *element) {
    if (element == NULL || element->vtable == NULL || element->vtable->clone == NULL) {
        return NULL;
    }
    return element->vtable->clone(element);
}

void element_destroy(Element *element) {
    if (element == NULL || element->vtable == NULL || element->vtable->destroy == NULL) {
        return;
    }
    element->vtable->destroy(element);
}

void element_print(const Element *element) {
    if (element == NULL || element->vtable == NULL || element->vtable->print == NULL) {
        printf("<null>");
        return;
    }
    element->vtable->print(element);
}

const char *element_as_cstring(const Element *element) {
    if (element == NULL || element->vtable == NULL || element->vtable->as_cstring == NULL) {
        return NULL;
    }
    return element->vtable->as_cstring(element);
}

static Element *string_element_clone(const Element *element) {
    const StringElement *string_element = (const StringElement *)element;
    return sc_create_string_element(string_element->value);
}

static void string_element_destroy(Element *element) {
    StringElement *string_element = (StringElement *)element;
    free(string_element->value);
    free(string_element);
}

static void string_element_print(const Element *element) {
    const StringElement *string_element = (const StringElement *)element;
    printf("\"%s\"", string_element->value);
}

static const char *string_element_as_cstring(const Element *element) {
    const StringElement *string_element = (const StringElement *)element;
    return string_element->value;
}

static const ElementVTable STRING_ELEMENT_VTABLE = {
    string_element_clone,
    string_element_destroy,
    string_element_print,
    string_element_as_cstring
};

Element *sc_create_string_element(const char *value) {
    if (value == NULL) {
        return NULL;
    }
    StringElement *element = malloc(sizeof(StringElement));
    if (element == NULL) {
        return NULL;
    }
    element->value = copy_string(value);
    if (element->value == NULL) {
        free(element);
        return NULL;
    }
    element->base.vtable = &STRING_ELEMENT_VTABLE;
    return (Element *)element;
}

static Element *char_element_clone(const Element *element) {
    const CharElement *char_element = (const CharElement *)element;
    return sc_create_char_element(char_element->value);
}

static void char_element_destroy(Element *element) {
    free(element);
}

static void char_element_print(const Element *element) {
    const CharElement *char_element = (const CharElement *)element;
    printf("'%c'", char_element->value);
}

static const char *char_element_as_cstring(const Element *element) {
    const CharElement *char_element = (const CharElement *)element;
    return char_element->string_value;
}

static const ElementVTable CHAR_ELEMENT_VTABLE = {
    char_element_clone,
    char_element_destroy,
    char_element_print,
    char_element_as_cstring
};

Element *sc_create_char_element(char value) {
    CharElement *element = malloc(sizeof(CharElement));
    if (element == NULL) {
        return NULL;
    }
    element->base.vtable = &CHAR_ELEMENT_VTABLE;
    element->value = value;
    element->string_value[0] = value;
    element->string_value[1] = '\0';
    return (Element *)element;
}

static SCStatus sc_reserve(StringCollection *collection, size_t new_capacity) {
    if (collection == NULL) {
        return SC_ERROR_NULL_POINTER;
    }
    if (new_capacity <= collection->capacity) {
        return SC_OK;
    }
    Element **new_data = realloc(collection->data, new_capacity * sizeof(Element *));
    if (new_data == NULL) {
        return SC_ERROR_MEMORY;
    }
    collection->data = new_data;
    collection->capacity = new_capacity;
    return SC_OK;
}

StringCollection *sc_create(void) {
    StringCollection *collection = malloc(sizeof(StringCollection));
    if (collection == NULL) {
        return NULL;
    }
    collection->data = calloc(INITIAL_CAPACITY, sizeof(Element *));
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
        element_destroy(collection->data[i]);
    }
    free(collection->data);
    free(collection);
}

SCStatus sc_push(StringCollection *collection, Element *element) {
    if (collection == NULL || element == NULL) {
        return SC_ERROR_NULL_POINTER;
    }
    if (collection->size == collection->capacity) {
        SCStatus status = sc_reserve(collection, collection->capacity * 2);
        if (status != SC_OK) {
            return status;
        }
    }
    collection->data[collection->size] = element;
    collection->size++;
    return SC_OK;
}

SCStatus sc_get(const StringCollection *collection, size_t index, const Element **out) {
    if (collection == NULL || out == NULL) {
        return SC_ERROR_NULL_POINTER;
    }
    if (index >= collection->size) {
        return SC_ERROR_INDEX;
    }
    *out = collection->data[index];
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
        Element *copy = element_clone(collection->data[index]);
        if (copy == NULL) {
            sc_destroy(result);
            return NULL;
        }
        SCStatus status = sc_push(result, copy);
        if (status != SC_OK) {
            element_destroy(copy);
            sc_destroy(result);
            return NULL;
        }
    }
    return result;
}

void sc_print(const StringCollection *collection) {
    if (collection == NULL) {
        printf("Collection is NULL\n");
        return;
    }
    printf("[ ");
    for (size_t i = 0; i < collection->size; i++) {
        element_print(collection->data[i]);
        if (i + 1 < collection->size) {
            printf(", ");
        }
    }
    printf(" ]\n");
}
