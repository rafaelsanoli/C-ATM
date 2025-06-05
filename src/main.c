#include <stdio.h>
#include <string.h>
#include <atm/auth.h>
#include <atm/account.h>
#include <atm/storage.h>

#define MAX_CONTAS 10


#include <stdio.h> 
#include "atm/ui.h"   
                      

int main() {
    // Mensagem inicial opcional, ou pode ir direto para a UI.
    printf("Iniciando o Simulador C-ATM...\n");

    // Chama a função principal da interface do usuário.
    // Todo o loop de menu e interações acontecerão dentro desta função.
    exibir_menu_principal();

    printf("Encerrando o Simulador C-ATM. Ate logo!\n");

    return 0;
}