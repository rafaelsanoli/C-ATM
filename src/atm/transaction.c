#include "../../include/atm/transaction.h"
#include "../../include/atm/storage.h" // Para a struct Conta
#include <stdio.h>
#include <string.h> // Para strcpy, se necessário
#include <time.h>   // Para simular data/hora

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
void registrar_transacao(const char *tipo, const Conta *conta, double valor, const char *destino_id) {
    time_t agora = time(NULL);
    char buffer_tempo[30];
    strftime(buffer_tempo, sizeof(buffer_tempo), "%Y-%m-%d %H:%M:%S", localtime(&agora));

    // Em uma implementação real, isso escreveria em transaction_log.json
    printf("[LOG TRANSACAO %s] Tipo: %s, ContaID: %s, Nome: %s, Valor: %.2f",
           buffer_tempo, tipo, conta->id, conta->nome, valor);
    if (destino_id != NULL) {
        printf(", ContaDestinoID: %s", destino_id);
    }
    printf("\n");
}

//Gabriel de Almeida
