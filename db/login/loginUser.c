#include "loginUser.h"
#include "../conexao//conexao.h"



Usuario buscar_usuario(const char *email, const char *password) {
    Usuario user;
    user.id = -1;
    user.username[0] = '\0';

    if (conn == NULL || PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Banco não conectado\n");
        return user;
    }

    const char *params[2] = { email, password };

    PGresult *res = PQexecParams(conn,"SELECT id, username FROM usuario WHERE email = $1 AND password = $2",2,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Erro na query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return user;
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        return user;
    }

    user.id = atoi(PQgetvalue(res, 0, 0));
    strncpy(user.username, PQgetvalue(res, 0, 1), sizeof(user.username) - 1);
    user.username[sizeof(user.username) - 1] = '\0';

    PQclear(res);
    return user;
}