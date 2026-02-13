#ifndef API_CLIENT_H
#define API_CLIENT_H

#include "../data/structs.h"

// Initializes the API client
void api_init(void);

// Cleans up resources
void api_cleanup(void);

// Fetches a word definition from the API.
// Uses char* buffers directly to be independent of specific struct versions if possible,
// or avoids the old Word struct.
// Returns true if found.
bool fetch_word_definition(const char* word_text, char* out_definition, int max_len);

#endif // API_CLIENT_H
