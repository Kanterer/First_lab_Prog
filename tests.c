#include "string_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void test_concat(void) {
    char *result = sc_concat_strings("Hello, ", "world!");

    assert(result != NULL);
    assert(strcmp(result, "Hello, world!") == 0);

    free(result);

    printf("test_concat: OK\n");
}

static void test_substring(void) {
    char *result = sc_substring("abcdef", 1, 3);

    assert(result != NULL);
    assert(strcmp(result, "bcd") == 0);

    free(result);

    assert(sc_substring("abcdef", 4, 2) == NULL);
    assert(sc_substring("abcdef", 10, 12) == NULL);

    printf("test_substring: OK\n");
}

static void test_collection_push_and_get(void) {
    StringCollection *collection = sc_create();

    assert(collection != NULL);
    assert(sc_size(collection) == 0);

    assert(sc_push_char(collection, 'A') == SC_OK);
    assert(sc_push_string(collection, "Hello") == SC_OK);

    assert(sc_size(collection) == 2);

    SCElement element;

    assert(sc_get(collection, 0, &element) == SC_OK);
    assert(element.type == SC_ELEMENT_CHAR);
    assert(strcmp(element.value, "A") == 0);

    assert(sc_get(collection, 1, &element) == SC_OK);
    assert(element.type == SC_ELEMENT_STRING);
    assert(strcmp(element.value, "Hello") == 0);

    sc_destroy(collection);

    printf("test_collection_push_and_get: OK\n");
}

static void test_slice(void) {
    StringCollection *collection = sc_create();

    assert(collection != NULL);

    assert(sc_push_string(collection, "one") == SC_OK);
    assert(sc_push_string(collection, "two") == SC_OK);
    assert(sc_push_string(collection, "three") == SC_OK);
    assert(sc_push_string(collection, "four") == SC_OK);

    StringCollection *slice = sc_slice(collection, 1, 2);

    assert(slice != NULL);
    assert(sc_size(slice) == 2);

    SCElement element;

    assert(sc_get(slice, 0, &element) == SC_OK);
    assert(strcmp(element.value, "two") == 0);

    assert(sc_get(slice, 1, &element) == SC_OK);
    assert(strcmp(element.value, "three") == 0);

    sc_destroy(slice);
    sc_destroy(collection);

    printf("test_slice: OK\n");
}

static void test_split_words(void) {
    StringCollection *words = sc_split_words("Hello   world from   C");

    assert(words != NULL);
    assert(sc_size(words) == 4);

    SCElement element;

    assert(sc_get(words, 0, &element) == SC_OK);
    assert(strcmp(element.value, "Hello") == 0);

    assert(sc_get(words, 1, &element) == SC_OK);
    assert(strcmp(element.value, "world") == 0);

    assert(sc_get(words, 2, &element) == SC_OK);
    assert(strcmp(element.value, "from") == 0);

    assert(sc_get(words, 3, &element) == SC_OK);
    assert(strcmp(element.value, "C") == 0);

    sc_destroy(words);

    printf("test_split_words: OK\n");
}

void run_all_tests(void) {
    printf("Running tests...\n\n");

    test_concat();
    test_substring();
    test_collection_push_and_get();
    test_slice();
    test_split_words();

    printf("\nAll tests passed successfully.\n\n");
}