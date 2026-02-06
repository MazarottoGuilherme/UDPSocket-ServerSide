#ifndef LOGIN_USER_H
#define LOGIN_USER_H


#include "../conexao/conexao.h"
#include <string.h>
#include <stdlib.h>
#include "../../net/net.h"



typedef struct {
    int id;
    char email[256];
    char password[100];
} Usuario;

Usuario buscar_usuario(const char *email, const char *password);

#endif // LOGIN_USER_H

// gcc *.c -o server $(pkg-config --cflags --libs libpq)