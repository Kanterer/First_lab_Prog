#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

#include "string_collection.h"

char *string_concat(const char *left, const char *right);
char *string_substring(const char *str, size_t i, size_t j);
StringCollection *string_split_words(const char *str);

#endif