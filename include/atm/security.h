#ifndef SECURITY_H
#define SECURITY_H

#include <stdbool.h> // Para usar o tipo bool
#include <stddef.h>  // Para usar size_t

// Define o tamanho esperado para o output do hash SHA-256 em formato hexadecimal (64 chars + null terminator)
#define SHA256_HEX_LEN 65
// Define o tamanho da chave AES em bytes (AES-256)
#define AES_KEY_LEN 32
// Define o tamanho do vetor de inicialização (IV) para AES em bytes
#define AES_IV_LEN 16

/**
 * @brief Gera o hash SHA-256 de um PIN combinado com um salt.
 *
 * A função concatena o PIN e o salt, calcula o hash SHA-256
 * e retorna a representação hexadecimal do hash.
 *
 * @param pin O PIN a ser hasheado.
 * @param salt O salt a ser combinado com o PIN.
 * @param output_hash_hex Buffer para armazenar o hash hexadecimal resultante (deve ter pelo menos SHA256_HEX_LEN bytes).
 * @return true se o hash foi gerado com sucesso, false caso contrário.
 */
bool hash_pin(const char *pin, const char *salt, char *output_hash_hex);

/**
 * @brief Criptografa dados usando AES-256-CBC.
 *
 * @param input_data Ponteiro para os dados de entrada a serem criptografados.
 * @param input_len Comprimento dos dados de entrada.
 * @param key Chave de criptografia (deve ter AES_KEY_LEN bytes).
 * @param iv Vetor de Inicialização (deve ter AES_IV_LEN bytes).
 * @param output_buffer Buffer para armazenar os dados criptografados.
 * O tamanho deste buffer deve ser input_len + AES_BLOCK_SIZE.
 * @param encrypted_len Ponteiro para armazenar o tamanho dos dados criptografados.
 * @return true se a criptografia foi bem-sucedida, false caso contrário.
 */
bool criptografar_data(const unsigned char *input_data, size_t input_len,
                       const unsigned char *key, const unsigned char *iv,
                       unsigned char *output_buffer, size_t *encrypted_len);

/**
 * @brief Descriptografa dados usando AES-256-CBC.
 *
 * (Esta função é complementar à criptografar_data e pode ser necessária)
 *
 * @param input_data_encrypted Ponteiro para os dados criptografados.
 * @param input_len_encrypted Comprimento dos dados criptografados.
 * @param key Chave de criptografia (deve ter AES_KEY_LEN bytes).
 * @param iv Vetor de Inicialização (deve ter AES_IV_LEN bytes).
 * @param output_buffer_decrypted Buffer para armazenar os dados descriptografados.
 * O tamanho deve ser suficiente (geralmente igual ao input_len_encrypted).
 * @param decrypted_len Ponteiro para armazenar o tamanho dos dados descriptografados.
 * @return true se a descriptografia foi bem-sucedida, false caso contrário.
 */
bool descriptografar_data(const unsigned char *input_data_encrypted, size_t input_len_encrypted,
                          const unsigned char *key, const unsigned char *iv,
                          unsigned char *output_buffer_decrypted, size_t *decrypted_len);


/**
 * @brief Valida a integridade de um arquivo comparando seu hash SHA-256 com um hash esperado.
 *
 * @param filepath Caminho para o arquivo a ser validado.
 * @param expected_hash_hex Hash SHA-256 hexadecimal esperado para o arquivo.
 * @return true se o hash do arquivo corresponde ao esperado (íntegro), false caso contrário ou se ocorrer um erro.
 */
bool validar_integridade_arquivo(const char *filepath, const char *expected_hash_hex);

/**
 * @brief Gera um salt aleatório.
 *
 * @param salt_buffer Buffer para armazenar o salt gerado (deve ter pelo menos length + 1 bytes).
 * @param length O comprimento do salt a ser gerado (excluindo o terminador nulo).
 * @return true se o salt foi gerado com sucesso, false caso contrário.
 */
bool gerar_salt(char *salt_buffer, size_t length);

#endif // SECURITY_H


// RAFAEL SANTOS
