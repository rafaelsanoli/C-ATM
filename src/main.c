#include <stdio.h>
#include <string.h>
#include <atm/auth.h>
#include <atm/account.h>
#include <atm/storage.h>

#define MAX_CONTAS 10

int main() {
    Conta contas[MAX_CONTAS];
    int totalContas = 0;

    printf("Iniciando carregamento de contas...\n");

    // Carrega contas do arquivo (ajuste o nome do arquivo real se necessário)
    totalContas = carregar_contas(contas, MAX_CONTAS);
    
    if (totalContas <= 0) {
        printf("Erro ao carregar contas ou nenhuma conta encontrada.\n");
        return 1;
    }

    printf("Contas carregadas com sucesso: %d\n", totalContas);

    // Autenticação do usuário
    int indiceConta = iniciarAutenticacao(contas, totalContas, "dados_contas.txt");
    
    if (indiceConta == -1) {
        printf("Falha na autenticação. Encerrando o programa.\n");
        return 1;
    }

    if (indiceConta < 0 || indiceConta >= totalContas) {
    printf("Índice da conta inválido: %d\n", indiceConta);
    return 1;
    }

    // Exibe os dados da conta autenticada
    printf("\n=== Informações da Conta ===\n");
    printf("Nome: %s\n", contas[indiceConta].nome);
    printf("ID: %s\n", contas[indiceConta].id);
    printf("Saldo: R$ %.2f\n", contas[indiceConta].saldo);

    return 0;
}