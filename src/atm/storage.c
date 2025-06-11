#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>  // Biblioteca para trabalhar com arquivos JSON
#include "../../include/atm/storage.h"     // Header com a definição da struct Conta

#define MAX_CONTAS 10     // Número máximo de contas que podem ser carregadas

// Função que carrega as contas a partir do arquivo JSON e preenche o vetor de contas
int carregar_contas(Conta contas[], int max_contas) {
    // Abre o arquivo JSON com as contas no modo leitura
    FILE *fp = fopen("../data/accounts.json", "r");
    if (fp == NULL) {
        // Se não conseguir abrir, imprime erro e retorna 0
        perror("Erro ao abrir o arquivo de contas");
        return 0;
    }

    // Objetos para armazenar os dados interpretados do JSON
    struct json_object *parsed_json, *array, *item;
    size_t n_contas;

    // Buffer para armazenar o conteúdo do arquivo lido
    char buffer[4096];
    fread(buffer, 1, sizeof(buffer), fp); // Lê o conteúdo do arquivo para o buffer
    fclose(fp); // Fecha o arquivo após a leitura

    // Converte o conteúdo do buffer em um objeto JSON manipulável
    parsed_json = json_tokener_parse(buffer);
    if (!parsed_json) {
        // Se houver erro na conversão do JSON, imprime mensagem e sai da função
        fprintf(stderr, "Erro ao fazer parse do JSON.\n");
        return 0;
    }

    array = parsed_json; // O JSON esperado é um array de contas
    n_contas = json_object_array_length(array); // Obtém o número de contas no array

    // Garante que o número de contas não exceda o limite passado
    if (n_contas > max_contas) n_contas = max_contas;

    // Percorre cada item (conta) do array JSON
    for (size_t i = 0; i < n_contas; i++) {
        item = json_object_array_get_idx(array, i); // Pega a conta na posição i

        // Obtém os campos individualmente
        struct json_object *j_nome = json_object_object_get(item, "nome");
        struct json_object *j_id = json_object_object_get(item, "id");
        struct json_object *j_senha = json_object_object_get(item, "senha_hash");
        struct json_object *j_saldo = json_object_object_get(item, "saldo");

        // Verifica se todos os campos existem
        if (!j_nome || !j_id || !j_senha || !j_saldo) {
            fprintf(stderr, "Erro: Campo ausente na conta %zu\n", i);
            continue;
        }

        // Copia os valores do JSON para os campos da struct Conta de forma segura
        strncpy(contas[i].nome, json_object_get_string(j_nome), sizeof(contas[i].nome) - 1);
        contas[i].nome[sizeof(contas[i].nome) - 1] = '\0';

        strncpy(contas[i].id, json_object_get_string(j_id), sizeof(contas[i].id) - 1);
        contas[i].id[sizeof(contas[i].id) - 1] = '\0';

        strncpy(contas[i].senha_hash, json_object_get_string(j_senha), sizeof(contas[i].senha_hash) - 1);
        contas[i].senha_hash[sizeof(contas[i].senha_hash) - 1] = '\0';

        contas[i].saldo = json_object_get_double(j_saldo);
    }

    json_object_put(parsed_json); // Libera a memória usada pelo JSON

    return (int)n_contas; // Retorna a quantidade de contas carregadas
}

//Anderson Vinicios
