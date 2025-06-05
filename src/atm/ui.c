#include <stdio.h>
#include <stdlib.h> // Para system()
#include <string.h> // Para strcpy (se usado em idUsuarioLogado)

#include "../../include/atm.h"


#ifndef MAX_CONTAS_DEFINIDO_EM_STORAGE_OU_MAIN
#define MAX_CONTAS_DEFINIDO_EM_STORAGE_OU_MAIN 10 
#endif

// Estado da UI: informações sobre as contas e o usuário logado
static Conta contas[MAX_CONTAS_DEFINIDO_EM_STORAGE_OU_MAIN];
static int totalContasCarregadas = 0;
static char idUsuarioLogado[20] = {0}; 
static int indiceUsuarioLogado = -1;   



/**
 * Função principal para exibir o menu do ATM e processar as opções do usuário.
 */
void exibir_menu_principal() {
    int opcao;

    // Tentativa de carregar os dados das contas uma única vez.
    if (totalContasCarregadas == 0) {
        totalContasCarregadas = carregar_contas(contas, MAX_CONTAS_DEFINIDO_EM_STORAGE_OU_MAIN);
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
        // printf("%d contas carregadas com sucesso.\n", totalContasCarregadas);
        // system("sleep 1"); // Pequena pausa
    }

    do {
        system("clear || cls");

        printf("Bem-vindo ao C-ATM!\n");
        printf("=====================\n");
        if (indiceUsuarioLogado != -1) {
            // Garante que o índice é válido antes de acessar contas[indiceUsuarioLogado]
            if (indiceUsuarioLogado >= 0 && indiceUsuarioLogado < totalContasCarregadas) {
                 printf("Usuario Logado: %s (ID: %s)\n", contas[indiceUsuarioLogado].nome, contas[indiceUsuarioLogado].id);
            } else {
                // Isso indicaria um estado inconsistente, melhor deslogar.
                printf("Erro: Estado de login inconsistente. Fazendo logout.\n");
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
            while (getchar() != '\n'); // Limpa buffer de entrada em caso de input inválido
            opcao = -1; // Força a entrada no 'default' do switch
        }

        switch (opcao) {
            case 1: // Login
                if (indiceUsuarioLogado != -1) {
                    printf("Voce ja esta logado. Faca logout primeiro.\n");
                } else {
                    // A função iniciarAutenticacao deve retornar o índice da conta ou -1
                    // e preencher idUsuarioLogado em caso de sucesso.
                    indiceUsuarioLogado = iniciarAutenticacao(contas, totalContasCarregadas, idUsuarioLogado);
                }
                break;

            case 2: // Ver Saldo
                if (indiceUsuarioLogado != -1) {
                    exibirInformacoesConta(&contas[indiceUsuarioLogado]);
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 3: // Sacar
                if (indiceUsuarioLogado != -1) {
                    double valor_saque;
                    printf("Digite o valor para sacar: R$ ");
                    if (scanf("%lf", &valor_saque) == 1) {
                        sacar(&contas[indiceUsuarioLogado], valor_saque);
                        // A função sacar deve internamente atualizar o saldo, registrar
                        // a transação e persistir os dados (chamar salvar_contas).
                    } else {
                        printf("Valor invalido!\n");
                        while (getchar() != '\n');
                    }
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 4: // Depositar
                if (indiceUsuarioLogado != -1) {
                    double valor_deposito;
                    printf("Digite o valor para depositar: R$ ");
                     if (scanf("%lf", &valor_deposito) == 1) {
                        depositar(&contas[indiceUsuarioLogado], valor_deposito);
                        // A função depositar deve internamente atualizar o saldo, registrar
                        // a transação e persistir os dados.
                    } else {
                        printf("Valor invalido!\n");
                        while (getchar() != '\n');
                    }
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 5: // Ver Extrato
                if (indiceUsuarioLogado != -1) {
                    // TODO: Implementar chamada para função de exibir extrato
                    // Ex: exibir_extrato_usuario(idUsuarioLogado);
                    printf("Funcionalidade de extrato ainda nao implementada.\n");
                } else {
                    printf("Voce precisa fazer login primeiro!\n");
                }
                break;

            case 6: // Logout
                if (indiceUsuarioLogado != -1) {
                    // Garante que o índice é válido antes de tentar acessar o nome
                    if (indiceUsuarioLogado >= 0 && indiceUsuarioLogado < totalContasCarregadas) {
                        printf("Logout realizado com sucesso, %s.\n", contas[indiceUsuarioLogado].nome);
                    } else {
                        printf("Logout realizado.\n"); // Caso genérico se o nome não puder ser acessado
                    }
                    indiceUsuarioLogado = -1;
                    idUsuarioLogado[0] = '\0'; // Limpa o ID
                } else {
                    printf("Nenhum usuario logado para fazer logout.\n");
                }
                break;

            case 7: // Sair do ATM
                printf("Obrigado por usar o C-ATM. Saindo...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        if (opcao != 7) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n'); // Limpa buffer residual do scanf
            getchar();                 // Espera o Enter do usuário
        }

    } while (opcao != 7);
}