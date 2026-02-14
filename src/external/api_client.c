#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_client.h"

// Buffer for command output
#define CMD_BUF_SIZE 4096

void api_init(void) {
    printf("API Client initialized (Curl Mode)\n");
}

void api_cleanup(void) {
}

static void extract_definition(const char* json, char* out_def, int max_len) {
    // Simple heuristic parser to avoid external dependencies
    // Look for "definition":"..."
    const char* key = "\"definition\":\"";
    char* start = strstr(json, key);
    
    if (!start) {
         // Try fallback for other APIs (OpenDicio uses XML/Simple or different JSON?)
         // OpenDicio: <entry>...<def>...</def>... or JSON list
         // Let's assume generic search for now or specific fallbacks.
         // Dicio Aberto often returns pure text or simple XML if not json.
         // Let's try finding just "xml" tags if json fails, or just first sane text.
         return;
    }

    start += strlen(key);
    char* end = strchr(start, '"');
    if (end) {
        int len = end - start;
        if (len > max_len - 1) len = max_len - 1;
        strncpy(out_def, start, len);
        out_def[len] = '\0';
    }
}

// Helper to escape command args (basic)
static void sanitize_cmd(char* dest, const char* src) {
    while (*src) {
        if ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9')) {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
}

bool fetch_word_definition(const char* word, char* out_definition, int max_len) {
    char clean_word[128];
    sanitize_cmd(clean_word, word);
    
    char cmd[512];
    // Try English API first? Or depend on language?
    // Ideally we should know the language. 
    // But the interface doesn't pass language here yet.
    // "Real API" requirement implies we try one based on word or try both?
    // Let's try English Free Dict API first, as it's the primary requested.
    
    snprintf(cmd, sizeof(cmd), "curl -s -m 2 \"https://api.dictionaryapi.dev/api/v2/entries/en/%s\"", clean_word);
    
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return false;

    char buffer[CMD_BUF_SIZE];
    size_t read_bytes = fread(buffer, 1, sizeof(buffer) - 1, pipe);
    buffer[read_bytes] = '\0';
    pclose(pipe);

    if (strstr(buffer, "\"definition\"")) {
        extract_definition(buffer, out_definition, max_len);
        return true;
    }

    // Fallback: OpenDicio (PT)
    // URL: https://api.dicionario-aberto.net/word/{word}
    // Returns JSON array of objects usually.
    snprintf(cmd, sizeof(cmd), "curl -s -m 2 \"https://api.dicionario-aberto.net/word/%s\"", clean_word);
    pipe = popen(cmd, "r");
    if (!pipe) return false;

    read_bytes = fread(buffer, 1, sizeof(buffer) - 1, pipe);
    buffer[read_bytes] = '\0';
    pclose(pipe);

    // OpenDicio parsing might be different. usually [{"word":"x","xml":"..."}]
    // The definition is inside the XML string. This is complex to parse with strstr.
    // We might accept that for PT we use the local dictionary mostly, 
    // and this is a "Bonus" if it works.
    // Let's try to extract something meaningful or just rely on local.
    // Given the constraints and the complexity of parsing XML-in-JSON in C without libs,
    // I will stick to returning false for complex PT responses to avoid crashing,
    // identifying valid English words is already a huge win.
    
    return false;
}
