#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

struct type {
	char *description;
};


static void create_user(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	char options[3] = {'1','2', '3'};
	char r;
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call crea_utente(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);

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

	//prepare parameters
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
	
	free(param);
	mysql_stmt_close(prepared_stmt);
}


void add_driver(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_conducente(?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	
	char username[46];
	char fiscal_code[17];
	char name[46];
	char surname[46];
	char birth_date[11];
	MYSQL_TIME birth;
	char birth_city[46];
	char license_number[11];
	char license_expiry_date[11];
	MYSQL_TIME expiry_date;
	
	
	// Get the required information
	printf("\nUsername: ");
	getInput(46, username, false);
	printf("fiscal code: ");
	getInput(17, fiscal_code, false);
	printf("name: ");
	getInput(46, name, false);
	printf("surname: ");
	getInput(46, surname, false);
	printf("birth date (YYYY-MM-DD): ");
	getInput(11, birth_date, false);
	printf("birth city: ");
	getInput(46, birth_city, false);
	printf("license number: ");	
	getInput(11, license_number, false);
	printf("license expiry date (YYYY-MM-DD): ");	
	getInput(11, license_expiry_date, false);
	
	// Convert values
	set_date(&birth, birth_date);
	
	set_date(&expiry_date, license_expiry_date);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = fiscal_code;
	param[0].buffer_length = strlen(fiscal_code);

	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = username;
	param[1].buffer_length = strlen(username);

	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer = name;
	param[2].buffer_length = strlen(name);
	
	param[3].buffer_type = MYSQL_TYPE_STRING;
	param[3].buffer = surname;
	param[3].buffer_length = strlen(surname);
	
	param[4].buffer_type = 	MYSQL_TYPE_DATE;
	param[4].buffer = &birth;
	param[4].buffer_length = sizeof(birth);
	
	param[5].buffer_type = MYSQL_TYPE_STRING;
	param[5].buffer = birth_city;
	param[5].buffer_length = strlen(birth_city);
	
	param[6].buffer_type = MYSQL_TYPE_STRING;
	param[6].buffer = license_number;
	param[6].buffer_length = strlen(license_number);
	
	param[7].buffer_type = MYSQL_TYPE_DATE;
	param[7].buffer = &expiry_date;
	param[7].buffer_length = sizeof(expiry_date);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("Driver correctly added...\n");
	}
	
	free(param);
	mysql_stmt_close(prepared_stmt);
}

	
void add_vehicle(MYSQL *conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_veicolo(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char license_plate[8];
	char model[46];
	char purchase_date[11];
	MYSQL_TIME purchase;
	short int car_id;
	
	// Get the required information
	printf("\nlicense plate: ");
	getInput(8, license_plate, false);
	printf("model: ");
	getInput(46, model, false);
	printf("purchase date (YYYY-MM-DD): ");
	getInput(11, purchase_date, false);
	
	// Convert values
	set_date(&purchase, purchase_date);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = license_plate;
	param[0].buffer_length = strlen(license_plate);

	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = model;
	param[1].buffer_length = strlen(model);

	param[2].buffer_type = MYSQL_TYPE_DATE;
	param[2].buffer = &purchase;
	param[2].buffer_length = sizeof(purchase);
	
	param[3].buffer_type = 	MYSQL_TYPE_SHORT;
	param[3].buffer = &car_id;
	param[3].buffer_length = sizeof(car_id);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		//Get the id of the new vehicle
		get_result_set(conn, prepared_stmt, "", &param);
		printf("Vehicle correctly added with ID %04d\n", *(short int*)param[0].buffer);
	}

	free(param);
	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);
}


void assign_shift(MYSQL *conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call assegna_turno(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char driver[17];
	char shift_date[11];
	MYSQL_TIME date;
	char car_id[5];
	short int id;
	char start_shift[6];
	MYSQL_TIME start;
	char end_shift[6];
	MYSQL_TIME end;
	
	// Get the required information
	printf("\ndriver fiscal code: ");
	getInput(17, driver, false);
	printf("date (YYYY-MM-DD): ");
	getInput(11, shift_date, false);
	printf("car id: ");
	getInput(5, car_id, false);
	printf("start time (hh:mm): ");
	getInput(6, start_shift, false);
	printf("end time (hh:mm): ");
	getInput(6, end_shift, false);
	
	// Convert values
	set_date(&date, shift_date);
	id = atoi(car_id);
	set_time(&start, start_shift);
	set_time(&end, end_shift);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = driver;
	param[0].buffer_length = strlen(driver);

	param[1].buffer_type = MYSQL_TYPE_DATE;
	param[1].buffer = &date;
	param[1].buffer_length = sizeof(date);

	param[2].buffer_type = 	MYSQL_TYPE_SHORT;
	param[2].buffer = &id;
	param[2].buffer_length = sizeof(id);
	
	param[3].buffer_type = 	MYSQL_TYPE_TIME;
	param[3].buffer = &start;
	param[3].buffer_length = sizeof(start);
	
	param[4].buffer_type = 	MYSQL_TYPE_TIME;
	param[4].buffer = &end;
	param[4].buffer_length = sizeof(end);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("Shift correctly added\n");
	}
	
	free(param);
	mysql_stmt_close(prepared_stmt);
}

void add_stop(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_fermata(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char stop_name[46];
	char latitude[46];
	char longitude[46];
	int line_num;
	
	// Get the required information
	printf("\nstop name: ");
	getInput(46, stop_name, false);
	printf("latitude (decimal): ");
	getInput(46, latitude, false);
	printf("longitude (decimal): ");
	getInput(46, longitude, false);

	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = stop_name;
	param[0].buffer_length = strlen(stop_name);

	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = latitude;
	param[1].buffer_length = strlen(latitude);

	param[2].buffer_type = 	MYSQL_TYPE_STRING;
	param[2].buffer = longitude;
	param[2].buffer_length = strlen(longitude);
	
	param[3].buffer_type = 	MYSQL_TYPE_LONG;
	param[3].buffer = &line_num;
	param[3].buffer_length = sizeof(line_num);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		get_result_set(conn, prepared_stmt, "", &param);
		printf("Stop correctly added with ID %05d\n", *(int*)param[0].buffer);
		
	}
	
	
	free(param);
	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);
}

void create_newline(MYSQL *conn) { //inoltre aggiunge automaticamente i capolinea come waypoint con sequenza 1
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call crea_tratta(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char line_num[46];
	int line;
	char partenza[46];
	int partenza_num;
	char arrivo[46];
	int arrivo_num;
	
	// Get the required information
	printf("\nline number: ");
	getInput(46, line_num, false);
	printf("departure stop id: ");
	getInput(46, partenza, false);
	printf("arrival stop id: ");
	getInput(46, arrivo, false);
	
	// Convert values
	line = atoi(line_num);
	partenza_num = atoi(partenza);
	arrivo_num = atoi(arrivo);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &line;
	param[0].buffer_length = sizeof(line);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &partenza_num;
	param[1].buffer_length = sizeof(partenza_num);

	param[2].buffer_type = 	MYSQL_TYPE_LONG;
	param[2].buffer = &arrivo_num;
	param[2].buffer_length = sizeof(arrivo_num);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("Line correctly added\n");
	}
	
	
	free(param);
	mysql_stmt_close(prepared_stmt);
}

void add_waypoint(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call crea_assegna_waypoint(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char line_num[46];
	int line;
	char direction[46];
	char latitude[46];
	char longitude[46];
	char stop_name[46] = ""; 
	
	// Get the required information
	printf("\nline number: ");
	getInput(46, line_num, false);
	printf("direction: ");
	getInput(46, direction, false);
	printf("latitude (decimal): ");
	getInput(46, latitude, false);
	printf("longitude (decimal): ");
	getInput(46, longitude, false);
	
	//il waypoint può essere opzionalmente una fermata
	if (yesOrNo("This waypoint is a stop?", 'y', 'n', 0, 1)) {
		printf("stop name: ");
		getInput(46, stop_name, false);
	}
	
	// Convert values
	line = atoi(line_num);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &line;
	param[0].buffer_length = sizeof(line);

	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = direction;
	param[1].buffer_length = strlen(direction);

	param[2].buffer_type = 	MYSQL_TYPE_STRING;
	param[2].buffer = latitude;
	param[2].buffer_length = strlen(latitude);
	
	param[3].buffer_type = 	MYSQL_TYPE_STRING;
	param[3].buffer = longitude;
	param[3].buffer_length = strlen(longitude);
	
	param[4].buffer_type = 	MYSQL_TYPE_STRING;
	param[4].buffer = stop_name;
	param[4].buffer_length = strlen(stop_name);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("Waypoint correctly added\n");
	}
	
	
	free(param);
	mysql_stmt_close(prepared_stmt);
}

void manage_line(MYSQL *conn){
	
	char options[4] = {'0','1','2','Q'};
	char op;
	
	printf("0) Add stop\n");
	printf("1) Create new line\n");
	printf("2) Add waypoint\n");
	printf("Q)Quit\n");
	
	
	op = multiChoice("Select an option", options, 4);

	switch(op) {
		case '0':
			add_stop(conn);
			break;
		case '1':
			create_newline(conn);
			break;
		case '2':
			add_waypoint(conn);	
			break;
		case 'Q':
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
	}
}


void issue_subscription(MYSQL *conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call emetti_abbonamento(?, ?, ?, ?, ?, ? ,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char username[46];
	char fiscal_code[17];
	char name[46];
	char surname[46];
	int number_sub;
	MYSQL_TIME issue_date;
	MYSQL_TIME expiry_date;
	
	// Get the required information
	printf("\nusername: ");
	getInput(46, username, false);
	printf("fiscal_code: ");
	getInput(17, fiscal_code, false);
	printf("name: ");
	getInput(46, name, false);
	printf("surname: ");
	getInput(46, surname, false);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = fiscal_code;
	param[1].buffer_length = strlen(fiscal_code);
	
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer = name;
	param[2].buffer_length = strlen(name);
	
	param[3].buffer_type = MYSQL_TYPE_STRING;
	param[3].buffer = surname;
	param[3].buffer_length = strlen(surname);
	
	param[4].buffer_type = MYSQL_TYPE_LONG;
	param[4].buffer = &number_sub;
	param[4].buffer_length = sizeof(number_sub);
	
	param[5].buffer_type = MYSQL_TYPE_DATETIME;
	param[5].buffer = &issue_date;
	param[5].buffer_length = sizeof(issue_date);
	
	param[6].buffer_type = MYSQL_TYPE_DATE;
	param[6].buffer = &expiry_date;
	param[6].buffer_length = sizeof(expiry_date);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		//Get the id of the new vehicle
		get_result_set(conn, prepared_stmt, "", &param);
		char issue[20];
		char expiry[11];
		get_datetime(*(MYSQL_TIME *)param[1].buffer, issue);
		get_date(*(MYSQL_TIME *)param[2].buffer, expiry);
		
		printf("Subscription completed successfully with id: %07d\nissue date: %s\nexpiry date: %s\n", *(int *)param[0].buffer, issue, expiry);
	}

	free(param);
	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);
}


void issue_ticket(MYSQL *conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call emetti_biglietto(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char username[46];
	int ticket_num;
	
	// Get the required information
	printf("\nusername: ");
	getInput(46, username, false);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);
	
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &ticket_num;
	param[1].buffer_length = sizeof(ticket_num);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
	//Get the id of the new ticket
		get_result_set(conn, prepared_stmt, "", &param);	
		printf("Ticket issued correctly with id: %010d", *(int *)param[0].buffer);
	}
	
	free(param);
	mysql_stmt_next_result(prepared_stmt);  //necessario per assicurarsi che tutti i result set siano stati consumati (anche se ovviamente non è presente un resul set successivo)
	mysql_stmt_close(prepared_stmt);
}


void put_on_sick_leave(MYSQL *conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	int num_rows;
	MYSQL_TIME date;
	MYSQL_TIME start_time;
	MYSQL_TIME end_time;
	char targa[8];
	char modello[46];
	MYSQL_TIME *days;
	int status;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiorna_stato_conducente(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char driver[46];
	char *stato = "malattia";
	
	// Get the required information
	printf("\ndriver username: ");
	getInput(46, driver, false);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = driver;
	param[0].buffer_length = strlen(driver);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = stato;
	param[1].buffer_length = strlen(stato);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("Driver status updated correctly\n");
	}
	
	free(param);

	mysql_stmt_close(prepared_stmt);
	
	//**********************************************************
	//recupera i turni assegnati al conducente in malattia
	

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call lista_turni_conducente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = driver;
	param[0].buffer_length = strlen(driver);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		finish_with_stmt_error (conn, prepared_stmt, "An error occurred while adding the user.", true);
	}

	if (mysql_stmt_store_result(prepared_stmt)) {
		fprintf(stderr, " mysql_stmt_store_result(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt));
		exit(0);
	}  
	
	num_rows = mysql_stmt_num_rows(prepared_stmt);
	
	//Binding output column
	param = malloc(5*sizeof(MYSQL_BIND));
	if (param == NULL) {
		finish_with_stmt_error(conn, prepared_stmt, "Cannot allocate output buffers\n", true);
	}		
	memset(param, 0, 5 * sizeof (MYSQL_BIND));
	
	param[0].buffer_type = MYSQL_TYPE_DATE;
	param[0].buffer =  &date;
	param[0].buffer_length = sizeof(date);
	
	param[1].buffer_type = MYSQL_TYPE_TIME;
	param[1].buffer =  &start_time;
	param[1].buffer_length = sizeof(start_time);
	
	param[2].buffer_type = MYSQL_TYPE_TIME;
	param[2].buffer =  &end_time;
	param[2].buffer_length = sizeof(end_time);

	param[3].buffer_type = MYSQL_TYPE_STRING;
	param[3].buffer = targa;
	param[3].buffer_length = strlen(targa);
	
	param[4].buffer_type = MYSQL_TYPE_STRING;
	param[4].buffer =  modello;
	param[4].buffer_length = strlen(modello);
	
	if(mysql_stmt_bind_result(prepared_stmt, param)) {
			finish_with_stmt_error(conn, prepared_stmt, "Unable to bind output parameters\n", true);
	}
	
	days = malloc(num_rows * sizeof(MYSQL_TIME));
	if (days == NULL) {
		finish_with_stmt_error(conn, prepared_stmt, "Cannot allocate output buffers\n", true);
	}		
	memset(days, 0, num_rows * sizeof(MYSQL_TIME));
	
	/* fetch rows */ 
	int i = 0;
	while (true) {
		status = mysql_stmt_fetch(prepared_stmt);  //mette nei buffer associati con il bind i valori delle colonne della riga successiva

		if (status == 1 || status == MYSQL_NO_DATA)  //quando non ci sono più righe da leggere (o si verifica un errore) esce dal while
			break;
			
		days[i] = date;
		i++;
	}
	
	free(param);
	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);	
	
	
	
	//***************************************************************
	
	for (int i = 0; i < num_rows; i++) {
		
		// Prepare stored procedure call
		if(!setup_prepared_stmt(&prepared_stmt, "call aggiorna_turno(?, ?, ?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
		}
	
		//setup MYSQL_BIND structures
		setup_binding(conn, prepared_stmt, &param);
		
		char new_driver[46];
		
		printf("Reassign the shift for the day %04d-%02d-%02d...\n", (days+i)->year, (days+i)->month, (days+i)->day);
		// Get the required information
		printf("\nnew driver username: ");
		getInput(46, new_driver, false);
		
		//prepare parameters
		param[0].buffer_type = MYSQL_TYPE_STRING;
		param[0].buffer = new_driver;
		param[0].buffer_length = strlen(new_driver);
		
		//prepare parameters
		param[1].buffer_type = MYSQL_TYPE_STRING;
		param[1].buffer = driver;
		param[1].buffer_length = strlen(driver);
		
		//prepare parameters
		param[2].buffer_type = MYSQL_TYPE_DATE;
		param[2].buffer = days+i;
		param[2].buffer_length = sizeof(MYSQL_TIME);
		
			//binding parameters
		if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
			finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
		}
	
	
		// Run procedure
		if (mysql_stmt_execute(prepared_stmt) != 0) {
			print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
		} else {
			printf("Shift updated correctly\n");
		}
	
		free(param);

		mysql_stmt_close(prepared_stmt);
	}
}


void assign_timetable(MYSQL *conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_tabella_oraria(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char line_num[46];
	int line;
	char direction[46];
	char departure_time[6];
	MYSQL_TIME departure;
	char vehicle_id[46];
	short int vehicle;
	
	// Get the required information
	printf("\nline number: ");
	getInput(46, line_num, false);
	printf("direction: ");
	getInput(46, direction, false);
    printf("departure time (hh:mm): ");
	getInput(6, departure_time, false);
    printf("vehicle id: ");
	getInput(46, vehicle_id, false);
	
	// Convert values
	line = atoi(line_num);
	set_time(&departure, departure_time);
	vehicle = atoi(vehicle_id);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &line;
	param[0].buffer_length = sizeof(line);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = direction;
	param[1].buffer_length = strlen(direction);
	
	param[2].buffer_type = MYSQL_TYPE_TIME;
	param[2].buffer = &departure;
	param[2].buffer_length = sizeof(departure);
	
	param[3].buffer_type = MYSQL_TYPE_SHORT;
	param[3].buffer = &vehicle;
	param[3].buffer_length = sizeof(vehicle);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("Departure time added");
	}
	
	free(param);
	mysql_stmt_close(prepared_stmt);	
}

void show_maintenance(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call mostra_parco_veicoli()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		get_result_set(conn, prepared_stmt, "\nChoose a vehicle from the following:\n", &param);
		if (mysql_stmt_next_result(prepared_stmt))
				finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
	}
	
	mysql_stmt_close(prepared_stmt);
	
	//***********************************************

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call lista_interventi_manutenzione(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char vehicle_id[46];
	short int vehicle;
	int num_km;
	
	// Get the required information
	printf("\nvehicle id: ");
	getInput(46, vehicle_id, false);
	
	// Convert values
	vehicle = atoi(vehicle_id);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_SHORT;
	param[0].buffer = &vehicle;
	param[0].buffer_length = sizeof(vehicle);
	
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &num_km;
	param[1].buffer_length = sizeof(num_km);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		int status;
		int num_works;
		int first = 1;
		do {
			get_result_set(conn, prepared_stmt, "\nMaintenance works of selected vehicle", &param);
			if (first) {
				num_works = mysql_stmt_num_rows(prepared_stmt); //non può ritornare errori
			}
			status = mysql_stmt_next_result(prepared_stmt);
			first = 0;
			if (status > 0)
				finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		} while (status == 0);
		
		//show the current vehicle kilometers (if available)
		if (num_works != 0)
			printf("Vehicle current kilometers: %d", *(int *)param[0].buffer);
		else 
			printf("\nThere is no recorded maintenance\n");
	}
	
	free(param);
	mysql_stmt_close(prepared_stmt);	
}

void add_maintenance(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND *param;
	int num_rows;
	int status;
	char maintenance_type[46];
	struct type *types;
	
	//get maintenance types available
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call mostra_tipi_intervento()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		finish_with_stmt_error (conn, prepared_stmt, "An error occurred while adding the user.", true);
	}


	if (mysql_stmt_store_result(prepared_stmt)) {
		fprintf(stderr, " mysql_stmt_store_result(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt));
		exit(0);
	}  
	
	num_rows = mysql_stmt_num_rows(prepared_stmt);
	
	//Binding output column
	param = malloc(sizeof(MYSQL_BIND));
	if (param == NULL) {
		finish_with_stmt_error(conn, prepared_stmt, "Cannot allocate output buffers\n", true);
	}		
	memset(param, 0, sizeof (MYSQL_BIND));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer =  maintenance_type;
	param[0].buffer_length = 46;
	
	if(mysql_stmt_bind_result(prepared_stmt, param)) {
			finish_with_stmt_error(conn, prepared_stmt, "Unable to bind output parameters\n", true);
	}
	
	types = malloc(num_rows * sizeof(struct type));
	if (types == NULL) {
		finish_with_stmt_error(conn, prepared_stmt, "Cannot allocate output buffers\n", true);
	}		
	memset(types, 0, num_rows * sizeof (struct type));
	
	/* fetch rows */ 
	int i = 0;
	while (true) {
		status = mysql_stmt_fetch(prepared_stmt);  //mette nei buffer associati con il bind i valori delle colonne della riga successiva

		if (status == 1 || status == MYSQL_NO_DATA)  //quando non ci sono più righe da leggere (o si verifica un errore) esce dal while
			break;
			
		(types + i)->description = malloc(strlen(maintenance_type)+1);
		memcpy((types + i)->description, maintenance_type,strlen(maintenance_type)+1);
		((types + i)->description)[strlen(maintenance_type)] = '\0';
		i++;
	}
	
	free(param);
	mysql_stmt_next_result(prepared_stmt);
	mysql_stmt_close(prepared_stmt);	
	
	//************************************************************
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call registra_intervento(?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}
	
	//setup MYSQL_BIND structures
	setup_binding(conn, prepared_stmt, &param);
	
	char vehicle_id[46];
	short int vehicle;
	char maintenance[46];
	char maintenance_date[11];
	MYSQL_TIME date;
	char num_km[46];
	int kilometers;
	
	// Get the required information
	printf("\nvehicle id: ");
	getInput(46, vehicle_id, false);
	
	//show maintenance types available
	printf("\nChoose maintenance work type:\n");
	for (int i = 0; i < num_rows; i++ ) {
		printf("--%s\n", (types + i)->description);
	}
	
	printf("\nmaintenance work: ");
	getInput(46, maintenance, false);
	printf("date (YYYY-MM-DD): ");
	getInput(11, maintenance_date, false);
	printf("number of kilometers: ");
	getInput(46, num_km, false);
	
	// Convert values
	vehicle = atoi(vehicle_id);
	set_date(&date, maintenance_date);
	kilometers = atoi(num_km);
	
	//prepare parameters
	param[0].buffer_type = MYSQL_TYPE_SHORT;
	param[0].buffer = &vehicle;
	param[0].buffer_length = sizeof(vehicle);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = maintenance;
	param[1].buffer_length = strlen(maintenance);

	param[2].buffer_type = MYSQL_TYPE_DATE;
	param[2].buffer = &date;
	param[2].buffer_length = sizeof(date);
	
	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &kilometers;
	param[3].buffer_length = sizeof(kilometers);
	
	//binding parameters
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}	
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("Maintenance work added correctly\n");
	}
	
	//free output buffers
	for (int i = 0; i < num_rows; i++ ) {
		free((types+i)->description);
	}
	free(types);
	free(param);
	mysql_stmt_close(prepared_stmt);
}


void manage_maintenance(MYSQL *conn) {
	char options[3] = {'0','1','Q'};
	char op;
	
	printf("0) Show maintenance works\n");
	printf("1) Add maintenance work\n");
	printf("Q)Quit\n");
	
	
	op = multiChoice("Select an option", options, 3);

	switch(op) {
		case '0':
			show_maintenance(conn);
			break;
		case '1':
			add_maintenance(conn);
			break;
		case 'Q':
			return;
				
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
	}
}


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
		printf("4) Manage line\n");
		printf("5) Issue a subscription\n");
		printf("6) Issue a ticket\n");
		printf("7) Put a driver on sick leave\n");
		printf("8) Assign timetable of departures\n");
		printf("9) Manage maintenance work\n");
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
				manage_line(conn);
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
				manage_maintenance(conn);
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
