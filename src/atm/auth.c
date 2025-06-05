#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include "../../include/atm/auth.h"

#define TENTATIVAS_MAX 3

void gerarHashAuth(const char* senhaOriginal, char* senhaHashDestino) {
    unsigned char hashBinario[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;

    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, (const unsigned char*)senhaOriginal, strlen(senhaOriginal));
    SHA256_Final(hashBinario, &sha256_ctx);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(senhaHashDestino + (i * 2), "%02x", hashBinario[i]);
    }
    senhaHashDestino[64] = '\0';
}

int autenticarUsuario(const char* id, const char* pinDigitado, Conta contas[], int totalContas) {
    char pinHashCalculado[65];

    if (id == NULL || pinDigitado == NULL || contas == NULL || totalContas <= 0) {
        return -1;
    }

    gerarHashAuth(pinDigitado, pinHashCalculado);

    for (int i = 0; i < totalContas; i++) {
        if (strcmp(contas[i].id, id) == 0) {
            if (strcmp(contas[i].senha_hash, pinHashCalculado) == 0) {
                return i; 
            } else {
                return -1; 
            }
        }
    }
    return -1;
}

int iniciarAutenticacao(Conta contas[], int totalContas, char* idAutenticadoBuffer) {
    char idInput[20];
    char pinInput[20];
    int tentativas = 0;
    int indiceContaAutenticada = -1;

    if (contas == NULL || totalContas <= 0 || idAutenticadoBuffer == NULL) {
        fprintf(stderr, "Erro: Parametros invalidos para iniciarAutenticacao.\n");
        return -1;
    }

    while (tentativas < TENTATIVAS_MAX) {
        printf("Digite seu ID: ");
        if (scanf("%19s", idInput) != 1) {
             while (getchar() != '\n' && getchar() != EOF);
             printf("Entrada de ID invalida.\n");
             continue;
        }

        printf("Digite seu PIN: ");
        if (scanf("%19s", pinInput) != 1) {
            while (getchar() != '\n' && getchar() != EOF);
            printf("Entrada de PIN invalida.\n");
            continue;
        }

        indiceContaAutenticada = autenticarUsuario(idInput, pinInput, contas, totalContas);

        if (indiceContaAutenticada != -1) {
            printf("Login realizado com sucesso!\n");
            strncpy(idAutenticadoBuffer, contas[indiceContaAutenticada].id, 19);
            idAutenticadoBuffer[19] = '\0';
            return indiceContaAutenticada;
        } else {
            tentativas++;
            printf("ID ou PIN incorreto.\n");
            if (tentativas < TENTATIVAS_MAX) {
                printf("Voce tem mais %d tentativa(s).\n", TENTATIVAS_MAX - tentativas);
            } else {
                printf("Numero maximo de tentativas atingido.\n");
            }
        }
    }

    idAutenticadoBuffer[0] = '\0';
    return -1;
}

