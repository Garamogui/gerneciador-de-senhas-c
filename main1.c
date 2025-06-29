// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>     // para access()

typedef struct {
    char service[50];
    char username[50];
    char password[50];
} Password;

// protótipos
void encrypt_file(const char *inName, const char *outName, int key);
void decrypt_file(const char *inName, const char *outName, int key);
void get_password();
void create_password();
char *generate_password();
void edit_password();
void delete_password();
void pause_enter();

int main(void) {
    const int key = 123;
    // Se existir versão criptografada, descriptografa para .txt
    if (access("passwords.enc", F_OK) == 0) {
        decrypt_file("passwords.enc", "passwords.txt", key);
    }
    // Garante que o arquivo .txt existe (mesmo vazio) para CRUD
    FILE *fcheck = fopen("passwords.txt", "a");
    if (fcheck) fclose(fcheck);

    int menu_choice;
    do {
        system("clear");
        printf("-------- Gerenciador de Senhas --------\n\n");
        printf("1. Obter Senhas\n");
        printf("2. Criar Senha\n");
        printf("3. Editar Senha\n");
        printf("4. Excluir Senha\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &menu_choice);

        // limpa o '\n' de scanf
        int _c; while ((_c = getchar()) != '\n' && _c != EOF);

        switch (menu_choice) {
            case 1: get_password();    break;
            case 2: create_password(); break;
            case 3: edit_password();   break;
            case 4: delete_password(); break;
            case 5:
                printf("\nSaindo do programa. Até breve!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                pause_enter();
        }
    } while (menu_choice != 5);

    // Ao sair, criptografa TXT em ENC e remove TXT
    encrypt_file("passwords.txt", "passwords.enc", key);
    remove("passwords.txt");
    return 0;
}

// criptografa cada byte de inName → outName
void encrypt_file(const char *inName, const char *outName, int key) {
    FILE *fin = fopen(inName, "r");
    if (!fin) return;  // nada a criptografar
    FILE *fout = fopen(outName, "w");
    if (!fout) { fclose(fin); return; }

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc((ch + key) & 0xFF, fout);
    }
    fclose(fin);
    fclose(fout);
    printf("\n[+] Arquivo criptografado em %s\n", outName);
}

// descriptografa cada byte de inName → outName
void decrypt_file(const char *inName, const char *outName, int key) {
    FILE *fin = fopen(inName, "r");
    if (!fin) return;  // nada a descriptografar
    FILE *fout = fopen(outName, "w");
    if (!fout) { fclose(fin); return; }

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc((ch - key) & 0xFF, fout);
    }
    fclose(fin);
    fclose(fout);
    printf("[+] Arquivo descriptografado em %s\n", outName);
}

// pausa até ENTER
void pause_enter() {
    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n');
}

// 1) Lista todas as senhas
void get_password() {
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
                  p.service, p.username, p.password) == 3)
    {
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

// 2) Cria uma nova entrada
void create_password() {
    int choice;
    Password np;
    system("clear");
    printf("---- CRIAR NOVA ENTRADA ----\n\n");

    printf("Serviço: ");
    fgets(np.service, sizeof np.service, stdin);
    np.service[strcspn(np.service, "\n")] = '\0';

    printf("Usuário: ");
    fgets(np.username, sizeof np.username, stdin);
    np.username[strcspn(np.username, "\n")] = '\0';

    do {
        printf("\n1. Definir senha manual\n");
        printf("2. Gerar senha aleatória\n");
        printf("Escolha: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1) {
            printf("Senha: ");
            fgets(np.password, sizeof np.password, stdin);
            np.password[strcspn(np.password, "\n")] = '\0';
        }
        else if (choice == 2) {
            strcpy(np.password, generate_password());
            printf("Senha gerada: %s\n", np.password);
        }
        else {
            printf("Opção inválida. Tente novamente.\n");
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

// 2.1) Gera senha aleatória
char *generate_password() {
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
    while (getchar() != '\n');
    return pwd;
}

// 3) Edita uma entrada existente
void edit_password() {
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
    printf("1.Serviço  2.Usuário  3.Senha\nCampo: ");
    scanf("%d", &fld);
    while (getchar() != '\n');

    char *target = (fld == 1 ? lst[rec-1].service :
                     fld == 2 ? lst[rec-1].username :
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

// 4) Exclui uma entrada existente
void delete_password() {
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
