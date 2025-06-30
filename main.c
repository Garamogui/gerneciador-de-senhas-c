// Bibliotecas do projto
#include "crud/crud.h"
#include "cryptography/cryptography.h"


// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  const int key = 123;

  if (access("passwords.enc", F_OK) == 0) {
    decrypt_file("passwords.enc", "passwords.txt", key);
  }

  int opt;
  do {
    system("clear");
    printf("----------------- Gerenciador de Senhas -----------------\n");
    printf("1.Obter \n 2.Criar \n 3.Editar \n 4.Excluir \n 5.Sair: ");
    scanf("%d", &opt);
    while (getchar() != '\n');

    switch (opt) {
    case 1:
      get_password();
      break;
    case 2:
      create_password();
      break;
    case 3:
      edit_password();
      break;
    case 4:
      delete_password();
      break;
    }
  } while (opt != 5);

  encrypt_file("passwords.txt", "passwords.enc", key);
  remove("passwords.txt");
  return 0;
}
