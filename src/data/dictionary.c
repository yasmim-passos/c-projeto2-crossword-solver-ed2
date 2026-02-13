#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include "../external/api_client.h"

void dict_init(void) {
    api_init();
}


// Helper to check local file
static bool check_local_dictionary(const char* filename, const char* word, char* out_def, int max_len) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open local dictionary: %s\n", filename);
        return false;
    }

    char line[512];
    bool found = false;
    int wordLen = strlen(word);

    while (fgets(line, sizeof(line), file)) {
        // Format: WORD:Definition
        // Check if line starts with word
        if (strncasecmp(line, word, wordLen) == 0) {
            if (line[wordLen] == ':') {
                // Found match
                char* defStart = line + wordLen + 1;
                // Remove newline
                char* newline = strrchr(defStart, '\n');
                if (newline) *newline = '\0';
                
                if (out_def) {
                    strncpy(out_def, defStart, max_len - 1);
                    out_def[max_len - 1] = '\0';
                }
                found = true;
                break;
            }
        }
    }

    fclose(file);
    return found;
}

bool dict_check_word(const char* word, char* out_def, int max_len) {
    // 1. Check API
    char temp_def[TAMANHO_MAX_DICA];
    if (fetch_word_definition(word, temp_def, sizeof(temp_def))) {
        if (out_def) {
            strncpy(out_def, temp_def, max_len - 1);
            out_def[max_len - 1] = '\0';
        }
        return true;
    }
    
    // 2. Fallback to Local File
    // Try multiple paths to find the dictionary file
    const char* paths[] = {
        "data_files/dictionaries/en.txt",
        "../data_files/dictionaries/en.txt",
        "../../data_files/dictionaries/en.txt"
    };
    const char* pathsPT[] = {
        "data_files/dictionaries/pt.txt",
        "../data_files/dictionaries/pt.txt",
        "../../data_files/dictionaries/pt.txt"
    };

    bool found = false;
    
    // Check EN (or requested language - basic logic for now)
    for(int i=0; i<3; i++) {
        if(check_local_dictionary(paths[i], word, out_def, max_len)) {
            found = true;
            break;
        }
    }
    
    if(!found) {
        for(int i=0; i<3; i++) {
            if(check_local_dictionary(pathsPT[i], word, out_def, max_len)) {
                found = true;
                break;
            }
        }
    }
    
    if (found) return true;
    
    return false;
}

void dict_search_by_size(int size, char*** out_words, int* out_count) {
    // Mock implementation returning some static words for testing
    // In real implementation, this would scan a dictionary file
    
    *out_count = 3;
    *out_words = malloc(sizeof(char*) * (*out_count));
    
    (*out_words)[0] = strdup("HELLO");
    (*out_words)[1] = strdup("WORLD");
    (*out_words)[2] = strdup("BRAIN");
}
