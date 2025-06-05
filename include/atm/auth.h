// ../../include/atm/auth.h

#ifndef AUTH_H
#define AUTH_H

#include "storage.h" 

void gerarHashAuth(const char* senhaOriginal, char* senhaHashDestino);

int autenticarUsuario(const char* id, const char* pinDigitado, Conta contas[], int totalContas);

int iniciarAutenticacao(Conta contas[], int totalContas, char* idAutenticadoBuffer);

#endif // AUTH_H

