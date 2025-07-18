#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lib.h"
#include "port.h"
#include "binery.h"
#include "inputer.h"
#include "print_lib.h"

//create a empty port
Port* createPort(int num, portType type) {
	Port* newPort = (Port*)malloc(sizeof(Port));
	if (!newPort) {
		//error code:
		fprintf(stderr, "Memory allocation failed for Port\n");
		return NULL;
	}
	newPort->num = num;
	newPort->type = type;
	newPort->status = 2; // Initially free
	newPort->p2car = NULL;
	newPort->next = NULL;
	return newPort;
}

//get test base the type of port and switch it to the right mode
portType which_mod_port(const char* mod) {
	if (strcmp(mod, "FAST") == 0) {
		return FAST;
	}
	else if (strcmp(mod, "MID") == 0) {
		return MID;
	}
	else if (strcmp(mod, "SLOW") == 0) {
		return SLOW;
	}
	else {
		//error code:
		fprintf(stderr, "Unknown port type: %s\n", mod);
		return SLOW; // Default to SLOW if unknown
	}
}

//get raw data from the file (easier to work with data in memory then on file)
raw_db_port *getRowData_Port_from_file(char const* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		//error code:
		fprintf(stderr, "Could not open file %s\n", filename);
		return NULL;
	}
	char buffer[256];
	raw_db_port* head = NULL;
	fgets(buffer, sizeof(buffer), file); // Skip header line
	if (feof(file))
	{
		//error code:
		fclose(file);
		return NULL; // No data in file
	}
	while (fgets(buffer, sizeof(buffer), file)) {
		char type[10];
		char license[10];
		raw_db_port*port = malloc(sizeof(raw_db_port));
		//10 inputs
		short check;
		check = sscanf(buffer, "%d,%d,%10[^,],%d,%hd,%hd,%hd,%hd,%hd,%10[^,]", &port->station_id,&port->port_num,&type,&port->status,&port->tin.Year, &port->tin.Month, &port->tin.Day, &port->tin.Hour, &port->tin.Min,&license);
		if (check != 10) {
			//error code:
			fprintf(stderr, "Error reading line: %s\n", buffer);
			free(port);
			continue; // Skip this line if it doesn't match the expected format
		}
		port->type = which_mod_port(type);
		port->next = NULL;
		strcpy(port->nLicense, license);
		raw_db_port* current = head;
		if (current == NULL) {
			head = port; // If list is empty, set head to new port
			continue;
		}
		else 
		{
			while (current->next)
			{
				current = current->next;
			}
			current->next = port;
		}
	}
	return head;
}

//print db raw as a format to check for debug
void print_raw_db_port(raw_db_port* head) {
	raw_db_port* current = head;
	while (current) {
		printf("Station ID: %d, Port Num: %d, Type: %d, Status: %d, Time In: %04d-%02d-%02d %02d:%02d, License: %s\n",
			current->station_id, current->port_num, current->type, current->status,
			current->tin.Year, current->tin.Month, current->tin.Day,
			current->tin.Hour, current->tin.Min, current->nLicense);
		current = current->next;
	}
}

//get raw queue from file
raw_que** get_raw_que_from_file(char const* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		//error code:
		fprintf(stderr, "Could not open file %s\n", filename);
		return NULL;
	}
	char buffer[256];
	fgets(buffer, sizeof(buffer), file); // Skip header line
	if (feof(file)) {
		//error code:
		fclose(file);
		return NULL; // No data in file
	}
	raw_que** head = (raw_que**)malloc(sizeof(raw_que**));
	char nLicense[10];
	nLicense[sizeof(nLicense) - 1] = '\0';  // Force null-termination
	int i = 0;
	while ((fgets(buffer, sizeof(buffer), file)) != NULL)
	{
		head = (raw_que**)realloc(head, sizeof(raw_que**) * (i + 1));
		//make real good realloc system
		if (!head) {
			//error code:
			fprintf(stderr, "Memory allocation failed for raw_que array\n");
			fclose(file);
			return NULL;
		}
		head[i + 1] = NULL; // Null-terminate the array
		raw_que* que = malloc(sizeof(raw_que));
		if (que == NULL) {
			//error code:
			fprintf(stderr, "Memory allocation failed for raw_que\n");
			fclose(file);
			return NULL;
		}

		sscanf(buffer, "%10[^,],%d", &nLicense,&que->id);
		strcpy(que->nLicense, nLicense);
		head[i] = que;
		i++;
	}
	return head;
}

//DEBUG
void print_raw_que(raw_que** head) {
	if (head == NULL) {
		printf("Queue is empty.\n");
		return;
	}
	for (int i = 0; head[i] != NULL; i++) {
		printf("ID: %d, License: %s\n", head[i]->id, head[i]->nLicense);
	}
}

//add ports to the station database-->loadFiles
void add_ports_to_stations(Station* st_db, raw_db_port* port_data,tCar *car_db) {
	if (st_db == NULL || port_data == NULL) {
		//error code:
		return;
	}
	st_db->portList = linker_port(st_db->id, port_data,car_db); // Link ports to the station
	add_ports_to_stations(st_db->left, port_data,car_db); // Recursively add ports to left subtree
	add_ports_to_stations(st_db->right, port_data,car_db); // Recursively add ports to right subtree
}

//assignCar2port func --> add_ports_to_stations --> laodFiles
Port* linker_port(int id, raw_db_port* port_data,tCar *car_db) 
{
	if (port_data == NULL) {
		//error code:
		fprintf(stderr, "Port data is NULL.\n");
		return NULL;
	}
	raw_db_port* found_port = find_port_by_id(port_data, id);
	if (found_port == NULL) {
		fprintf(stderr, "Port with ID %d not found.\n", id);
		return NULL;
	}
	Port* head = NULL;
	while (found_port->station_id == id)
	{
		Port* newPort = createPort(found_port->port_num, found_port->type);
		newPort->status = found_port->status;
		if (strcmp(found_port->nLicense, "-1") == 0)
		{ 
			newPort->p2car = NULL; 
		}
		else {
			trim_newline(found_port->nLicense); // Remove newline character if present
			Car *ptcar = find_car(car_db, found_port->nLicense);
			//car was found - add the car to the port and port to the car
			newPort->p2car = ptcar; // Assuming find_car returns a pointer to Car struct
			if (ptcar != NULL)
			{
				ptcar->pPort = newPort; // Link the car to the port
			}
			
		}
		newPort->next = NULL;
		newPort->tin = found_port->tin;
		if (head == NULL)
		{
			head = newPort;
		}
		else 
		{
			Port* temp = head;
			while (temp->next != NULL) {
				temp = temp->next; // Traverse to the end of the list
			}
			temp->next = newPort; // Link the new port at the end
		}
		found_port = found_port->next; // Move to the next port in the linked list
		if (found_port == NULL)
		{
			break;
		}
	}
	return head; // Return the linked list of ports
}

//find_port_by_id fanction --> linker_port --> add_ports_to_stations --> laodFiles
raw_db_port* find_port_by_id(raw_db_port* head, int id) {
	raw_db_port* current = head;
	while (current != NULL) {
		if (current->station_id == id) {
			return current; // Found the port with the given ID
		}
		current = current->next;
	}
	return NULL; // Port not found
}

//free the port list
void free_port_list(Port* head) {
	Port* current = head;
	while (current != NULL) {
		Port* temp = current;
		current = current->next;
		free(temp);
	}
}

//free the raw db port list
void free_raw_db_port(raw_db_port* head) {
	raw_db_port* current = head;
	while (current != NULL) {
		raw_db_port* temp = current;
		current = current->next;
		free(temp);
	}
}


void add_que_to_st(Station* st_db, tCar* car_db, raw_que** head) {
	if (st_db == NULL)
	{
		return ;
	}
	add_que_to_st(st_db->left, car_db, head); // Recursively add to left subtree
	add_que_to_st(st_db->right, car_db, head); // Recursively add to right subtree
	int i = 0;
	while (head[i] != NULL)
	{
		if (head[i]->id == st_db->id) // Check if the queue belongs to the current station
		{
			Car* newCar = find_car(car_db, head[i]->nLicense); // Find the car in the car database
			carNode* new_car_node = (carNode*)malloc(sizeof(carNode));
			if (new_car_node == NULL) {
				//error code:
				fprintf(stderr, "Memory allocation failed for carNode\n");
				return;
			}
			new_car_node->car = newCar;
			new_car_node->next = NULL; // Initialize the next pointer to NULL

			if (st_db->carQueue.front == NULL)
			{
				st_db->carQueue.front = new_car_node; // If the queue is empty, set both front and rear to the new car
				st_db->carQueue.rear = new_car_node; // If the queue is empty, set both front and rear to the new car
			}
			else
			{
				st_db->carQueue.rear->next = new_car_node; // Link the new car node at the end of the queue
				st_db->carQueue.rear = new_car_node; // Update the rear pointer to the new car node
			}
		}
		i++;
	}
}

int port_occupid(Station* st)
{
	if (st == NULL) {
		//error code:
		return 0; // No ports occupied if station is NULL
	}
	int count = 0;
	Port* current = st->portList;
	while (current != NULL) {
		if (current->status == 1) { // Check if the port is occupied
			count++;
		}
		current = current->next;
	}
	return count; // Return the count of occupied ports	
}

void write_que_to_file(Station* st_db, const char* file_name) 
{
	FILE* pf = fopen(file_name, "w");
	if (pf == NULL) {
		//error code:
		fprintf(stderr, "Could not open file %s for writing\n", file_name);
		return;
	}
	fprintf(pf, "License,StationID\n"); // Write header
	write_que_to_file_rec(st_db,pf);
	fclose(pf);
}

void write_que_to_file_rec(Station* st_db, FILE* pf) 
{
	if (st_db == NULL) {
		return; // Base case: if the list is empty, return
	}
	write_que_list_to_file(st_db->carQueue.front,st_db->id, pf); // Write the queue of the current station to file
	write_que_to_file_rec(st_db->left, pf); // Recursively write to left subtree
	write_que_to_file_rec(st_db->right, pf); // Recursively write to right subtree
}

void write_que_list_to_file(carNode* head,int id, FILE* pf)
{
	if (head == NULL || pf == NULL) {
		return;
	}
	while (head)
	{
		fprintf(pf, "%s,%d\n", head->car->nLicense, id); // Write car license and station ID to file
		head = head->next; // Move to the next car in the queue
	}
}

void write_port_to_file(Station* st_db, const char* file_name)
{
	FILE* pf = fopen(file_name, "w");
	if (pf == NULL) {
		//error code:
		fprintf(stderr, "Could not open file %s for writing\n", file_name);
		return;
	}
	fprintf(pf, "StationID,PortNumber,PortType,Status,Year,Month,Day,Hour,Min,CarLicense\n"); // Write header
	write_port_to_file_rec(st_db, pf);
	fclose(pf);
}

void write_port_to_file_rec(Station* st_db, FILE* pf)
{
	if (st_db == NULL) {
		return; // Base case: if the list is empty, return
	}
	write_port_list_to_file(st_db->portList, st_db->id, pf); // Write the ports of the current station to file
	write_port_to_file_rec(st_db->left, pf); // Recursively write to left subtree
	write_port_to_file_rec(st_db->right, pf); // Recursively write to right subtree
}

void write_port_list_to_file(Port* head, int id, FILE* pf)
{
	if (head == NULL || pf == NULL) {
		return;
	}
	while (head)
	{
		fprintf(pf, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n", id, head->num, port_type_to_string(head->type), head->status,
			head->tin.Year, head->tin.Month, head->tin.Day, head->tin.Hour, head->tin.Min,
			head->p2car ? head->p2car->nLicense : "-1"); // Write port details to file
		head = head->next; // Move to the next port in the list
	}
}