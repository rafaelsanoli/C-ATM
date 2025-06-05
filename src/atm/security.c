#include "atm/security.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>    // Para SHA256
#include <openssl/evp.h>    // Para AES (EVP_CIPHER) e outras funções EVP
#include <openssl/rand.h>   // Para gerar salts e IVs seguros (RAND_bytes)
#include <openssl/err.h>    // Para mensagens de erro do OpenSSL (ERR_print_errors_fp)
#include <limits.h>         // Para INT_MAX


/**
 * @brief Converte uma sequência de bytes para sua representação hexadecimal.
 *
 * @param bytes Ponteiro para os bytes de entrada.
 * @param len Comprimento da sequência de bytes.
 * @param hex_str Buffer de saída para a string hexadecimal (deve ter tamanho len * 2 + 1).
 */
static void bytes_para_hex(const unsigned char *bytes, size_t len, char *hex_str) {
    for (size_t i = 0; i < len; i++) {
        sprintf(hex_str + (i * 2), "%02x", bytes[i]);
    }
    hex_str[len * 2] = '\0'; // Adiciona o terminador nulo
}

// --- Implementações das Funções de Segurança ---

bool hash_pin(const char *pin, const char *salt, char *output_hash_hex) {
    if (!pin || !salt || !output_hash_hex) {
        fprintf(stderr, "Erro [hash_pin]: Argumentos nulos fornecidos.\n");
        return false;
    }

    // Buffer para pin + salt. Ajuste o tamanho se PINs ou salts muito longos forem esperados.
    // Para um PIN típico e salt, 256 é geralmente suficiente.
    char combined_input[256];
    int written = snprintf(combined_input, sizeof(combined_input), "%s%s", pin, salt);

    if (written < 0 || (size_t)written >= sizeof(combined_input)) {
        fprintf(stderr, "Erro [hash_pin]: Erro ao concatenar PIN e salt ou buffer pequeno demais.\n");
        return false;
    }

    unsigned char hash_binario[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;

    if (!SHA256_Init(&sha256_ctx)) {
        fprintf(stderr, "Erro [hash_pin]: Falha ao inicializar SHA256_CTX.\n");
        // Em versões mais recentes do OpenSSL, SHA256_Init sempre retorna 1
        // Mas é melhor você testar e verificar depois !!!!!!!!!!!!!!!!!!!!!!
        // !!!!
        // !!!!
        // !!!!
        // !!!!
        return false;
    }
    if (!SHA256_Update(&sha256_ctx, combined_input, strlen(combined_input))) {
        fprintf(stderr, "Erro [hash_pin]: Falha ao atualizar SHA256_CTX.\n");
        return false;
    }
    if (!SHA256_Final(hash_binario, &sha256_ctx)) {
        fprintf(stderr, "Erro [hash_pin]: Falha ao finalizar SHA256_CTX.\n");
        return false;
    }

    bytes_para_hex(hash_binario, SHA256_DIGEST_LENGTH, output_hash_hex);
    return true;
}

bool criptografar_data(const unsigned char *input_data, size_t input_len,
                       const unsigned char *key, const unsigned char *iv,
                       unsigned char *output_buffer, size_t *encrypted_len) {

    if (!input_data || !key || !iv || !output_buffer || !encrypted_len) {
        fprintf(stderr, "Erro [criptografar_data]: Argumentos nulos fornecidos.\n");
        return false;
    }

    // Verifica se input_len excede INT_MAX, pois funções EVP usam int para length
    if (input_len > INT_MAX) {
        fprintf(stderr, "Erro [criptografar_data]: Comprimento dos dados de entrada (%zu) excede INT_MAX (%d).\n", input_len, INT_MAX);
        return false;
    }

    EVP_CIPHER_CTX *ctx = NULL; // Inicializar para NULL
    int len_update = 0;
    int len_final = 0;
    bool success = false;

    // Cria e inicializa o contexto do cipher
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        fprintf(stderr, "Erro [criptografar_data]: Falha ao criar EVP_CIPHER_CTX.\n");
        ERR_print_errors_fp(stderr);
        goto cleanup; // Pula para a limpeza
    }

    // Inicializa a operação de criptografia. AES-256-CBC é uma boa, mas testa também
    // !!!!!
    // !!!!  LEMBRA DE TESTAR!!!
    // !!!!
    // !!!
    // A função chamadora é responsável por garantir que 'key' e 'iv' tenham os tamanhos corretos.
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        fprintf(stderr, "Erro [criptografar_data]: Falha ao inicializar a criptografia (EVP_EncryptInit_ex).\n");
        ERR_print_errors_fp(stderr);
        goto cleanup;
    }

    // Fornece a mensagem a ser criptografada.
    if (1 != EVP_EncryptUpdate(ctx, output_buffer, &len_update, input_data, (int)input_len)) {
        fprintf(stderr, "Erro [criptografar_data]: Falha no EVP_EncryptUpdate.\n");
        ERR_print_errors_fp(stderr);
        goto cleanup;
    }

    // Finaliza a criptografia (escreve o padding, se necessário).
    if (1 != EVP_EncryptFinal_ex(ctx, output_buffer + len_update, &len_final)) {
        fprintf(stderr, "Erro [criptografar_data]: Falha no EVP_EncryptFinal_ex.\n");
        ERR_print_errors_fp(stderr);
        goto cleanup;
    }

    *encrypted_len = (size_t)len_update + (size_t)len_final;
    success = true;

cleanup:
    if (ctx) {
        EVP_CIPHER_CTX_free(ctx);
    }
    return success;
}

bool descriptografar_data(const unsigned char *input_data_encrypted, size_t input_len_encrypted,
                          const unsigned char *key, const unsigned char *iv,
                          unsigned char *output_buffer_decrypted, size_t *decrypted_len) {
    if (!input_data_encrypted || !key || !iv || !output_buffer_decrypted || !decrypted_len) {
        fprintf(stderr, "Erro [descriptografar_data]: Argumentos nulos fornecidos.\n");
        return false;
    }

    if (input_len_encrypted > INT_MAX) {
        fprintf(stderr, "Erro [descriptografar_data]: Comprimento dos dados de entrada (%zu) excede INT_MAX (%d).\n", input_len_encrypted, INT_MAX);
        return false;
    }

    // OpenSSL pode ter problemas com input de tamanho 0 para descriptografia em alguns modos/versões então é melhor você dar uma olhada depois
    // Se for um caso de uso, pode ser necessário tratamento especial ou garantir com certeza que não aconteça
    if (input_len_encrypted == 0) {
        fprintf(stderr, "Warning [descriptografar_data]: Comprimento dos dados de entrada é zero.\n");
        *decrypted_len = 0;
        return true;
    }

    EVP_CIPHER_CTX *ctx = NULL; // inicia para null
    int len_update = 0;
    int len_final = 0;
    bool success = false;

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        fprintf(stderr, "Erro [descriptografar_data]: Falha ao criar EVP_CIPHER_CTX.\n");
        ERR_print_errors_fp(stderr);
        goto cleanup_decrypt;
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        fprintf(stderr, "Erro [descriptografar_data]: Falha ao inicializar a descriptografia (EVP_DecryptInit_ex).\n");
        ERR_print_errors_fp(stderr);
        goto cleanup_decrypt;
    }

    // EVP_CIPHER_CTX_set_padding(ctx, 0); // Descomente se você sabe que não vai usar padding
                                        // Por padrão, o padding é aguardado e depois removido

    if (1 != EVP_DecryptUpdate(ctx, output_buffer_decrypted, &len_update, input_data_encrypted, (int)input_len_encrypted)) {
        fprintf(stderr, "Erro [descriptografar_data]: Falha no EVP_DecryptUpdate.\n");
        ERR_print_errors_fp(stderr);
        goto cleanup_decrypt;
    }

    if (1 != EVP_DecryptFinal_ex(ctx, output_buffer_decrypted + len_update, &len_final)) {
        fprintf(stderr, "Erro [descriptografar_data]: Falha no EVP_DecryptFinal_ex (verifique chave, IV, padding ou dados corrompidos).\n");
        ERR_print_errors_fp(stderr);
        goto cleanup_decrypt;
    }

    *decrypted_len = (size_t)len_update + (size_t)len_final;
    success = true;

cleanup_decrypt:
    if (ctx) {
        EVP_CIPHER_CTX_free(ctx);
    }
    return success;
}

bool validar_integridade_arquivo(const char *filepath, const char *expected_hash_hex) {
    if (!filepath || !expected_hash_hex) {
        fprintf(stderr, "Erro [validar_integridade_arquivo]: Argumentos nulos fornecidos.\n");
        return false;
    }

    FILE *file = fopen(filepath, "rb"); // Abre em modo binário
    if (!file) {
        fprintf(stderr, "Erro [validar_integridade_arquivo]: Nao foi possivel abrir o arquivo '%s'. ", filepath);
        perror(NULL); // Adiciona a mensagem de erro do sistema (ex: "No such file or directory")
        return false;
    }

    SHA256_CTX sha256_ctx;
    if (!SHA256_Init(&sha256_ctx)) {
        fprintf(stderr, "Erro [validar_integridade_arquivo]: Falha ao inicializar SHA256_CTX.\n");
        fclose(file);
        return false;
    }

    const size_t buffer_size = 4096; // Buffer de 4KB para leitura, qualquer coisa aumenta depois
    unsigned char buffer[buffer_size];
    size_t bytes_lidos;
    bool error_occurred = false;

    while ((bytes_lidos = fread(buffer, 1, buffer_size, file)) > 0) {
        if (!SHA256_Update(&sha256_ctx, buffer, bytes_lidos)) {
            fprintf(stderr, "Erro [validar_integridade_arquivo]: Falha ao atualizar SHA256_CTX durante leitura do arquivo.\n");
            error_occurred = true;
            break;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Erro [validar_integridade_arquivo]: Erro de leitura do arquivo '%s'. ", filepath);
        perror(NULL);
        error_occurred = true;
    }
    fclose(file);

    if (error_occurred) {
        return false;
    }

    unsigned char hash_binario_calculado[SHA256_DIGEST_LENGTH];
    if (!SHA256_Final(hash_binario_calculado, &sha256_ctx)) {
        fprintf(stderr, "Erro [validar_integridade_arquivo]: Falha ao finalizar SHA256_CTX.\n");
        return false;
    }

    char hash_hex_calculado[SHA256_HEX_LEN]; // Definido no security.h como 65 (64 + null)
    bytes_para_hex(hash_binario_calculado, SHA256_DIGEST_LENGTH, hash_hex_calculado);

    // Compara o hash calculado com o esperado
    // strcmp é seguro aqui pois ambos devem ser strings C válidas (null-terminated)
    if (strcmp(hash_hex_calculado, expected_hash_hex) == 0) {
        return true; // Íntegro
    } else {
        // BOA PRATICA: não logar os hashes em produção se forem sensíveis,
        // mas útil para depuração.
        // fprintf(stderr, "Falha na verificação de integridade do arquivo '%s'.\n", filepath);
        // fprintf(stderr, "Esperado: %s\n", expected_hash_hex);
        // fprintf(stderr, "Calculado: %s\n", hash_hex_calculado);
        return false; // Não íntegro ou hashes diferentes
    }
}

bool gerar_salt(char *salt_buffer, size_t length_bytes_aleatorios) {
    // O salt_buffer precisa ter espaço para (length_bytes_aleatorios * 2) caracteres hex + 1 para o '\0'
    if (!salt_buffer || length_bytes_aleatorios == 0) {
        fprintf(stderr, "Erro [gerar_salt]: Buffer de salt inválido ou comprimento zero especificado.\n");
        return false;
    }

    // Aloca um buffer temporário para os bytes aleatórios no heap para evitar estouro de pilha
    // se length_bytes_aleatorios for grande, embora para salts isso seja incomum
    unsigned char *random_bytes = malloc(length_bytes_aleatorios);
    if (!random_bytes) {
        fprintf(stderr, "Erro [gerar_salt]: Falha ao alocar memória para bytes aleatórios.\n");
        return false;
    }

    if (RAND_bytes(random_bytes, length_bytes_aleatorios) != 1) {
        ERR_print_errors_fp(stderr); // Mostra erros específicos do OpenSSL
        fprintf(stderr, "Erro [gerar_salt]: Falha ao gerar bytes aleatórios para o salt (RAND_bytes).\n");
        free(random_bytes);
        return false;
    }

    // Converte bytes aleatórios para uma string hexadecimal
    // A função chamadora deve garantir que salt_buffer seja grande o suficiente
    // Ex: se length_bytes_aleatorios = 16, salt_buffer precisa de 33 chars
    bytes_para_hex(random_bytes, length_bytes_aleatorios, salt_buffer);

    free(random_bytes);
    return true;
}
