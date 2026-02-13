#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_client.h"

// STUB IMPLEMENTATION - REMOVING LIBCURL DEPENDENCY FOR EASIER BUILD
// This forces the system to use the local dictionary files (fallback).

void api_init(void) {
    // No-op
    printf("API Client initialized (STUB MODE - Offline Only)\n");
}

void api_cleanup(void) {
    // No-op
}

bool fetch_word_definition(const char* word_text, char* out_definition, int max_len) {
    // Always return false to trigger local dictionary fallback
    // Use snprintf to ensure buffer is not empty just in case
    // snprintf(out_definition, max_len, "API disabled. Using local dictionary.");
    return false; 
}
