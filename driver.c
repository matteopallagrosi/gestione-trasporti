#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

void show_shifts(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call lista_turni_conducente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = conf.username;
	param[0].buffer_length = strlen(conf.username);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for show shifts\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while executing procedure.");
	} else {
		get_result_set(conn, prepared_stmt, "\nYour shifts:\n", &param);
	}

	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);
}

void next_departure(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	short int vehicle_id;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call prossima_partenza_conducente(?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conf.username;
	param[0].buffer_length = strlen(conf.username);
	
	param[1].buffer_type = MYSQL_TYPE_SHORT;
	param[1].buffer = &vehicle_id;
	param[1].buffer_length = sizeof(vehicle_id);
	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for show shifts\n", true);
	}
	
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		int status;
		int num_rows;
		int first = 1;
		do {
			get_result_set(conn, prepared_stmt, "\nNext departure details: ", &param);
			if (first) {
				num_rows = mysql_stmt_num_rows(prepared_stmt); //non può ritornare errori
			}
			status = mysql_stmt_next_result(prepared_stmt);
			first = 0;
			if (status > 0)
				finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		} while (status == 0);
		
		//show the current vehicle kilometers (if available)
		if (num_rows != 0)
			printf("Vehicle for next departure: %04d\n", *(short int *)param[0].buffer);
		else 
			printf("\nThere are no scheduled departures\n");
	}
	
	free(param);
	mysql_stmt_close(prepared_stmt);	
}


void run_as_driver(MYSQL* conn) {
	char options[3] = {'0','1','Q'};
	char op;
	
	printf("Connected as driver\n\n");
	
	if(!parse_config("users/conducente.json", &conf)) {
		fprintf(stderr, "Unable to load driver configuration\n");
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
		printf("0) Show sifts\n");
		printf("1) Next departure\n");
		printf("Q)Quit\n");
		

		op = multiChoice("Select an option", options, 11);

		switch(op) {
			case '0':
				show_shifts(conn);
				break;
			case '1':
				next_departure(conn);
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
