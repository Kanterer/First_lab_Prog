#include "string_collection.h"

#include <stdio.h>
#include <stdlib.h>

static void print_menu(void) {
    printf("\n===== String Collection Menu =====\n");
    printf("1. Add character to collection\n");
    printf("2. Add string to collection\n");
    printf("3. Print collection\n");
    printf("4. Concatenate two strings\n");
    printf("5. Get substring from string\n");
    printf("6. Get collection elements from i to j\n");
    printf("7. Split string into words\n");
    printf("8. Run tests\n");
    printf("0. Exit\n");
    printf("Choose option: ");
}

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    size_t i = 0;

    while (buffer[i] != '\0') {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }

        i++;
    }
}

int main(void) {
    StringCollection *collection = sc_create();

    if (collection == NULL) {
        printf("Error: failed to create collection.\n");
        return 1;
    }

    int option;
    char buffer1[512];
    char buffer2[512];

    do {
        print_menu();

        if (scanf("%d", &option) != 1) {
            printf("Input error.\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer();

        switch (option) {
            case 1: {
                char ch;

                printf("Enter character: ");

                if (scanf("%c", &ch) != 1) {
                    printf("Input error.\n");
                    clear_input_buffer();
                    break;
                }

                clear_input_buffer();

                SCStatus status = sc_push_char(collection, ch);

                if (status == SC_OK) {
                    printf("Character added successfully.\n");
                } else {
                    printf("Error: failed to add character.\n");
                }

                break;
            }

            case 2: {
                printf("Enter string: ");
                read_line(buffer1, sizeof(buffer1));

                SCStatus status = sc_push_string(collection, buffer1);

                if (status == SC_OK) {
                    printf("String added successfully.\n");
                } else {
                    printf("Error: failed to add string.\n");
                }

                break;
            }

            case 3: {
                printf("Current collection:\n");
                sc_print(collection);
                break;
            }

            case 4: {
                printf("Enter first string: ");
                read_line(buffer1, sizeof(buffer1));

                printf("Enter second string: ");
                read_line(buffer2, sizeof(buffer2));

                char *result = sc_concat_strings(buffer1, buffer2);

                if (result == NULL) {
                    printf("Error: failed to concatenate strings.\n");
                } else {
                    printf("Result: %s\n", result);
                    free(result);
                }

                break;
            }

            case 5: {
                size_t i;
                size_t j;

                printf("Enter string: ");
                read_line(buffer1, sizeof(buffer1));

                printf("Enter i: ");

                if (scanf("%zu", &i) != 1) {
                    printf("Input error.\n");
                    clear_input_buffer();
                    break;
                }

                printf("Enter j: ");

                if (scanf("%zu", &j) != 1) {
                    printf("Input error.\n");
                    clear_input_buffer();
                    break;
                }

                clear_input_buffer();

                char *result = sc_substring(buffer1, i, j);

                if (result == NULL) {
                    printf("Error: incorrect indexes or memory error.\n");
                } else {
                    printf("Substring: %s\n", result);
                    free(result);
                }

                break;
            }

            case 6: {
                size_t i;
                size_t j;

                printf("Enter i: ");

                if (scanf("%zu", &i) != 1) {
                    printf("Input error.\n");
                    clear_input_buffer();
                    break;
                }

                printf("Enter j: ");

                if (scanf("%zu", &j) != 1) {
                    printf("Input error.\n");
                    clear_input_buffer();
                    break;
                }

                clear_input_buffer();

                StringCollection *slice = sc_slice(collection, i, j);

                if (slice == NULL) {
                    printf("Error: incorrect indexes or memory error.\n");
                } else {
                    printf("Slice:\n");
                    sc_print(slice);
                    sc_destroy(slice);
                }

                break;
            }

            case 7: {
                printf("Enter string: ");
                read_line(buffer1, sizeof(buffer1));

                StringCollection *words = sc_split_words(buffer1);

                if (words == NULL) {
                    printf("Error: failed to split string.\n");
                } else {
                    printf("Words:\n");
                    sc_print(words);
                    sc_destroy(words);
                }

                break;
            }

            case 8: {
                run_all_tests();
                break;
            }

            case 0: {
                printf("Program finished.\n");
                break;
            }

            default: {
                printf("Unknown option.\n");
                break;
            }
        }

    } while (option != 0);

    sc_destroy(collection);

    return 0;
}