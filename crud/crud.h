#ifndef CRUD_H
#define CRUD_H

typedef struct {
    char service[50];
    char username[50];
    char password[50];
} Password;

void get_password(void);
void create_password(void);
void edit_password(void);
void delete_password(void);

#endif
