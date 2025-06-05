#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/atm.h"

#ifndef MAX_CONTAS
#define MAX_CONTAS 10
#endif

static Conta contas[MAX_CONTAS];
static int totalContasCarregadas = 0;
static char idUsuarioLogado[20] = {0};
static int indiceUsuarioLogado = -1;

void exibir_menu_principal(void) {
    int opcao;

    if (totalContasCarregadas == 0) {
        totalContasCarregadas = carregar_contas(contas, MAX_CONTAS);
        if (totalContasCarregadas <= 0) {
            printf("Erro critico: Nao foi possivel carregar os dados das contas.\n");
            printf("Verifique o arquivo de dados e as permissoes.\n");
            printf("O sistema sera encerrado em 5 segundos.\n");
            #ifdef _WIN32
                system("timeout /t 5 /nobreak > nul");
            #else
                system("sleep 5");
            #endif
            return;
        }
    }

    do {
        system("clear || cls");
        printf("Bem-vindo ao C-ATM!\n");
        printf("=====================\n");
        if (indiceUsuarioLogado != -1) {
            if (indiceUsuarioLogado >= 0 && indiceUsuarioLogado < totalContasCarregadas) {
                 printf("Usuario Logado: %s (ID: %s)\n", contas[indiceUsuarioLogado].nome, contas[indiceUsuarioLogado].id);
            } else {
                printf("Erro: Estado de login inconsistente. Deslogando automaticamente.\n");
                indiceUsuarioLogado = -1;
                idUsuarioLogado[0] = '\0';
            }
            printf("---------------------\n");
        }

        printf("1. Login\n");
        printf("2. Ver Saldo\n");
        printf("3. Sacar\n");
        printf("4. Depositar\n");
        printf("5. Ver Extrato\n");
        printf("6. Logout\n");
        printf("7. Sair do ATM\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                if (indiceUsuarioLogado != -1) {
                    printf("Voce ja esta logado. Faca logout primeiro para trocar de usuario.\n");
                } else {
                    indiceUsuarioLogado = iniciarAutenticacao(contas, totalContasCarregadas, idUsuarioLogado);
                }
                break;

            case 2:
                if (indiceUsuarioLogado != -1) {
                    exibirInformacoesConta(&contas[indiceUsuarioLogado]);
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 3:
                if (indiceUsuarioLogado != -1) {
                    double valor_saque;
                    printf("Digite o valor para sacar: R$ ");
                    if (scanf("%lf", &valor_saque) == 1) {
                        sacar(&contas[indiceUsuarioLogado], valor_saque);
                    } else {
                        printf("Valor de saque invalido!\n");
                        while (getchar() != '\n');
                    }
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 4:
                if (indiceUsuarioLogado != -1) {
                    double valor_deposito;
                    printf("Digite o valor para depositar: R$ ");
                     if (scanf("%lf", &valor_deposito) == 1) {
                        depositar(&contas[indiceUsuarioLogado], valor_deposito);
                    } else {
                        printf("Valor de deposito invalido!\n");
                        while (getchar() != '\n');
                    }
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 5:
                if (indiceUsuarioLogado != -1) {
                    printf("Funcionalidade de extrato ainda nao implementada.\n");
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 6:
                if (indiceUsuarioLogado != -1) {
                     if (indiceUsuarioLogado >= 0 && indiceUsuarioLogado < totalContasCarregadas) {
                        printf("Logout realizado com sucesso, %s.\n", contas[indiceUsuarioLogado].nome);
                    } else {
                        printf("Logout realizado.\n");
                    }
                    indiceUsuarioLogado = -1;
                    idUsuarioLogado[0] = '\0';
                } else {
                    printf("Nenhum usuario logado para fazer logout.\n");
                }
                break;

            case 7:
                printf("Obrigado por usar o C-ATM. Saindo...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        if (opcao != 7) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n');
            getchar();
        }

    } while (opcao != 7);
}