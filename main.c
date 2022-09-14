#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "defines.h"

typedef enum {
	ADMINISTRATOR = 1,
	DRIVER,
	USER,
	FAILED_LOGIN
} role_t;

struct configuration conf;

static MYSQL *conn;

static role_t attempt_login(MYSQL *conn, char *username, char *password) {
	MYSQL_STMT *login_procedure;
	
	int role = 0;
	
	if(!setup_prepared_stmt(&login_procedure, "call login(?, ?, ?)", conn)) {
		print_stmt_error(login_procedure, "Unable to initialize login statement\n");
		goto err2;
	}
	
	//binding dei parametri
	MYSQL_BIND *param;
	setup_binding(conn, login_procedure, &param);
	
	param[0].buffer_type = MYSQL_TYPE_STRING; // IN
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_STRING; // IN
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	param[2].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[2].buffer = &role;
	param[2].buffer_length = sizeof(role);
	
	if (mysql_stmt_bind_param(login_procedure, param) != 0) { // Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		goto err;
	}
	
	//esegue la procedura
	if (mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		goto err;
	}
	
	//recupera il result set di output
	get_result_set(conn, login_procedure, "", &param);
	
	/*MYSQL_BIND new_param[1];
	
	//recupera l'output della procedura
	memset(new_param, 0, sizeof(new_param));
	new_param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	new_param[0].buffer = &role;
	new_param[0].buffer_length = sizeof(role);
	
	
	if(mysql_stmt_bind_result(login_procedure, new_param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		goto err;
	}
	
	if(mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		goto err;
	} */
	
	role = *(int*)(param[0].buffer);
	free(param);
	
	mysql_stmt_close(login_procedure);
	return role;
	
	err:
	mysql_stmt_close(login_procedure);
    err2:
	return FAILED_LOGIN;
}


int main(void) {
	role_t role;
	
	if(!parse_config("users/login.json", &conf)) {  //inizializza la struct recuperando le informazioni dal file JSON
		fprintf(stderr, "Unable to load login configuration\n");
		exit(EXIT_FAILURE);
	}
	
	conn = mysql_init (NULL);
	if (conn == NULL) {
		fprintf (stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}
	
	//si connette al db come utente login (che può solo eseguire la procedure 'login')
	if (mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf (stderr, "mysql_real_connect() failed\n");
		mysql_close (conn);
		exit(EXIT_FAILURE);
	}
	
	printf("Welcome to your transport management platform!\n\n");
	printf("Insert your credentials\n");
	
	//recupera username e password dell'utente che vuole loggarsi
	printf("Username: ");
	getInput(128, conf.username, false);
	printf("Password: ");
	getInput(128, conf.password, true);
	
	//esegue il login
	role = attempt_login(conn, conf.username, conf.password);
	
	switch(role) {
		case USER:
			run_as_user(conn);
			break;
			
		case DRIVER:
			run_as_driver(conn);
			break;
			
		case ADMINISTRATOR:
			run_as_administrator(conn);
			break;
			
		case FAILED_LOGIN:
			fprintf(stderr, "Invalid credentials\n");
			exit(EXIT_FAILURE);
			break;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
	}

	printf("Bye!\n");

	mysql_close (conn);
	return 0;
}
