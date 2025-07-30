#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "lib.h"
#include "binery.h"
#include "port.h"
#include "print_lib.h"
#include "inputer.h"

//---------station tree func----------
// 
//read one station from file (one line) --> read_st_from_file
Station* read_s_one_f_f(FILE *pf)
{
	char buffer[256];
	fgets(buffer, sizeof(buffer), pf); 
	Station* station = malloc(sizeof(Station));
	if (station == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return 1;
	}
	char name_b[200];
	sscanf(buffer, "%u,%99[^,],%d,%lf,%lf", &station->id, name_b, &station->nPorts, &station->coord[0], &station->coord[1]);
	station->name = malloc(strlen(name_b) + 1);
	if (station->name == NULL) {
		fprintf(stderr, "Memory allocation failed for station name\n");
		free(station);
		return NULL;
	}
	station->nCars = 0; // Initialize nCars to 0
	station->carQueue.front = NULL; // Initialize front of the queue to NULL
	station->carQueue.rear = NULL; // Initialize rear of the queue to NULL
	station->left = NULL;
	station->right = NULL;
	//copy the name from buffer to station->name
	strcpy(station->name, name_b);
	
	return station;
}

//function to read all stations from file 
Station* read_st_from_file(char const *file_name)
{
	FILE* pf = fopen(file_name, "r");
	if (pf == NULL) {
		//error code:
		perror("fopen");  // Prints system error
		return NULL;
	}
	Station* head = NULL;
	// Read until end of file
	char buffer[256];
	fgets(buffer, sizeof(buffer), pf); // Skip header line
	if (feof(pf)) { return 0; }
	do{
		Station* station = read_s_one_f_f(pf);
		if (station == NULL) {
			//error code:
			continue; // skip bad lines
		}
		if (feof(pf)) { break; }
		head = add_to_stationDB(head, station);

	} while (!feof(pf));
	return head;
}

//add a new station to the station database (binary tree) --> read_st_from_file
Station* add_to_stationDB(Station* head, Station* new_station) {
	if (head == NULL) {
		return new_station;
	}

	if (head->id < new_station->id)
	{
		head->right = add_to_stationDB(head->right, new_station);
		return head;
	}
	else
	{
		head->left = add_to_stationDB(head->left, new_station);
		return head;
	}
}

//create a new station with given parameters
Station* create_station(unsigned id, const char* name, int nPorts, double coordX, double coordY) {
	Station* s = malloc(sizeof(Station));
	if (!s) {
		//error code:
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	s->id = id;
	s->name = malloc(strlen(name) + 1);
	if (s->name) {
		strcpy(s->name, name);
	}
	s->nPorts = nPorts;
	s->coord[0] = coordX;
	s->coord[1] = coordY;
	s->nCars = 0;
	s->left = NULL;
	s->right = NULL;
	s->carQueue.front = NULL; // Initialize front of the queue to NULL
	s->carQueue.rear = NULL; // Initialize rear of the queue to NULL
	return s;
}

//searchStation func
Station* find_station_by_id(Station* head, unsigned id)
{
	if (head == NULL) {
		return NULL;
	}
	if (head->id == id) {
		return head;
	}
	else if (head->id < id) {
		return find_station_by_id(head->right, id);
	}
	else {
		return find_station_by_id(head->left, id);
	}
}

Station* find_station_by_name(Station* head, const char* name)
{
	if (head == NULL) {
		return NULL; // Base case: if the station is NULL, return NULL
	}
	if (strcmp(head->name, name) == 0) {
		return head; // Found the station by name
	}
	// Search in left and right subtrees
	Station* left_result = find_station_by_name(head->left, name);
	if (left_result != NULL) {
		return left_result; // Found in left subtree
	}
	return find_station_by_name(head->right, name); // Search in right subtree
}

Station* find_station_by_car(Station* head, const char* n_lic)
{
	if (head == NULL) {
		return NULL; // Base case: if the station is NULL, return NULL
	}
	Port* current_port = head->portList;
	while (current_port != NULL) {
		if (current_port->p2car != NULL && strcmp(current_port->p2car->nLicense, n_lic) == 0) {
			return head; // Found the car in this station
		}
		current_port = current_port->next;
	}
	carNode* current_car = head->carQueue.front;
	while (current_car != NULL) {
		if (strcmp(current_car->car->nLicense, n_lic) == 0) {
			return head; // Found the car in the queue of this station
		}
		current_car = current_car->next;
	}
	// Search in left and right subtrees
	Station* left_result = find_station_by_car(head->left, n_lic);
	if (left_result != NULL) {
		return left_result; // Found in left subtree
	}
	return find_station_by_car(head->right, n_lic); // Search in right subtree
}



//check the number of cars in each station
void check_number_of_cars(Station* st_db)
{
	if (st_db == NULL) {
		return; // Base case: if the station is NULL, return
	}
	//send left send right
	check_number_of_cars(st_db->left);
	check_number_of_cars(st_db->right);
	//init
	st_db->nCars = 0;
	if (!st_db->portList)
	{
		return;
	}
	Port* current_port = st_db->portList;
	while (current_port != NULL) {
		if (current_port->p2car != NULL) {
			st_db->nCars++;
		}
		current_port = current_port->next;
	}
}

//check the number of ports in each station
void check_port_count(Station* st_db)
{
	if (st_db == NULL) {
		return; // Base case: if the station is NULL, return
	}
	//send left send right
	check_port_count(st_db->left);
	check_port_count(st_db->right);
	//init
	st_db->nPorts = 0;
	Port* current_port = st_db->portList;
	while (current_port != NULL) {
		st_db->nPorts++;
		current_port = current_port->next;
	}
}

void write_st_to_file(Station* st_db, char* file_name)
{
	FILE* pf = fopen(file_name, "w");
	if (pf == NULL) {
		//error code:
		return;
	}
	fprintf(pf, "ID,StationName,NumOfPorts,CoordX,CoordY\n"); // Write header
	write_st_to_file_rec(st_db, pf);
	fclose(pf);
}

void write_st_to_file_rec(Station* st_db, FILE* pf)
{
	if (st_db == NULL) {
		return; // Base case: if the station is NULL, return
	}
	fprintf(pf, "%u,%s,%d,%2.2lf,%2.2lf\n", st_db->id, st_db->name, st_db->nPorts, st_db->coord[0], st_db->coord[1]);
	write_st_to_file_rec(st_db->left, pf); // Write left subtree
	write_st_to_file_rec(st_db->right, pf); // Write right subtree
}


//-----free the station tree-----
void free_station(Station* st_db) 
{
	if (st_db == NULL) {
		return; // Base case: if the station is NULL, return
	}
	free(st_db->name); // Free the name string
	free_port_list(st_db->portList); // Free the port list
	free_car_queue(&st_db->carQueue); // Free the car queue
	free(st_db); // Free the station itself
}

void free_st_db(Station* st_db) 
{
	if (st_db == NULL) {
		return; // Base case: if the station is NULL, return
	}
	free_st_db(st_db->left); // Free left subtree
	free_st_db(st_db->right); // Free right subtree
	free_station(st_db); // Free the station itself
}

void free_car_queue(qCar* queue) 
{
	if (queue == NULL) {
		return; // Base case: if the queue is NULL, return
	}
	carNode* current = queue->front;
	while (current != NULL) {
		carNode* temp = current;
		current = current->next;
		free(temp); // Free each car in the queue
	}
	queue->front = NULL; // Set front to NULL after freeing
	queue->rear = NULL; // Set rear to NULL after freeing
}

Station* free_st_rec(Station* st_db,Station* st) 
{
	if(st_db == NULL) {
		return NULL; // Base case: if the station is NULL, return
	}
	if(st_db->id == st->id) {
		if (st_db->left == NULL && st_db->right == NULL) {
			free_station(st_db);
			return NULL; // Leaf node
		}
		else if (st_db->left == NULL || st_db->right == NULL) {
			Station* temp = (st_db->left) ? st_db->left : st_db->right;
			free_station(st_db);
			return temp; // One child
		}
		else {
			// Two children: find the in-order successor
			Station* successor = st_db->right;
			while (successor && successor->left != NULL) {
				successor = successor->left;
			}
			// Clean up old dynamically allocated fields
			free(st_db->name);

			// Shallow copy all successor data
			*st_db = *successor;

			// Deep copy the name
			char* new_name = malloc(strlen(successor->name) + 1);
			strcpy(new_name, successor->name);
			st_db->name = new_name;

			st_db->right = free_st_rec(st_db->right, successor);
			return st_db;

		}
		st_db->left = free_st_rec(st_db->left, st);
		st_db->right = free_st_rec(st_db->right, st);
		return st_db; // Exit the function after freeing
	}
}

//--------car_db & tCar: func---------
// 
//get row data from file --> get_tCar_from_file
car_db* get_db_car_from_file(FILE* pf)
{
	char buffer[256];
	car_db* head = malloc(sizeof(car_db));
	if (head == NULL) {
		//error code:
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	fgets(buffer, sizeof(buffer), pf); // Skip header line
	if (feof(pf)) {
		//error code:
		free(head);
		return NULL; // Return NULL if file is empty or only has header
	}
	fgets(buffer, sizeof(buffer), pf); // Read first line of data
	char n_ls[10];
	n_ls[sizeof(n_ls) - 1] = '\0';  // Force null-termination
	char mod[5];
	sscanf(buffer, "%10[^,],%4[^,],%lf,%d,%d,%d", &n_ls, &mod, &head->totalPayed, &head->st_id,&head->port,&head->inqueue);
	strcpy(head->nLicense, n_ls);
	head->type = which_port_type_st(mod);
	head->next = NULL; // Initialize the next pointer to NULL
	if (feof(pf)) {
		free(head);
		return NULL; // Return NULL if only one line of data
	}
	while (fgets(buffer, sizeof(buffer), pf)) 
	{
		car_db* new_car = malloc(sizeof(car_db));
		if (!new_car) {
			fprintf(stderr, "Memory allocation failed for new car\n");
			free(head);
			return NULL;
		}
		sscanf(buffer, "%10[^,],%4[^,],%lf,%d,%d,%d", &n_ls, &mod,
			&new_car->totalPayed, &new_car->st_id, &new_car->port, &new_car->inqueue);
		strcpy(new_car->nLicense, n_ls);
		new_car->type = which_port_type_st(mod);
		new_car->next = NULL;
		insert_car_db(head, new_car);
	}

	return head;
}

//insert a new car into the car_db linked list (car_db is a linked list)
void insert_car_db(car_db* head, car_db* new_car) {
	if (head == NULL) {
		return new_car;
	}
	while (head->next != NULL) {
		head = head->next;
	}
	head->next = new_car;
}

//add a new car from car_db to tCar binary tree (tCar is a binary tree of cars) --> turb_db_car_to_tree
tCar* add_car_db_to_tree(car_db *db,tCar *head) {
	tCar* new_tCar = malloc(sizeof(tCar));
	if (new_tCar == NULL) {
		//error code:
		fprintf(stderr, "Memory allocation failed for tCar\n");
		return NULL;
	}
	Car* new_car = malloc(sizeof(Car));
	if (new_car == NULL) {
		//error code:
		fprintf(stderr, "Memory allocation failed for Car\n");
		free(new_tCar);
		return NULL;
	}
	new_car->inqueue = db->inqueue;
	new_car->pPort = NULL; // Assuming pPort is not set here, adjust as needed
	new_car->type = db->type;
	new_car->totalPayed = db->totalPayed;
	strcpy(new_car->nLicense, db->nLicense);
	new_tCar->car = new_car;
	new_tCar->left = NULL;
	new_tCar->right = NULL;
	head = rec_add_to_tree(head, new_tCar);
	return head;
}

// Recursive function to add a new tCar to the binary tree --> add_car_db_to_tree --> turb_db_car_to_tree
tCar* rec_add_to_tree(tCar* head, tCar* new_tCar) {
	if (head == NULL) {
		return new_tCar;
	}
	if (strcmp(new_tCar->car->nLicense, head->car->nLicense) < 0) {
		head->left = rec_add_to_tree(head->left, new_tCar);
	}
	else {
		head->right = rec_add_to_tree(head->right, new_tCar);
	}
	return head;
}

//remove a car from tCar binary tree by license number
tCar* remove_from_tCar(tCar* head, const char* nLicense) {
	if (head == NULL) {
		return NULL; // Not found
	}

	if (strcmp(nLicense, head->car->nLicense) < 0) {
		head->left = remove_from_tCar(head->left, nLicense);
	}
	else if (strcmp(nLicense, head->car->nLicense) > 0) {
		head->right = remove_from_tCar(head->right, nLicense);
	}
	else {
		// Node to be deleted found
		if (head->left == NULL && head->right == NULL) {
			free(head->car);
			free(head);
			return NULL; // Leaf node
		}
		else if (head->left == NULL || head->right == NULL) {
			tCar* temp = (head->left) ? head->left : head->right; 
			free(head->car);
			free(head);
			return temp; // One child
		}
		else {
			// Two children: find the in-order successor
			tCar* successor = head->right;
			while (successor && successor->left != NULL) {
				successor = successor->left;
			}
			strcpy(head->car->nLicense, successor->car->nLicense); // Copy successor's data
			head->right = remove_from_tCar(head->right, successor->car->nLicense); // Remove successor
		}
	}
	return head;
}

void free_tCar_db(tCar* head) 
{
	if (!head)
	{
		//error code:
		return;
	}
	free_tCar_db(head->left);
	free_tCar_db(head->right);
	if (head->car) {
		free(head->car); // Free the Car structure
	}
	free(head); // Free the tCar node itself
}

// Initialize a new tCar node
tCar* init_tCar() {
	tCar* head = malloc(sizeof(tCar));
	if (head == NULL) {
		//error code:
		fprintf(stderr, "Memory allocation failed for tCar\n");
		return NULL;
	}
	head->car = NULL; // Initialize car pointer to NULL
	head->left = NULL; // Initialize left child to NULL
	head->right = NULL; // Initialize right child to NULL
	return head;
}

//turn the car_db linked list to a tCar binary tree --> get_tCar_from_file
tCar* turn_db_car_to_tree(car_db* db) {
	tCar* tree_head = NULL;
	while (db != NULL) {
		tCar* added = add_car_db_to_tree(db, tree_head);
		if (tree_head == NULL) {
			tree_head = added;
		}
		db = db->next;
	}
	return tree_head;
}

//get tCar from file
tCar* get_tCar_from_file(char const *file_name)
{
	FILE* pf = fopen(file_name, "r");
	if (!pf) {
		//error code:
		printf("memo prob!!!");
		exit(1);
	}

	// Load car database from file
	car_db* db_head = get_db_car_from_file(pf);
	
	//--dubug-tools--
	//printf("Car database loaded successfully.\n");
	//print_db_car(db_head);

	//  Initialize tree head to NULL
	tCar* tree_head = NULL;

	//  Add a few cars from db to tree for testing
	tree_head = turn_db_car_to_tree(db_head);

	//free the car_db linked list
	free_car_linked_list(db_head);
	fclose(pf);
	return tree_head;
}

//free the car_db linked list --> get_tCar_from_file
void free_car_linked_list(car_db* head) 
{
	car_db* temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

//searchCar func
Car* find_car(tCar* car_db, char* nlis) 
{
	if (car_db == NULL) {
		return NULL; // Tree is empty
	}
	if (strcmp(nlis, car_db->car->nLicense) == 0)
	{
		return car_db->car; // Found the car
	}
	if (strcmp(nlis, car_db->car->nLicense) < 0) {
		return find_car(car_db->left, nlis); // Search in left subtree
	}
	else if (strcmp(nlis, car_db->car->nLicense) > 0) {
		return find_car(car_db->right, nlis); // Search in right subtree
	}
}

//testing function for find_car
void test_find_car() {
	// Create mock cars
	Car* car1 = (Car*)malloc(sizeof(Car));
	Car* car2 = (Car*)malloc(sizeof(Car));
	Car* car3 = (Car*)malloc(sizeof(Car));

	strcpy(car1->nLicense, "123456789");
	strcpy(car2->nLicense, "123867368");
	strcpy(car3->nLicense, "389693758");

	// Create tree nodes
	tCar* root = (tCar*)malloc(sizeof(tCar));
	tCar* left = (tCar*)malloc(sizeof(tCar));
	tCar* right = (tCar*)malloc(sizeof(tCar));

	// Assign structure
	root->car = car2;
	root->left = left;
	root->right = right;

	left->car = car1;
	left->left = NULL;
	left->right = NULL;

	right->car = car3;
	right->left = NULL;
	right->right = NULL;

	// Test search
	char* license_to_find = "123456789";
	Car* found = find_car(root, license_to_find);

	if (found != NULL) {
		printf("Found car with license: %s\n", found->nLicense);
	}
	else {
		printf("Car not found.\n");
	}

	// Clean up
	free(car1); free(car2); free(car3);
	free(left); free(right); free(root);
}

//write tCar to file
void write_tCar_to_file(tCar* head,Station* st_db, char const* file_name) 
{
	FILE* pf = fopen(file_name, "w");
	if (pf == NULL) {
		//error code:
		return;
	}
	fprintf(pf, "License,PortType,TotalPayed,StationID,PortNumber,InQueue\n"); // Write header
	write_tCar_to_file_rec(head,st_db ,pf);
	//delete_last_two_lines(pf); // Remove the last two lines (empty line and header)
	fclose(pf);
}

void write_tCar_to_file_rec(tCar* head,Station *st_db, FILE* pf)
{
	if (head == NULL) {
		return; // Base case: if the tCar is NULL, return
	}
	int id = find_st_id_by_licanse(st_db, head->car->nLicense);
	int num = 0;
	if (head->car->pPort)
	{
		num = head->car->pPort->num;
	}
	fprintf(pf, "%s,%s,%.2lf,%d,%d,%d\n", head->car->nLicense, port_type_to_string(head->car->type), head->car->totalPayed,
		id, num, head->car->inqueue);
	write_tCar_to_file_rec(head->left,st_db, pf); // Write left subtree
	write_tCar_to_file_rec(head->right,st_db, pf); // Write right subtree
}

//find the station ID by license number
int find_st_id_by_licanse(Station* st_db, char const* license) 
{
	if (st_db == NULL) {
		return 0; // Not found
	}
	Port* current_port = st_db->portList;
	while (current_port != NULL) {
		if (current_port->p2car != NULL && strcmp(current_port->p2car->nLicense, license) == 0) {
			return st_db->id; // Found the car in this station
		}
		current_port = current_port->next;
	}
	// Search in left and right subtrees
	int left_result = find_st_id_by_licanse(st_db->left, license);
	if (left_result != 0) {
		return left_result; // Found in left subtree
	}
	return find_st_id_by_licanse(st_db->right, license); // Search in right subtree
}

