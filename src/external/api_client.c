#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api_client.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void api_init(void) {
    curl_global_init(CURL_GLOBAL_ALL);
}

void api_cleanup(void) {
    curl_global_cleanup();
}

static void extract_definition(const char* json, char* buffer, size_t max_len) {
    const char* key = "\"definition\":\"";
    char* found = strstr(json, key);
    if (!found) {
        snprintf(buffer, max_len, "Definicao nao encontrada."); // Localized fallback
        return;
    }

    found += strlen(key);
    size_t i = 0;
    while(found[i] != '\"' && found[i] != '\0' && i < max_len - 1) {
        buffer[i] = found[i];
        i++;
    }
    buffer[i] = '\0';
}

bool fetch_word_definition(const char* word_text, char* out_definition, int max_len) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (!curl) {
        free(chunk.memory);
        return false;
    }

    char url[512];
    snprintf(url, sizeof(url), "https://api.dictionaryapi.dev/api/v2/entries/en/%s", word_text);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl);
    bool success = false;

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        if (strstr(chunk.memory, "No Definitions Found") || strstr(chunk.memory, "title\":\"No Definitions Found")) {
            success = false;
        } else {
            extract_definition(chunk.memory, out_definition, max_len);
            success = true;
        }
    }

    curl_easy_cleanup(curl);
    free(chunk.memory);
    return success;
}
