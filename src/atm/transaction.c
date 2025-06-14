#include "../../include/atm/transaction.h"
#include "../../include/atm/storage.h" // Para a struct Conta
#include <stdio.h>
#include <string.h> // Para strcpy, se necessário
#include <time.h>   // Para simular data/hora
#include <json-c/json.h> 

// Função mock para depositar
int depositar(Conta *conta, double valor) {
    if (conta == NULL) {
        printf("Erro: Conta invalida para deposito.\n");
        return 0; // Falha
    }
    if (valor <= 0) {
        printf("Valor de deposito invalido: R$ %.2f\n", valor);
        return 0; // Falha
    }

    printf("[MOCK TRANSACAO] Depositando R$ %.2f na conta %s (Titular: %s)\n", valor, conta->id, conta->nome);
    conta->saldo += valor;
    printf("[MOCK TRANSACAO] Novo saldo: R$ %.2f\n", conta->saldo);

    // Simula o registro da transação
    registrar_transacao("DEPOSITO", conta, valor, NULL);

    // Em uma implementação real, aqui você chamaria uma função para salvar
    // o estado atualizado das contas no arquivo JSON.
    // ex: salvar_contas(lista_global_de_contas, total_contas_global);

    return 1; // Sucesso
}

// Função mock para sacar
int sacar(Conta *conta, double valor) {
    if (conta == NULL) {
        printf("Erro: Conta invalida para saque.\n");
        return 0; // Falha
    }
    if (valor <= 0) {
        printf("Valor de saque invalido: R$ %.2f\n", valor);
        return 0; // Falha
    }

    printf("[MOCK TRANSACAO] Tentando sacar R$ %.2f da conta %s (Titular: %s)\n", valor, conta->id, conta->nome);
    if (conta->saldo >= valor) {
        conta->saldo -= valor;
        printf("[MOCK TRANSACAO] Saque realizado. Novo saldo: R$ %.2f\n", conta->saldo);
        registrar_transacao("SAQUE", conta, valor, NULL);
        // Em uma implementação real, salvaria o estado das contas.
        return 1; // Sucesso
    } else {
        printf("[MOCK TRANSACAO] Saldo insuficiente (R$ %.2f) para sacar R$ %.2f.\n", conta->saldo, valor);
        return 0; // Falha
    }
}

// Função mock para transferir
int transferir(Conta *origem, Conta *destino, double valor) {
    if (origem == NULL || destino == NULL) {
        printf("Erro: Conta de origem ou destino invalida para transferencia.\n");
        return 0; // Falha
    }
    if (valor <= 0) {
        printf("Valor de transferencia invalido: R$ %.2f\n", valor);
        return 0; // Falha
    }

    printf("[MOCK TRANSACAO] Transferindo R$ %.2f da conta %s para %s\n", valor, origem->id, destino->id);
    if (origem->saldo >= valor) {
        origem->saldo -= valor;
        destino->saldo += valor;
        printf("[MOCK TRANSACAO] Transferencia realizada.\n");
        printf("[MOCK TRANSACAO] Novo saldo %s: R$ %.2f\n", origem->id, origem->saldo);
        printf("[MOCK TRANSACAO] Novo saldo %s: R$ %.2f\n", destino->id, destino->saldo);
        // Registra para a origem
        registrar_transacao("TRANSFERENCIA_SAIDA", origem, valor, destino->id);
        // Registra para o destino
        registrar_transacao("TRANSFERENCIA_ENTRADA", destino, valor, origem->id);
        // Em uma implementação real, salvaria o estado das contas.
        return 1; // Sucesso
    } else {
        printf("[MOCK TRANSACAO] Saldo insuficiente na conta de origem (%s) para transferir R$ %.2f.\n", origem->id, valor);
        return 0; // Falha
    }
}

// Função mock para registrar transação
// Substitua a função registrar_transacao existente por esta
void registrar_transacao(const char *tipo, const Conta *conta, double valor, const char *destino_id) {
    const char *filepath = "../data/transaction_log.json";
    
    // 1. Ler o arquivo de log existente
    FILE *fp = fopen(filepath, "r");
    char buffer[8192]; // Buffer para ler o arquivo, aumente se o log ficar muito grande
    if (fp != NULL) {
        fread(buffer, 1, sizeof(buffer), fp);
        fclose(fp);
    } else {
        // Se o arquivo não existe, começamos com um array vazio
        strcpy(buffer, "[]");
    }

    struct json_object *parsed_json = json_tokener_parse(buffer);
    if (!parsed_json) {
         // Se o arquivo estiver vazio ou corrompido, cria um novo array
        parsed_json = json_object_new_array();
    }

    // 2. Criar o objeto JSON para a nova transação
    struct json_object *transacao_obj = json_object_new_object();
    
    // Pega a data e hora atual
    time_t agora = time(NULL);
    char buffer_tempo[30];
    strftime(buffer_tempo, sizeof(buffer_tempo), "%Y-%m-%d %H:%M:%S", localtime(&agora));

    // Adiciona os campos ao objeto
    json_object_object_add(transacao_obj, "timestamp", json_object_new_string(buffer_tempo));
    json_object_object_add(transacao_obj, "tipo", json_object_new_string(tipo));
    json_object_object_add(transacao_obj, "contaId", json_object_new_string(conta->id));
    json_object_object_add(transacao_obj, "valor", json_object_new_double(valor));

    if (destino_id != NULL) {
        printf(", ContaDestinoID: %s", destino_id);
    }
    printf("\n");
}