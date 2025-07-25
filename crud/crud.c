#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crud.h"

void pause_enter(void) {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void get_password(void) {
    FILE *f = fopen("passwords.txt", "r");
    system("clear");
    printf("----- SENHAS SALVAS -----\n\n");
    if (!f) {
        printf("Nenhum registro encontrado.\n");
        pause_enter();
        return;
    }
    Password p;
    int found = 0;
    while (fscanf(f, "%49[^|]|%49[^|]|%49[^\n]\n",
                  p.service, p.username, p.password) == 3) {
        printf("Serviço: %s\nUsuário: %s\nSenha:   %s\n", 
               p.service, p.username, p.password);
        printf("-------------------------\n");
        found = 1;
    }
    fclose(f);
    if (!found) {
        printf("Nenhum registro encontrado.\n");
    }
    pause_enter();
}

char *generate_password(void) {
    static char pwd[50];
    const char *chars =
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "0123456789!<>:@#$%^&*()_+";
    int length;
    printf("\nTamanho da senha (1–49): ");
    scanf("%d", &length);
    if (length < 1) length = 1;
    if (length > 49) length = 49;
    srand((unsigned)time(NULL));
    for (int i = 0; i < length; i++) {
        pwd[i] = chars[rand() % strlen(chars)];
    }
    pwd[length] = '\0';
    while (getchar() != '\n'); // limpar \n pendente
    return pwd;
}

void create_password(void) {
    int choice;
    Password np;
    while (getchar() != '\n');
    system("clear");
    printf("---- CRIAR NOVA ENTRADA ----\n\n");

    printf("Serviço: ");
    fgets(np.service, sizeof np.service, stdin);
    np.service[strcspn(np.service, "\n")] = '\0';

    printf("Usuário: ");
    fgets(np.username, sizeof np.username, stdin);
    np.username[strcspn(np.username, "\n")] = '\0';

    do {
        printf("\n1. Definir senha manual\n2. Gerar senha aleatória\nEscolha: ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        if (choice == 1) {
            printf("Senha: ");
            fgets(np.password, sizeof np.password, stdin);
            np.password[strcspn(np.password, "\n")] = '\0';
        } else if (choice == 2) {
            strcpy(np.password, generate_password());
            printf("Senha gerada: %s\n", np.password);
        } else {
            printf("Opção inválida.\n");
        }
    } while (choice != 1 && choice != 2);

    FILE *f = fopen("passwords.txt", "a");
    if (f) {
        fprintf(f, "%s|%s|%s\n",
                np.service, np.username, np.password);
        fclose(f);
    }
    printf("\n[+] Nova entrada salva!\n");
    pause_enter();
}

void edit_password(void) {
    Password lst[100];
    int n = 0, rec, fld;
    FILE *f = fopen("passwords.txt", "r");
    system("clear");
    printf("---- EDITAR ENTRADA ----\n\n");
    if (!f) {
        printf("Nenhum registro para editar.\n");
        pause_enter();
        return;
    }
    while (fscanf(f, "%49[^|]|%49[^|]|%49[^\n]\n",
                  lst[n].service, lst[n].username, lst[n].password) == 3) {
        n++;
    }
    fclose(f);
    if (n == 0) {
        printf("Nenhum registro para editar.\n");
        pause_enter();
        return;
    }
    for (int i = 0; i < n; i++) {
        printf("%d. %s | %s | %s\n",
               i+1, lst[i].service, lst[i].username, lst[i].password);
    }
    printf("\nRegistro (0 = cancelar): ");
    scanf("%d", &rec);
    if (rec < 1 || rec > n) return;
    printf("1.Serviço 2.Usuário 3.Senha\nCampo: ");
    scanf("%d", &fld);
    while (getchar() != '\n');
    char *target = (fld==1 ? lst[rec-1].service :
                     fld==2 ? lst[rec-1].username :
                              lst[rec-1].password);
    printf("Novo valor: ");
    fgets(target, 50, stdin);
    target[strcspn(target, "\n")] = '\0';
    f = fopen("temp.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%s|%s|%s\n",
                lst[i].service, lst[i].username, lst[i].password);
    }
    fclose(f);
    rename("temp.txt", "passwords.txt");
    printf("\n[+] Registro atualizado!\n");
    pause_enter();
}

void delete_password(void) {
    Password lst[100];
    int n = 0, rec;
    FILE *f = fopen("passwords.txt", "r");
    system("clear");
    printf("---- EXCLUIR ENTRADA ----\n\n");
    if (!f) {
        printf("Nenhum registro para excluir.\n");
        pause_enter();
        return;
    }
    while (fscanf(f, "%49[^|]|%49[^|]|%49[^\n]\n",
                  lst[n].service, lst[n].username, lst[n].password) == 3) {
        n++;
    }
    fclose(f);
    if (n == 0) {
        printf("Nenhum registro para excluir.\n");
        pause_enter();
        return;
    }
    for (int i = 0; i < n; i++) {
        printf("%d. %s | %s | %s\n",
               i+1, lst[i].service, lst[i].username, lst[i].password);
    }
    printf("\nRegistro (0 = cancelar): ");
    scanf("%d", &rec);
    if (rec < 1 || rec > n) return;
    f = fopen("temp.txt", "w");
    for (int i = 0; i < n; i++) {
        if (i != rec-1) {
            fprintf(f, "%s|%s|%s\n",
                    lst[i].service, lst[i].username, lst[i].password);
        }
    }
    fclose(f);
    rename("temp.txt", "passwords.txt");
    printf("\n[+] Registro excluído!\n");
    pause_enter();
}
