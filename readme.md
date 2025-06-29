# Password Manager (C)

Um pequeno **gerenciador de senhas em C**, feito para terminal.  
Permite salvar, consultar, editar e excluir senhas, mantendo o arquivo criptografado em disco para maior segurança.

---

## 📋 Funcionalidades

✅ **Listar senhas** salvas  
✅ **Criar novas senhas** manualmente ou geradas aleatoriamente  
✅ **Editar senhas existentes** (serviço, usuário ou senha)  
✅ **Excluir registros** salvos  
✅ **Criptografia simples** do arquivo de senhas ao encerrar o programa

---

## 🔐 Como funciona a criptografia?

- Ao iniciar o programa:
  - Se existir o arquivo `passwords.enc`, ele é **descriptografado** para `passwords.txt`.
- Durante o uso:
  - Você manipula apenas o arquivo `passwords.txt` (em texto puro).
- Ao encerrar:
  - O arquivo `passwords.txt` é **criptografado** novamente para `passwords.enc`.
  - O `passwords.txt` é apagado.

A criptografia usada é bem simples (apenas soma/subtração de bytes com uma chave). **Não use este código para proteger dados sensíveis em produção.** Foi feito para fins didáticos!

---

## 🛠️ Instalação e execução

### 1. Clone o repositório

```bash
git clone https://github.com/seu-usuario/password-manager.git
cd password-manager
````

### 2. Compile o projeto

Se estiver usando Linux ou MacOS:

```bash
gcc -o password-manager \
    main.c \
    criptography/criptography.c \
    crud/crud.c
```

Se estiver no Windows (MinGW):

```bash
gcc -o password-manager.exe \
    main.c \
    criptography/criptography.c \
    crud/crud.c
```

---

## ▶️ Como usar

Rode o programa:

```bash
./password-manager
```
ou, se preferir: 
```bash
chmod +x password-manager;
password-manager
```

Você verá o menu:

```
-------- Gerenciador de Senhas --------

1. Obter Senhas
2. Criar Senha
3. Editar Senha
4. Excluir Senha
5. Sair
Escolha uma opção:
```

* Escolha as opções desejadas.
* Ao sair, o arquivo será salvo de forma criptografada.

---

## 📦 Estrutura do projeto

```
password-manager/
├── main.c
├── criptography/
│   ├── criptography.h
│   └── criptography.c
├── crud/
│   ├── crud.h
│   └── crud.c
└── README.md
```

---

## ⚠️ Aviso

> Este projeto é **didático**.
> Não é seguro para uso em produção nem para armazenar senhas reais.
