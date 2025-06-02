<<<<<<< HEAD
#include <stdio.h>
#include "../../include/atm/account.h"

// Exibe o nome e saldo da conta autenticada
void exibirInformacoesConta(const Conta* conta) {
    if (conta == NULL) {
        printf("Conta inválida.\n");
        return;
    }

    printf("\n=== Informações da Conta ===\n");
    printf("Nome: %s\n", conta->nome);
    printf("ID: %s\n", conta->id);
    printf("Saldo: R$ %.2f\n", conta->saldo);
    printf("============================\n");
}
=======
#include <stdio.h>
#include "../../include/atm/account.h"

// Exibe o nome e saldo da conta autenticada
void exibirInformacoesConta(const Conta* conta) {
    if (conta == NULL) {
        printf("Conta inválida.\n");
        return;
    }

    printf("\n=== Informações da Conta ===\n");
    printf("Nome: %s\n", conta->nome);
    printf("ID: %s\n", conta->id);
    printf("Saldo: R$ %.2f\n", conta->saldo);
    printf("============================\n");
}
// Laylla figueiredo
>>>>>>> 84170fc7b72620339d13e3b926d8cb143b644e58
