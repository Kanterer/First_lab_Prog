#include "string_utils.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *string_concat(const char *left, const char *right) {
    if (left == NULL || right == NULL) {
        return NULL;
    }

    size_t left_len = strlen(left);
    size_t right_len = strlen(right);

    char *result = malloc((left_len + right_len + 1) * sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    strcpy(result, left);
    strcat(result, right);

    return result;
}

char *string_substring(const char *str, size_t i, size_t j) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);

    if (i >= len || j >= len || i > j) {
        return NULL;
    }

    size_t result_len = j - i + 1;

    char *result = malloc((result_len + 1) * sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    strncpy(result, str + i, result_len);
    result[result_len] = '\0';

    return result;
}

StringCollection *string_split_words(const char *str) {
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

        char *word = string_substring(str, start, end);

        if (word == NULL) {
            sc_destroy(result);
            return NULL;
        }

        Element *element = sc_create_string_element(word);
        free(word);

        if (element == NULL) {
            sc_destroy(result);
            return NULL;
        }

        SCStatus status = sc_push(result, element);

        if (status != SC_OK) {
            element_destroy(element);
            sc_destroy(result);
            return NULL;
        }
    }

    return result;
}