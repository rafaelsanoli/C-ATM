#include <stdio.h>
#include <string.h>
#include <openssl/sha.h> // biblioteca pra hash
#include "../../include/atm/auth.h"

#define TENTATIVAS_MAX 3

void gerarHash(const char* senhaOriginal, char* senhaHashDestino) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)senhaOriginal, strlen(senhaOriginal), hash);

    // Converte hash para string hexadecimal
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(senhaHashDestino + (i * 2), "%02x", hash[i]);
    }
    senhaHashDestino[64] = '\0'; // Finaliza a string
}

int autenticarUsuario(const char* id, const char* senhaDigitada, Conta contas[], int tamanho) {
    char senhaHash[65];
    gerarHash(senhaDigitada, senhaHash); // Gera o hash da senha digitada

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(contas[i].id, id) == 0 && strcmp(contas[i].senha_hash, senhaHash) == 0) {
            return 1; // Login OK
        }
    }
    return 0; // Login inválido
}

int iniciarAutenticacao(Conta contas[], int tamanho, char* idAutenticado) {
    int tentativas = 0;
    char id[20];
    char senha[20];

    while (tentativas < TENTATIVAS_MAX) {
        printf("Digite seu ID: ");
        scanf("%s", id);

        printf("Digite seu PIN: ");
        scanf("%s", senha);

        if (autenticarUsuario(id, senha, contas, tamanho)) {
            printf("Login realizado com sucesso!\n");
            strcpy(idAutenticado, id); // Salva o ID do usuário autenticado
            return 1;
        } else {
            tentativas++;
            if (tentativas < TENTATIVAS_MAX) {
                printf("PIN incorreto. Você tem mais %d tentativa(s).\n", TENTATIVAS_MAX - tentativas);
            } else {
                printf("Número máximo de tentativas atingido. Conta bloqueada temporariamente.\n");
            }
        }
    }
    return 0;
}

// NOVA FUNÇÃO PARA LOCALIZAR A CONTA PELO ID COM SEGURANÇA
int encontrarIndiceConta(const char* id, Conta contas[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(contas[i].id, id) == 0) {
            return i;
        }
    }
    return -1; // Não encontrada
}

//Laylla