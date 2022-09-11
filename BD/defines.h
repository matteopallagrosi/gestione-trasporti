#pragma once

#include <stdbool.h>
#include <mysql.h>

struct configuration {
	char *host;
	char *db_username;
	char *db_password;
	unsigned int port;
	char *database;

	char username[128];
	char password[128];
};

extern struct configuration conf;

extern int parse_config(char *path, struct configuration *conf);
extern char *getInput(unsigned int lung, char *stringa, bool hide);
extern bool yesOrNo(char *domanda, char yes, char no, bool predef, bool insensitive);
extern char multiChoice(char *domanda, char choices[], int num);
extern void print_error (MYSQL *conn, char *message);
extern void print_stmt_error (MYSQL_STMT *stmt, char *message);
extern void finish_with_error(MYSQL *conn, char *message);
extern void finish_with_stmt_error(MYSQL *conn, MYSQL_STMT *stmt, char *message, bool close_stmt);
extern bool setup_prepared_stmt(MYSQL_STMT **stmt, char *statement, MYSQL *conn);
extern void get_result_set(MYSQL *conn, MYSQL_STMT *stmt, char *title, MYSQL_BIND **rs_bind);
extern void run_as_user(MYSQL *conn);
extern void run_as_driver(MYSQL *conn);
extern void run_as_administrator(MYSQL *conn);
extern void setup_binding(MYSQL *conn, MYSQL_STMT *stmt, MYSQL_BIND **bind);
