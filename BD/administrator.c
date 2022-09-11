#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static void create_user(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char options[3] = {'1','2', '3'};
	char r;

	// Input for the registration routine
	char username[46];
	char password[46];
	char ruolo[46];

	// Get the required information
	printf("\nUsername: ");
	getInput(46, username, false);
	printf("password: ");
	getInput(46, password, true);
	printf("Assign a possible role:\n");
	printf("\t1) Administrator\n");
	printf("\t2) Driver\n");
	printf("\t3) Standard user\n");
	r = multiChoice("Select role", options, 3);

	// Convert role into enum value
	switch(r) {
		case '1':
			strcpy(ruolo, "amministratore");
			break;
		case '2':
			strcpy(ruolo, "conducente");
			break;
		case '3':
			strcpy(ruolo, "utente");
			break;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
	}

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call crea_utente(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = ruolo;
	param[2].buffer_length = strlen(ruolo);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("User correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

void add_driver(MYSQL *conn){}
void add_vehicle(MYSQL *conn){}
void assign_shift(MYSQL *conn){}
void add_line(MYSQL *conn){}
void issue_subscription(MYSQL *conn){}
void issue_ticket(MYSQL *conn){}
void put_on_sick_leave(MYSQL *conn){}
void assign_timetable(MYSQL *conn){}
void add_manteinance(MYSQL *conn){}


void run_as_administrator(MYSQL* conn) {
	char options[11] = {'0','1','2', '3', '4', '5','6','7','8','9','Q'};
	char op;
	
	printf("Connected as administrator\n\n");
	
	if(!parse_config("users/amministratore.json", &conf)) {
		fprintf(stderr, "Unable to load administrator configuration\n");
		exit(EXIT_FAILURE);
	}
	
	mysql_close(conn);
	
	conn = mysql_init(NULL);
	if (mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}
	
	while(true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("0) Create new user\n");
		printf("1) Add new driver\n");
		printf("2) Add new vehicle\n");
		printf("3) Assign shift\n");
		printf("4) Add new line\n");
		printf("5) Issue a subscription\n");
		printf("6) Issue a ticket\n");
		printf("7) Put a driver on sick leave\n");
		printf("8) Assign timetable of departures\n");
		printf("9) Add a manteinance work\n");
		printf("Q)Quit\n");
		

		op = multiChoice("Select an option", options, 11);

		switch(op) {
			case '0':
				create_user(conn);
				break;
			case '1':
				add_driver(conn);
				break;
			case '2':
				add_vehicle(conn);
				break;
			case '3':
				assign_shift(conn);
				break;
			case '4':
				add_line(conn);
				break;
			case '5':
				issue_subscription(conn);
				break;
			case '6':
				issue_ticket(conn);
				break;
			case '7':
				put_on_sick_leave(conn);
				break;
			case '8':
				assign_timetable(conn);
				break;	
			case '9':
				add_manteinance(conn);
				break;									
			case 'Q':
				return;
				
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
