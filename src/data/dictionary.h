#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include "structs.h"

// Initialize dictionary system
void dict_init(void);

// Checks if a word is valid and populates definition if pointer provided
// Returns true if valid/found.
bool dict_check_word(const char* word, char* out_def, int max_len);

// Search for candidate words by size (Mock implementation for now)
// In a real scenario, this would check a local trie/list
void dict_search_by_size(int size, char*** out_words, int* out_count);

#endif // DICTIONARY_H
