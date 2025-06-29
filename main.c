#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void encrypt(int key);
void decrypt(int key);
void get_password();
void create_password();
char *generate_password();
void edit_password();
void delete_password();

typedef struct {
  char service[50];
  char username[50];
  char password[50];
} Password;

int main(void) {
  int key = 123;
  decrypt(key);

  int menu_choice = 0;
  system("clear");
  printf("-------- Bem vindo ao seu Gerenciador de Senhas --------\n");

  do {
    printf("\nMenu:\n");
    printf("1. Obter Senhas\n");
    printf("2. Criar Senha\n");
    printf("3. Editar Senha\n");
    printf("4. Excluir Senha\n");
    printf("5. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &menu_choice);

    switch (menu_choice) {
      case 1: get_password();    break;
      case 2: create_password(); break;
      case 3: edit_password();   break;
      case 4: delete_password(); break;
      case 5:
        printf("Saindo do programa. Até logo!\n");
        encrypt(key);
        break;
      default:
        printf("\nOpção inválida. Tente novamente.\n");
    }
  } while (menu_choice != 5);

  return 0;
}

void encrypt(int key) {
  char read_buffer[155];
  char write_buffer[155];

  FILE *in  = fopen("passwords.txt", "r");
  if (!in) {
    printf("Erro: Arquivo de senhas não encontrado!\n");
    return;
  }
  FILE *out = fopen("temp.txt", "w");
  if (!out) {
    printf("Erro ao criar arquivo temporário!\n");
    fclose(in);
    return;
  }

  while (fgets(read_buffer, sizeof(read_buffer), in)) {
    size_t len = strlen(read_buffer);
    for (size_t i = 0; i < len; i++) {
      write_buffer[i] = read_buffer[i] + key;
    }
    write_buffer[len] = '\0';
    fputs(write_buffer, out);
  }

  fclose(in);
  fclose(out);

  remove("passwords.txt");
  rename("temp.txt", "passwords.txt");
  printf("Arquivo criptografado com sucesso!\n");
}

void decrypt(int key) {
  char read_buffer[155];
  char write_buffer[155];

  FILE *in  = fopen("passwords.txt", "r");
  if (!in) return;  // nada a descriptografar
  FILE *out = fopen("temp.txt", "w");
  if (!out) { fclose(in); return; }

  while (fgets(read_buffer, sizeof(read_buffer), in)) {
    size_t len = strlen(read_buffer);
    for (size_t i = 0; i < len; i++) {
      write_buffer[i] = read_buffer[i] - key;
    }
    write_buffer[len] = '\0';
    fputs(write_buffer, out);
  }

  fclose(in);
  fclose(out);
  remove("passwords.txt");
  rename("temp.txt", "passwords.txt");
}

void get_password() {
  FILE *f = fopen("passwords.txt", "r");
  if (!f) {
    printf("Erro: Arquivo de senhas não encontrado!\n");
    return;
  }

  Password p;
  system("clear");
  printf(" ------------------- SENHAS SALVAS -------------------\n");

  while (fscanf(f, "%49[^|]|%49[^|]|%49[^\n]\n",
                p.service, p.username, p.password) == 3) {
    printf("Serviço: %s\n", p.service);
    printf("Usuário: %s\n", p.username);
    printf("Senha: %s\n", p.password);
    printf("--------------------------------\n");
  }
  fclose(f);
}

void create_password() {
  int choice = 0, c;
  Password new_password;
  // descarta o '\n' pendente
  while ((c = getchar()) != '\n' && c != EOF);

  system("clear");
  printf(" ------------------- CRIAR NOVA SENHA -------------------\n");

  printf("Digite o nome do serviço: ");
  scanf("%49s", new_password.service);
  printf("Digite o nome de usuário: ");
  scanf("%49s", new_password.username);

  do {
    printf("\n ---- Senha ----\n");
    printf("1. Inserir sua própria senha?\n");
    printf("2. Criar senha aleatória?\n");
    printf("Escolha: ");
    scanf("%d", &choice);
    while ((c = getchar()) != '\n' && c != EOF);

    if (choice == 1) {
      printf("Digite sua senha: ");
      fgets(new_password.password, sizeof(new_password.password), stdin);
      new_password.password[strcspn(new_password.password, "\n")] = '\0';
    }
    else if (choice == 2) {
      strcpy(new_password.password, generate_password());
      printf("Senha gerada: %s\n", new_password.password);
    }
    else {
      printf("Opção inválida. Por favor, escolha 1 ou 2.\n");
    }
  } while (choice != 1 && choice != 2);

  FILE *f = fopen("passwords.txt", "a");
  if (!f) { printf("Erro ao abrir o arquivo!\n"); return; }
  fprintf(f, "%s|%s|%s\n",
          new_password.service,
          new_password.username,
          new_password.password);
  fclose(f);

  printf("\nSenha salva com sucesso!\n");
}

char *generate_password() {
  static char password[50];
  int length;
  const char *base_chars =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789!<>:@#$%^&*()_+";

  printf("\nDigite o tamanho da senha (1-49): ");
  scanf("%d", &length);
  if (length < 1) length = 1;
  if (length >= 50) length = 49;

  srand((unsigned)time(NULL));
  for (int i = 0; i < length; i++) {
    int idx = rand() % strlen(base_chars);
    password[i] = base_chars[idx];
  }
  password[length] = '\0';
  return password;
}

void edit_password() {
  system("clear");
  FILE *f = fopen("passwords.txt", "r");
  if (!f) { printf("Nenhum arquivo para edição.\n"); return; }

  Password lista[100];
  int count = 0;
  while (fscanf(f, "%49[^|]|%49[^|]|%49[^\n]\n",
                lista[count].service,
                lista[count].username,
                lista[count].password) == 3) {
    count++;
  }
  fclose(f);

  if (count == 0) {
    printf("Não há senhas salvas.\n");
    return;
  }

  printf("\n--- Lista de Senhas ---\n\n");
  printf("0. Voltar ao menu\n");
  for (int i = 0; i < count; i++) {
    printf("%d. %s | %s | %s\n",
           i+1,
           lista[i].service,
           lista[i].username,
           lista[i].password);
  }

  int escolha, campo, c;
  printf("\nNúmero da senha para editar: ");
  scanf("%d", &escolha);
  if (escolha < 0 || escolha > count) return;
  if (escolha == 0) return;

  printf("\n1.Serviço  2.Usuário  3.Senha\nEscolha campo: ");
  scanf("%d", &campo);
  while ((c = getchar()) != '\n' && c != EOF);

  char *target;
  switch (campo) {
    case 1: target = lista[escolha-1].service;  break;
    case 2: target = lista[escolha-1].username; break;
    case 3: target = lista[escolha-1].password; break;
    default: return;
  }
  printf("Novo valor: ");
  fgets(target, 50, stdin);
  target[strcspn(target, "\n")] = '\0';

  FILE *out = fopen("temp.txt", "w");
  if (!out) return;
  for (int i = 0; i < count; i++) {
    fprintf(out, "%s|%s|%s\n",
            lista[i].service,
            lista[i].username,
            lista[i].password);
  }
  fclose(out);
  remove("passwords.txt");
  rename("temp.txt", "passwords.txt");

  printf("Senha editada com sucesso!\n");
}

void delete_password() {
  system("clear");
  FILE *f = fopen("passwords.txt", "r");
  if (!f) { printf("Nenhum arquivo para deletar.\n"); return; }

  Password lista[100];
  int count = 0;
  while (fscanf(f, "%49[^|]|%49[^|]|%49[^\n]\n",
                lista[count].service,
                lista[count].username,
                lista[count].password) == 3) {
    count++;
  }
  fclose(f);
  if (count == 0) { printf("Nada a excluir.\n"); return; }

  printf("\n--- Lista de Senhas ---\n\n");
  printf("0. Voltar ao menu\n");
  for (int i = 0; i < count; i++) {
    printf("%d. %s | %s | %s\n",
           i+1,
           lista[i].service,
           lista[i].username,
           lista[i].password);
  }

  int escolha;
  printf("\nNúmero da senha para excluir: ");
  scanf("%d", &escolha);
  if (escolha < 0 || escolha > count) return;
  if (escolha == 0) return;

  FILE *out = fopen("temp.txt", "w");
  if (!out) return;
  for (int i = 0; i < count; i++) {
    if (i != escolha-1) {
      fprintf(out, "%s|%s|%s\n",
              lista[i].service,
              lista[i].username,
              lista[i].password);
    }
  }
  fclose(out);
  remove("passwords.txt");
  rename("temp.txt", "passwords.txt");

  printf("Senha excluída com sucesso!\n");
}
