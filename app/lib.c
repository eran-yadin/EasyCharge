#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "lib.h"
#include "port.h"
#include "binery.h"
#include "inputer.h"
#include "print_lib.h"
#include "error_handle.h"

//----files names----
#define stations_file "Stations.txt"
#define ports_file "Ports.txt"
#define queues_file "LineOfCars.txt"
#define cars_file "Cars.txt"

// Define function codes for menu options
#define FUNC_1_locate_nearest_station 1
#define FUNC_2_charge_car 2
#define FUNC_3_check_car_statue 3
#define FUNC_4_stop_charge 4
#define FUNC_5_display_all_stations 5
#define FUNC_6_ 6
#define FUNC_7_ 7
#define FUNC_8_ 8
#define FUNC_9_ 9
#define FUNC_10_ 10
#define FUNC_11_ 11
#define FUNC_12_ 12
#define FUNC_13_ 13
#define FUNC_0_exit 0

#define CHARGE_COST 1.2
#define OCCUPID_PORT 1 // Port status for occupied
#define FREE_PORT 2 // Port status for free
#define OUT_OF_ORDER_PORT 3 // Port status for out of order

#define OUT_OF_ORDER_PORT 3 // Port status for out of order ports


//---------print settings-----------
//print_ALL_DB
#define st_choise_des {1, 1, 1, 1, 1, 1, 1, 1} // id,name,nPorts,coord,nCars,port_list,car_que_num,carQueue
#define st_port_des {1, 1, 1, 1, 1} //print_choise[num,port_type,status,charge_time,car]
#define car_choise_des { 1, 1, 1, 1, 1 } //print_choise[nLicense,type,totalPayed,inqueu,port]
//print_all_cars_in_stastion
#define car_choise_des_1 {1, 1, 1, 1, 1} // nLicense,type,totalPayed,inqueu,port
//


//load all files to the app
DB_holder * loadFiles()
{
	// Load stations, ports, queues, and cars from files
	Station* st_db = read_st_from_file(stations_file);
	raw_db_port* port_data = getRowData_Port_from_file(ports_file);
	raw_que** queue_data = get_raw_que_from_file(queues_file);
	tCar* car_db = get_tCar_from_file(cars_file);

	//add the port to the station
	add_ports_to_stations(st_db, port_data, car_db);
	//check how many cars in the station
	check_number_of_cars(st_db);
	//add the queue to the station
	add_que_to_st(st_db, car_db, queue_data);
	//port raw data
	free_raw_db_port(port_data);
	//check the port count in the station
	check_port_count(st_db);
	DB_holder *db_holder = malloc(sizeof(DB_holder));
	if (db_holder == NULL) {
		//error code: 1002
		fprintf(stderr, "Memory allocation failed for DB_holder\n");
		return NULL;
	}
	db_holder->st_db = st_db;
	db_holder->car_db = car_db;
	db_holder->date = getCurrentDate();
	return db_holder;
}

DB_holder* save_files(DB_holder* db_holder)
{
	//check case if db_holder is NULL or if the databases are NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "DB_holder is NULL.\n");
		return NULL;
	}
	//check if data exist in data base (no data no save)
	if (db_holder->st_db == NULL || db_holder->car_db == NULL) {
		//error code:
		fprintf(stderr, "Station or car database is NULL.\n");
		return NULL;
	}
	write_st_to_file(db_holder->st_db, stations_file);
	write_tCar_to_file(db_holder->car_db,db_holder->st_db, cars_file);
	write_que_to_file(db_holder->st_db, queues_file);
	write_port_to_file(db_holder->st_db,ports_file);
	//printf("Files saved successfully.\n");//debugging message
	return db_holder;
}

//load menu and get user input (also check if input valid)
int menu_decision()
{
	int decision;
	do
	{
		print_menu();
		int scn = scanf("%d", &decision);
		if (scn != 1) { log_error(5, "input in decision"); }
		clean_stdin(); // Clear the input buffer
		if (!is_valid(decision, 13, 0)) {
			printf("Invalid choice. Please choose a number between 1 and 14.\n");
		}
	} while (!is_valid(decision, 13, 0));
	return decision;
}

void free_DB_holder(DB_holder* db_holder)
{
	if (db_holder == NULL) {
		return; // Nothing to free
	}
	if (db_holder->st_db != NULL) {
		free_station(db_holder->st_db);
	}
	if (db_holder->car_db != NULL) {
		free_tCar_db(db_holder->car_db);
	}	
}

void fun_executer(int decision, DB_holder* db_holder)
{
	
	
	if (db_holder == NULL) {
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	switch (decision)
	{
	case FUNC_1_locate_nearest_station:
		locNearSt(db_holder->st_db);
		break;
	case FUNC_2_charge_car:
		charge_car(db_holder);
		break;
	case FUNC_3_check_car_statue:
		checkCarStatus(*db_holder); // Check car status
		break;
	case FUNC_4_stop_charge:
		stop_charging(db_holder);
		break;
	case FUNC_5_display_all_stations: {
		int st_choise[] = st_choise_des;
		int port_choise[] = st_port_des;
		int car_choise[] = car_choise_des;
		print_ALL_DB(db_holder->st_db, st_choise, port_choise, car_choise);
		break;
	}
	case FUNC_6_:
		display_all_cars_in_station(db_holder);
		break;
	case FUNC_7_:
		report_station_statistics(db_holder);
		break;
	case FUNC_8_:
		find_top_customer_five(db_holder);
		break;
	case FUNC_9_:
		add_new_port(db_holder);
		break;
	case FUNC_10_:
		Release_charging_ports(db_holder);
		break;
	case FUNC_11_:
		remOutOrderPort(db_holder->st_db);
		break;
	case FUNC_12_:
		remove_customer(db_holder);
		break;
	case FUNC_13_:
		close_station(db_holder);
		break;
	default:
		printf("Exiting the program.\n");
		//add new line test
		//save_files(db_holder); // Save the database before exiting
		free_DB_holder(db_holder); // Free the database holder
		exit(0);
	}
}

//func 1: locate nearest station FASE: TESTING
Station* locNearSt(Station* st_db)
{
	printf("\nenter coordinates: \n");
	coord user_coord = get_user_coord();
	if (st_db == NULL) {
		//error code:
		return NULL;
	}
	Station* nearest_station = locNearSt_rec(st_db, user_coord);
	if (nearest_station == NULL) {
		printf("No stations found.\n");
		return NULL;
	}
	print_nearest_Station(nearest_station, user_coord);
	printf("----------------------------\n");
	wait_for_user();
	return nearest_station;
}

Station* locNearSt_rec(Station* st_db, coord user_coord)
{
	if (st_db == NULL) {
		return NULL;
	}
	double nearest_dist = distance(user_coord, convert_coord(st_db->coord[0],st_db->coord[1]));
	Station* nearest = st_db;

	Station* left_nearest = locNearSt_rec(st_db->left, user_coord);
	if (left_nearest != NULL) {
		double left_dist = distance(user_coord, convert_coord(left_nearest->coord[0],left_nearest->coord[1]));
		if (left_dist < nearest_dist) {
			nearest = left_nearest;
			nearest_dist = left_dist;
		}
	}

	Station* right_nearest = locNearSt_rec(st_db->right, user_coord);
	if (right_nearest != NULL) {
		double right_dist = distance(user_coord, convert_coord(right_nearest->coord[0],right_nearest->coord[1]));
		if (right_dist < nearest_dist) {
			nearest = right_nearest;
			nearest_dist = right_dist;
		}
	}
	return nearest;
}

double distance(coord a, coord b)
{
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx * dx + dy * dy);
}

coord convert_coord(double x, double y)
{
	coord c;
	c.x = x;
	c.y = y;
	return c;
}

void print_nearest_Station(Station* st,coord user_coord)
{
	if (st == NULL) {
		//error code:
		printf("No stations available.\n");
		return;
	}
	printf("\n----------------------------\n");
	double dist = distance(user_coord, convert_coord(st->coord[0], st->coord[1]));
	printf("Nearest Station:\nname: %s\nid: %d\ndistance: %lf KM\n", st->name,st->id,dist*111);
	print_port_in_st_only_working(st);
	printf("number of occupid ports: %d\n", port_occupid(st));
	if (st->nCars > 0) {
		printf("Number of cars in the station: %d\n", st->nCars);
	}
	else {
		printf("No cars in the station.\n");
	}
}

//func 2: add new station FASE: TESTING
void charge_car(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	Station* st = NULL;

	//get user station ID 
	printf("Enter Station ID: ");
	do
	{
		char* st_id_str = get_user_string();
		unsigned int* st_id_unsigned = turn_string_to_us_int(st_id_str);
		if (st_id_unsigned && is_str_0(st_id_str))
		{			// User chose to exit
			printf("Exiting...\n");
			wait_for_user();
			return;
		}
		if (st_id_unsigned) {
			st = find_station_by_id(db_holder->st_db, *st_id_unsigned);
		}
		else
		{
			st = find_station_by_name(db_holder->st_db, st_id_str);
		}
		if (!st)
		{	//st does not exist
			printf("station does not exist, try again OR enter 0 to exit: ");
		}
	} while (!st);
	//get user license plate
	char* license_plate;
	tCar* tcar = init_tCar();
	do
	{
		license_plate = get_user_nLisence();
		if (license_plate == NULL) {
			//error code: 
			return;
		}
		tcar->car = find_car(db_holder->car_db, license_plate);
		if (!tcar->car) 
		{
			printf("Car with license plate %s does not exist\ndo you want:\n[1]. to add a car\nOR\n[2]. try gain\n[0] exit ", license_plate);
			int choice;
			do
			{
				scanf("%d", &choice);
				clean_stdin(); // Clear the input buffer
				if (!is_valid(choice, 2, 0)) {
					printf("Invalid choice. Please choose 1, 2, or 0.\n");
				}
			} while (!is_valid(choice, 2, 0));
			
			// Process the user's choice
			// i could have done this with a switch case i know!
			if (choice == 0) {
				printf("Exiting...\n");
				wait_for_user();
				free(license_plate);
				return;
			}
			else if (choice == 1) {
				//get new car details
				tcar= get_user_new_car();
				//add the new car to the database
				db_holder->car_db = rec_add_to_tree(db_holder->car_db, tcar);
				tcar->car->pPort = NULL;
			}
			else if (choice == 2) {
				printf("Try again: ");
			}
			free(license_plate);
			license_plate = NULL;
		}
	} while (!tcar->car);

	//car exist and station exist
	Port* port = is_port_type_exist(st, tcar->car->type);
	if (tcar->car->pPort) { printf("car allready in a port\n"); wait_for_user();printf("\n\n\n"); return; }//car all ready charging
	if (tcar->car->inqueue == 1) { printf("car allready inqueue for a port\n"); wait_for_user();printf("\n\n\n"); return; } //car waiting for charging
	if (!port || port->status == 3) //no ports available or port is under maintenance
	{
		printf("No ports available for this car type.\n");
		wait_for_user();
		printf("\n\n\n");
		return;
	}
	if (port->status == 2) //free
	{
		port->p2car = tcar->car; // Assign the car to the port
		port->status = 1; // Mark the port as occupied
		tcar->car->pPort = port; // Assign the port to the car
		tcar->car->inqueue = 0; // Mark the car as not in queue
		st->nCars++; // Increment the number of cars in the station
		port->tin = getCurrentDate(); // Set the time in for the port
		printf("Car with license plate %s has been charged at port %d in station %s.\n",
			tcar->car->nLicense, port->num, st->name);
	}
	else //occupied
	{
		carNode* car_node = malloc(sizeof(carNode));
		if (car_node == NULL) {
			//error code: memory allocation failed
			fprintf(stderr, "Memory allocation failed for car node.\n");
			free(tcar->car);
			free(tcar);
			return;
		}
		car_node->car = tcar->car; // Assign the car to the new car node
		st->carQueue.rear->next = car_node; // Link the new car node at the end of the queue
		st->carQueue.rear = car_node; // Update the rear pointer to the new car node
		st->nCars++; // Increment the number of cars in the station
		tcar->car->inqueue = 1;
		printf("Car with license plate %s has been added to the queue in station %s.\n",
			tcar->car->nLicense, st->name);
	}
	printf("---------------------\n");
	wait_for_user();
	printf("\n\n\n");
}


//func 3: check car status FASE: TESTING
void checkCarStatus(DB_holder db)
{
	Car* user_car;
	do
	{
		printf("Enter your car license plate (or 0 to exit): ");
		char* user_lis = get_user_nLisence();
		if(is_str_0(user_lis)) // User chose to exit
		{
			printf("Exiting...\n");
			wait_for_user();
			return;
		}
		user_car = find_car(db.car_db, user_lis);
	} while (!user_car);

	if (user_car == NULL)//dont find car
	{
		printf("car is not found :(\n");//optional- ask user what to do next
		return;
	}
	
	if (user_car->pPort)//car in port
	{
		//get station
		Station* st = find_station_by_car(db.st_db, user_car->nLicense);
		if (st == NULL) {
			printf("station not found for car %s\n", user_car->nLicense);
			return;
		}
		printf("car is charging in station %s\n", st->name);
		int port[5] = { 1, 0, 0, 1, 0 };
		print_port(user_car->pPort, port,NULL);
		return;
	}

	if (user_car->inqueue)
	{
		//get station
		Station* st = find_station_by_car(db.st_db, user_car->nLicense);
		//add print func
		int que = how_long_car_que(st, user_car);
		printf("car in place %d is in queue for charging in station %s\n", que,st->name);
		return;
	}
	printf("car is not charging and not in queue\n");
	return;
}


//func 4: stop charge FASE: TESTING
void stop_charging(DB_holder* DB)
{
	//check DB
	if (DB == NULL || DB->car_db == NULL || DB->st_db == NULL)
	{
		return; //error code: DB is NULL
	}

	//get user data
	Car* u_car;
	//car exist?
	do
	{
		printf("Enter your car license plate (or 0 to exit): ");
		char* user_lis = get_user_nLisence();
		if ( user_lis[7] == '0') // User chose to exit
		{
			printf("Exiting...\n");
			wait_for_user();
			return;
		}
		u_car = find_car(DB->car_db, user_lis);
		if (!u_car) { printf("car with license: %s doesn't exist\n", user_lis); }
	} while (!u_car);
	
	//car in port?
	if (u_car->pPort == NULL)
	{
		printf("\ncar is not charging\n");
		return; //error code: car is not charging
	}
	//car exit and in port
	Station* st = find_station_by_car(DB->st_db, u_car->nLicense);
	if (st == NULL)
	{
		printf("\nstation was not found\n");
		return; //error code: station not found
	}

	carNode* car_node = st->carQueue.front;
	//find car in queue with same type
	while( car_node != NULL && car_node->car->type != u_car->type)
	{
		car_node = car_node->next;
	}
	Port* correct_port = st->portList; // Get the port where the car is charging
	while (correct_port != NULL && correct_port->p2car != u_car)
	{
		correct_port = correct_port->next; // Find the port where the car is charging
	}
	if (correct_port == NULL) {
		printf("Car is not charging in any port.\n");
		return; //error code: car is not charging
	}
	// Calculate the total time charged
	int unsigned time_charged = get_charge_min(correct_port->tin, getCurrentDate());
	double charge_cost = 0.0;
	charge_cost = time_charged * CHARGE_COST;
	u_car->totalPayed += charge_cost; // Update the total amount paid by the car
	u_car->pPort = NULL; // Remove the car from the port
	if (car_node != NULL)
	{
		correct_port->p2car = car_node->car;
		remove_car_from_queue(st, car_node->car->nLicense); // Remove the car from the queue
		car_node->car->pPort = correct_port; // Assign the port to the car
	}else
	{
				correct_port->p2car = NULL; // Set the port's car pointer to NULL
				correct_port->status = 2; // Mark the port as free
	}
	//debug tool
	printf("Car with license plate %s has stopped charging.\n", u_car->nLicense);
	printf("Total time charged: %d minutes.\n", time_charged);
	printf("Total amount paid: %.2f.\n", u_car->totalPayed);
	printf("Port %d in station %s ", correct_port->num, st->name);
	if(car_node != NULL)
	{
		printf("is now occupied by car with license plate %s.\n", car_node->car->nLicense);
	}
	else
	{
		printf("is now free.\n");
	}
	
	// Update the station's number of cars
	return; // Successfully stopped charging
}


//func 5: display all stations
// print_ALL_DB

//func 6: display all cars in station
void display_all_cars_in_station(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	//get user station ID 
	printf("Enter Station ID or Name: ");
	Station* st = get_user_station(db_holder->st_db);
	if (!st)
	{
		//st wasnt found
		return;
	}
	int st_choise[] = st_choise_des;
	int car_choise[] = car_choise_des_1;
	print_station(st, st_choise, NULL, car_choise);
	wait_for_user();
	printf("\n\n\n");
}

//func 7: report of station's statistics
void report_station_statistics(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	//get user station ID 
	printf("Enter Station ID: ");
	Station* st = get_user_station(db_holder->st_db);
	if (!st)
	{
		//st wasnt found
		return;
	}
	print_station_statistics(st);
	wait_for_user();
	printf("\n\n\n");
}

void print_station_statistics(Station* st)
{
	if (st == NULL) {
		printf("No station found.\n");
		return;
	}
	//setup  numbers for printing
	int port_sta;
	float port_load;
	int count = 0;
	int car_count = 0;
	//get port occupation percentage
	Port* port = st->portList; // Get the head of the port list
	while (port != NULL)
	{
		if (port->status == 1) { // If the port is occupied
			count++;
		}
		port = port->next; // Move to the next port in the list
	}
	port_sta = (count*100)/st->nPorts; // Calculate the percentage of occupied ports
	//get car queue load
	carNode* car_node = st->carQueue.front; // Get the head of the car queue
	while (car_node != NULL)
	{
		car_count++; // Count the number of cars in the queue
		car_node = car_node->next; // Move to the next car in the queue
	}
	port_load = (float)car_count / st->nPorts; // Calculate the port load as a percentage
	//print statistics
	printf("Station ID: %d\n", st->id);
	printf("Station Name: %s\n", st->name);
	printf("precent of occupied ports: %d%%\n", port_sta);
	printf("port load: %f\n", port_load);

	//calculate the percentage of non-working ports
	count = 0; // Reset count for occupied ports
	port = st->portList; // Reset port to the head of the port list
	while (port != NULL)
	{
		if (port->status == 3) { // If the port is occupied
			count++;
		}
		port = port->next; // Move to the next port in the list
	}
	port_sta = (count * 100) / st->nPorts; // Calculate the percentage of occupied ports
	//print non-working ports statistics
	printf("precent of out of order ports: %d%%\n", port_sta);
}

//func 8: Display top customer
void find_top_customer_five(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	//get user station ID 
	tCar* car_db = db_holder->car_db;
	if (!car_db)
	{
		//st wasnt found
		return;
	}
	Car* top_five_customer[5] = { NULL, NULL, NULL, NULL, NULL }; // Array to hold top 5 customers
	for (size_t i = 0; i < 5; i++)
	{
		top_five_customer[i] = find_top_customer(car_db, top_five_customer);
	}
	for (size_t i = 0; i < 5; i++)
	{
		print_top_customer(top_five_customer[i]);
	}
	wait_for_user();
	printf("\n\n\n");
}

Car* find_top_customer(tCar* car_db,Car *top_five_cust[5])
{
	if(car_db == NULL) {
		//base case: if the tree is empty
		return NULL;
	}
	if (car_db->car == NULL) {
		fprintf(stderr, "Car record is missing.\n");
		return NULL;
	}
	if( top_five_cust == NULL) {
		fprintf(stderr, "Top five customers array is NULL.\n");
		return NULL;
	}
	Car* top_customer = car_db->car;
	double max_amount = 0;
	if (is_in_top_five(top_customer, top_five_cust)) {
		top_customer = NULL;
		max_amount = 0;
	}
	else { max_amount = top_customer->totalPayed; }

	Car* left_c = find_top_customer(car_db->left, top_five_cust);
	if (left_c != NULL && left_c->totalPayed > max_amount) {
		max_amount = left_c->totalPayed;
		top_customer = left_c;
	}
	Car* right_c = find_top_customer(car_db->right, top_five_cust);
	if (right_c != NULL && right_c->totalPayed > max_amount) {
		max_amount = right_c->totalPayed;
		top_customer = right_c;
	}
	return top_customer;
}

int is_in_top_five(Car* car, Car* top_five[5]) {
	for (int i = 0; i < 5; i++) {
		if (car == top_five[i]) return 1;
	}
	return 0;
}


void print_top_customer(Car* top_customer)
{
	if (top_customer == NULL) {
		printf("No customers found.\n");
		return;
	}
	printf("Top Customer:\n");
	printf("License Plate: %s\n", top_customer->nLicense);
	printf("Total Amount Paid: %.2f\n", top_customer->totalPayed);
	printf("Car Type: %d\n", top_customer->type);
	printf("----------------------------\n");
}
//run on all stations and find the top customer

//func 9: Add new port
void add_new_port(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}

	//get user station
	printf("Enter Station ID or name: ");
	Station* st = get_user_station(db_holder->st_db);
	if (!st)
	{
		//st wasnt found
		return;
	}
	portType type = get_port_type_from_user(); // Get the port type from the user
	//create a new port
	Port* new_port = malloc(sizeof(Port));
	if (new_port == NULL) {
		//error code: memory allocation failed
		fprintf(stderr, "Memory allocation failed for new port.\n");
		return;
	}
	new_port->num = st->nPorts + 1; // Assign a new port number
	new_port->type = type; // Set the port type
	Port* last_port = st->portList; // Get the head of the port list
	while (last_port != NULL && last_port->next != NULL)
	{
		last_port = last_port->next; // Traverse to the end of the port list
	}
	if (last_port == NULL) {
		st->portList = new_port; // If the port list is empty, set the new port as the head
		new_port->num = 1;
	}
	else {
		last_port->next = new_port; // Otherwise, add the new port to the end of the list
		new_port->num = last_port->num + 1; // Assign the new port number
	}
	carNode* car_node = st->carQueue.front; // Get the head of the car queue
	while (car_node != NULL && car_node->car->type != type)
	{
		car_node = car_node->next; // Find a car in the queue with the same type
	}
	if (car_node)
	{
		new_port->p2car = car_node->car; // Assign the car to the new port
		remove_car_from_queue(st, car_node->car->nLicense); // Remove the car from the queue
		car_node->car->pPort = new_port; // Assign the port to the car
		new_port->status = OCCUPID_PORT; // Mark the port as occupied
		printf("Car with license plate %s has been charged at port %d in station %s.\n",
			car_node->car->nLicense, new_port->num, st->name);
	}
	else {
		new_port->p2car = NULL; // If no car found, set the port's car pointer to NULL
		new_port->status = FREE_PORT; // Mark the port as free
	}
	
}

//func 10: Release charging port FASE: TESTING
void Release_charging_ports(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	//get user station ID 
	printf("Enter Station ID: ");
	Station* st = get_user_station(db_holder->st_db);
	if (!st)
	{
		//st wasnt found
		return;
	}
	Port* u_port = st->portList; // Get the head of the port list
	while (u_port != NULL)
	{
		if (u_port->status == 3) { continue; }
		if (u_port->p2car) {
			int unsigned time_charged = get_charge_min(u_port->tin, getCurrentDate());
			if(time_charged >= 30) // Check if the car has been charging for at least 30 minutes
			{
				release_car_from_port(u_port, st); // Release the car from the port
			}
		}
		u_port = u_port->next; // Move to the next port in the list
	}
}

//need to be tested
void release_car_from_port(Port* u_port,Station* st)
{
	Car* u_car = u_port->p2car; // Get the car from the port
	carNode* car_node = st->carQueue.front;
	//find car in queue with same type
	while (car_node != NULL && car_node->car->type != u_car->type)
	{
		car_node = car_node->next;
	}
	Port* correct_port = u_port; // Get the port where the car is charging
	
	// Calculate the total time charged
	int unsigned time_charged = get_charge_min(correct_port->tin, getCurrentDate());
	double charge_cost = 0.0;
	charge_cost = time_charged * CHARGE_COST;
	u_car->totalPayed += charge_cost; // Update the total amount paid by the car
	u_car->pPort = NULL; // Remove the car from the port
	if (car_node != NULL)
	{
		correct_port->p2car = car_node->car;
		remove_car_from_queue(st, car_node->car->nLicense); // Remove the car from the queue
		car_node->car->pPort = correct_port; // Assign the port to the car
	}
	else
	{
		correct_port->p2car = NULL; // Set the port's car pointer to NULL
		correct_port->status = 2; // Mark the port as free
	}
	//debug tool
	printf("Car with license plate %s has stopped charging.\n", u_car->nLicense);
	printf("Total time charged: %d minutes.\n", time_charged);
	printf("Total amount paid: %.2f.\n", u_car->totalPayed);
	printf("Port %d in station %s ", correct_port->num, st->name);
	if (car_node != NULL)
	{
		printf("is now occupied by car with license plate %s.\n", car_node->car->nLicense);
	}
	else
	{
		printf("is now free.\n");
	}

	// Update the station's number of cars
	return; // Successfully stopped charging
}



//func 11: Remove out of order port
int remove_out_of_order_port(Station* st, int num)// auxiliary function to remove out of order ports from a station
{
	Port* current = st->portList; // Start from the head of the port list
	Port* previous = NULL;
	Port* temp = NULL;
	
	while (current)
	{
		if (num == current->num)
		{
			if (current->status == OUT_OF_ORDER_PORT)
			{
				temp = current; //save the address of the current node

				if (previous == NULL) //if removing the head of the list
				{
					st->portList = current->next; // update the head of the list
					current = st->portList; // advance current to the new head
				}
				else // for removing a middle or tail node
				{
					previous->next = current->next; // skip the current node
					current = current->next; // advance current to the next node after the one being removed
				}
				free(temp);
				st->nPorts--;
				return 1; // Return 1 to indicate a port was removed
			}
			else // if the port is not out of order, just advance current
			{
				printf("Port %d is not out of order.\n", current->num);
				return 0;
			}
		}
		else
		{
			previous = current; // Save the current node as previous
			current = current->next; // Move to the next port
		}
	}
}

int remOutOrderPort(Station* st_db)
{
	if (st_db == NULL)
	{
		printf("No stations available.\n");
		return; // No stations to remove ports from
	}
	//get user station
	printf("Enter Station ID or Name: ");
	Station* st_u = get_user_station(st_db);//get user station by ID
	if (st_u == NULL) {
		printf("Station not found.\n");
		return; // Station not found
	}
	int st_choise[] = st_choise_des; // id,name,nPorts,coord,nCars,port_list,car_que_num,carQueue
	int port_choise[] = st_port_des; // num,type,status,p2car,tin,next
	print_station(st_u, st_choise, port_choise, NULL); // Print the station details
	printf("which port do you want to delete?(0 to exit):\n");
	int port_num = get_user_port_num();
	if (port_num == 0) { printf("exiting....\n");getchar(); wait_for_user(); printf("\n\n\n"); return 0; } // If user chose to exit
	int result = remove_out_of_order_port(st_u, port_num); // Remove the out of order port
	if (result == 1) {
		printf("Port %d has been removed successfully from station %s.\n", port_num, st_u->name);
	}
	else
	{
		printf("Port %d is not out of order or does not exist in station %s.\n", port_num, st_u->name);
	}

}







//func 12: Remove Customer
void remove_customer(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	Car* car_to_remove;
	char* license_plate_f;
	//get user license plate
	do
	{
		printf("Enter the license plate of the car to remove (or 0 to exit): ");
		char* license_plate = get_user_nLisence();
		if (license_plate == NULL) {
			//error code: 
			return;
		}
		if( license_plate[7] == '0') // User chose to exit
		{
			printf("Exiting...\n");
			wait_for_user();
			free(license_plate);
			return;
		}
		car_to_remove = find_car(db_holder->car_db, license_plate);
		if (!car_to_remove) {
			printf("Car with license plate %s does not exist.\n", license_plate);
			free(license_plate);
		}
		license_plate_f = license_plate; // Store the license plate for later use
	} while (!car_to_remove);
	{
		printf("Car with license plate %s found.\n", car_to_remove->nLicense);
		remove_from_tCar(db_holder->car_db, license_plate_f); // Remove the car from the tCar database
	}
	
}


//func 13: Close station
void close_station(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	//get user station ID 
	printf("Enter Station ID: ");
	Station* st = get_user_station(db_holder->st_db);
	if (!st)
	{
		//st wasnt found
		return;
	}
	int st_choise[] = st_choise_des; // id,name,nPorts,coord,nCars,port_list,car_que_num,carQueue
	print_station(st, st_choise, NULL, NULL); // Print station details
	db_holder->st_db = free_st_rec(db_holder->st_db,st); // Close the station recursively
}