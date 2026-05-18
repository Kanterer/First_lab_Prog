#include "string_collection.h"
#include "string_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_string_concat(void) {
    char *result = string_concat("Hello, ", "world!");
    assert(result != NULL);
    assert(strcmp(result, "Hello, world!") == 0);
    free(result);
    printf("test_string_concat: OK\n");
}

static void test_string_substring(void) {
    char *result = string_substring("abcdef", 1, 3);
    assert(result != NULL);
    assert(strcmp(result, "bcd") == 0);
    free(result);
    assert(string_substring("abcdef", 4, 2) == NULL);
    assert(string_substring("abcdef", 10, 12) == NULL);
    printf("test_string_substring: OK\n");
}

static void test_collection_push_and_get(void) {
    StringCollection *collection = sc_create();
    assert(collection != NULL);
    assert(sc_size(collection) == 0);
    assert(sc_push(collection, sc_create_char_element('A')) == SC_OK);
    assert(sc_push(collection, sc_create_string_element("Hello")) == SC_OK);
    assert(sc_size(collection) == 2);
    const Element *element = NULL;
    assert(sc_get(collection, 0, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "A") == 0);
    assert(sc_get(collection, 1, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "Hello") == 0);
    sc_destroy(collection);
    printf("test_collection_push_and_get: OK\n");
}

static void test_collection_slice(void) {
    StringCollection *collection = sc_create();
    assert(collection != NULL);
    assert(sc_push(collection, sc_create_string_element("one")) == SC_OK);
    assert(sc_push(collection, sc_create_string_element("two")) == SC_OK);
    assert(sc_push(collection, sc_create_char_element('A')) == SC_OK);
    assert(sc_push(collection, sc_create_string_element("four")) == SC_OK);
    StringCollection *slice = sc_slice(collection, 1, 2);
    assert(slice != NULL);
    assert(sc_size(slice) == 2);
    const Element *element = NULL;
    assert(sc_get(slice, 0, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "two") == 0);
    assert(sc_get(slice, 1, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "A") == 0);
    sc_destroy(slice);
    sc_destroy(collection);
    printf("test_collection_slice: OK\n");
}

static void test_split_words(void) {
    StringCollection *words = string_split_words("Hello   world from   C");
    assert(words != NULL);
    assert(sc_size(words) == 4);
    const Element *element = NULL;
    assert(sc_get(words, 0, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "Hello") == 0);
    assert(sc_get(words, 1, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "world") == 0);
    assert(sc_get(words, 2, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "from") == 0);
    assert(sc_get(words, 3, &element) == SC_OK);
    assert(strcmp(element_as_cstring(element), "C") == 0);
    sc_destroy(words);
    printf("test_split_words: OK\n");
}

void run_all_tests(void) {
    printf("Running tests...\n\n");
    test_string_concat();
    test_string_substring();
    test_collection_push_and_get();
    test_collection_slice();
    test_split_words();
    printf("\nAll tests passed successfully.\n\n");
}
