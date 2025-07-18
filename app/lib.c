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

//
#define stations_file "Stations.txt"
#define ports_file "Ports.txt"
#define queues_file "LineOfCars.txt"
#define cars_file "Cars.txt"

// Define function codes for menu options
#define FUNC_1_locate_nearest_station 1
#define FUNC_2_ 2
#define FUNC_3_ 3
#define FUNC_4_ 4
#define FUNC_5_ 5
#define FUNC_6_ 6
#define FUNC_7_ 7
#define FUNC_8_ 8
#define FUNC_9_ 9
#define FUNC_10_ 10
#define FUNC_11_ 11
#define FUNC_12_ 12
#define FUNC_13_ 13
#define FUNC_0_exit 0



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
		//error code:
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
		scanf("%d", &decision);
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
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	switch (decision)
	{
	case FUNC_1_locate_nearest_station:
		locNearSt(db_holder->st_db);
		break;
	case FUNC_2_:
		charge_car(db_holder);
		break;
	case FUNC_3_:
		// Implement functionality for case 3
		break;
	case FUNC_4_:
		// Implement functionality for case 4
		break;
	case FUNC_5_:
		// Implement functionality for case 5
		break;
	case FUNC_6_:
		// Implement functionality for case 6
		break;
	case FUNC_7_:
		// Implement functionality for case 7
		break;
	case FUNC_8_:
		// Implement functionality for case 8
		break;
	case FUNC_9_:
		// Implement functionality for case 9
		break;
	case FUNC_10_:
		// Implement functionality for case 10
		break;
	case FUNC_11_:
		
		break;
	case FUNC_12_:
		
		break;
	case FUNC_13_:
		
		break;
	default:
		printf("Exiting the program.\n");
		save_files(db_holder); // Save the database before exiting
		free_DB_holder(db_holder); // Free the database holder
		exit(0);
	}
}

//func 1: locate nearest station
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

//func 2: add new station
void charge_car(DB_holder* db_holder)
{
	// Check if db_holder is NULL
	if (db_holder == NULL) {
		//error code:
		fprintf(stderr, "Database holder is NULL.\n");
		return;
	}
	Station* st = NULL;
	int st_id = 0;

	//get user station ID 
	printf("Enter Station ID: ");
	do
	{
		st_id = get_user_st_ID();
		if (st_id == 0)
		{
			printf("Exiting...\n");
			wait_for_user();
			return;
		}
		st = find_station_by_id(db_holder->st_db, st_id);
		if (!st)
		{
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
			if (choice == 0) {
				printf("Exiting...\n");
				wait_for_user();
				free(license_plate);
				return;
			}
			else if (choice == 1) {
				//add new car
				tcar->car = get_user_new_car();
				//new car
				db_holder->car_db = rec_add_to_tree(db_holder->car_db, tcar);
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
	if (!port || port->status == 3) {
		printf("No ports available for this car type.\n");
		wait_for_user();
		return;
	}
	if (port->status == 2)
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
	else
	{
		carNode* car_node = malloc(sizeof(carNode));
		car_node->car = tcar->car; // Assign the car to the new car node
		st->carQueue.rear->next = car_node; // Link the new car node at the end of the queue
		st->carQueue.rear = car_node; // Update the rear pointer to the new car node
		st->nCars++; // Increment the number of cars in the station
		printf("Car with license plate %s has been added to the queue in station %s.\n",
			tcar->car->nLicense, st->name);
	}
}
