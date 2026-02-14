#ifndef API_CLIENT_H
#define API_CLIENT_H

#include "../data/structs.h"

// Inicializa a API do cliente
void api_init(void);

// Limpa recursos
void api_cleanup(void);

// Obtém a definição de uma palavra da API.
// Usa buffers char* diretamente para ser independente de versões específicas da struct, se possível,
// ou evita a antiga struct Word.
// Retorna verdadeiro se encontrado.
bool fetch_word_definition(const char* word_text, char* out_definition, int max_len);

#endif // API_CLIENT_H
