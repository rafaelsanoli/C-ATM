#include <stdio.h>
#include "../include/atm/ui.h" // Inclui o cabeçalho da UI para ter acesso a exibir_menu_principal()
                              // Certifique-se que este caminho está correto em relação à pasta src/
                              // e que ui.h declara exibir_menu_principal()

int main() {
    // A função exibir_menu_principal agora é responsável por todo o fluxo do ATM
    exibir_menu_principal();

    printf("\nSimulador C-ATM encerrado. Obrigado!\n");

    return 0;
}