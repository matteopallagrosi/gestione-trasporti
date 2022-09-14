#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

void validate_ticket(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	
	if(!setup_prepared_stmt(&prepared_stmt, "call mostra_biglietti_validi(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conf.username;
	param[0].buffer_length = strlen(conf.username);
	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for show shifts\n", true);
	}
	
	int num_rows;
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while executing procedure.");
	} else {
		get_result_set(conn, prepared_stmt, "\nChoose a ticket to validate:\n", &param);
		num_rows = mysql_stmt_num_rows(prepared_stmt);
	}
	
	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);
	
	if (num_rows == 0) {
		printf("There are no valid tickets\n");
		return;
	}
	
	//****************************************************************
	

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call valida_biglietto(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char num_ticket[46];
	int ticket;
	char vehicle_id[46];
	int vehicle;
	
	// Get the required information
	printf("\nticket number: ");
	getInput(46, num_ticket, false);
	printf("\nvehicle id: ");
	getInput(46, vehicle_id, false);
	
	//convert values
	ticket = atoi(num_ticket);
	vehicle = atoi(vehicle_id);
	
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conf.username;
	param[0].buffer_length = strlen(conf.username);
	
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &ticket;
	param[1].buffer_length = sizeof(ticket);
	
	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &vehicle;
	param[2].buffer_length = sizeof(vehicle);
	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for show shifts\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while executing procedure.");
	} else {
		printf("Ticket validate correctly\n");
	}
	
	free(param);
	mysql_stmt_close(prepared_stmt);
}

void use_subscription(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call valida_abbonamento(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char vehicle_id[46];
	int vehicle;
	
	// Get the required information
	printf("\nvehicle id: ");
	getInput(46, vehicle_id, false);
	
	//convert values
	vehicle = atoi(vehicle_id);
	
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conf.username;
	param[0].buffer_length = strlen(conf.username);
	
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &vehicle;
	param[1].buffer_length = sizeof(vehicle);
	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for show shifts\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while executing procedure.");
	} else {
		printf("Subscription validate correctly\n");
	}
	
	
	free(param);
	mysql_stmt_close(prepared_stmt);
}




void show_line(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call visualizza_tratta(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char num_line[46];
	int line;
	char direction[46];
	
	// Get the required information
	printf("\nline number: ");
	getInput(46, num_line, false);
	printf("\ndirection: ");
	getInput(46, direction, false);
	
	//convert values
	line = atoi(num_line);
	
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &line;
	param[0].buffer_length = sizeof(line);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = direction;
	param[1].buffer_length = strlen(direction);
	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for show shifts\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while executing procedure.");
	} else {
		get_result_set(conn, prepared_stmt, "\nLine details (from the first to the last stop):\n", &param);
	}
	
	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);
}



void show_oncoming_vehicles(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call veicoli_in_arrivo(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char num_stop[46];
	int stop;

	
	// Get the required information
	printf("\nstop number: ");
	getInput(46, num_stop, false);
	
	//convert values
	stop = atoi(num_stop);
	
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &stop;
	param[0].buffer_length = sizeof(stop);
	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for show shifts\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while executing procedure.");
	} else {
		int status;
		char *title = "\nveicoli in arrivo (con tempi attesa in minuti):";
		do {
			get_result_set(conn, prepared_stmt, title, &param);
			status = mysql_stmt_next_result(prepared_stmt);
			if (status > 0)
				finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
			title = "\nprossime partenze:";
		} while (status == 0);
	}
	
	
	mysql_stmt_close(prepared_stmt);
}




void run_as_user(MYSQL* conn) {
	char options[5] = {'0','1','2','3','Q'};
	char op;
	
	printf("Connected as standard user\n\n");
	
	if(!parse_config("users/utente.json", &conf)) {
		fprintf(stderr, "Unable to load user configuration\n");
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
		printf("0) Validate ticket\n");
		printf("1) Use subscription\n");
		printf("2) Show line details\n");
		printf("3) Show vehicles arriving at the stop\n");
		printf("Q)Quit\n");
		

		op = multiChoice("Select an option", options, 5);

		switch(op) {
			case '0':
				validate_ticket(conn);
				break;
			case '1':
				use_subscription(conn);
				break;	
			case '2':
				show_line(conn);
				break;
			case '3':
				show_oncoming_vehicles(conn);
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
