#include <stdio.h>
#include <stdlib.h>

#include "lib.h"
#include "print_lib.h"
#include "inputer.h"

void print_menu()
{
	printf("Menu:\n");
	printf("1.  Locate Closest Station\n");
	printf("2.  Charge Car\n");
	printf("3.  Check Car Statue\n");
	printf("4.  Stop Charge\n");
	printf("5.  Display All Stations\n");
	printf("6.  Dispaly Cars at Station\n");
	printf("7.  Report of Station's statistics\n");
	printf("8.  Display Top Custumers\n");
	printf("9.  Add New Port\n");
	printf("10. Release Charging Port\n");
	printf("11. Remove Out of Order Ports\n");
	printf("12. Remove Customer\n");
	printf("13. Close Station\n");
	printf("0. Exit\n");
	printf("Please choose an option: (0-13): ");
}

void print_ALL_DB(Station* st_db)
{
	if (st_db == NULL) {
		printf("--------------------------\n\n\n\n");
		return;
	}

	printf("Station ID: %d, Name: %s, Ports: %d, Cars: %d\n",
		st_db->id, st_db->name, st_db->nPorts, st_db->nCars);
	print_que_in_station(st_db);
	if (st_db->portList != NULL) {
		Port* current = st_db->portList;
		while (current != NULL) {
			printf("Port Number: %d, Type: %4s, Status: %d\n",
				current->num, port_type_to_string(current->type), current->status);
			current = current->next;
		}
	}
	else {
		printf("No ports available for this station.\n");
	}

	print_ALL_DB(st_db->left);
	print_ALL_DB(st_db->right);
}

void print_que_in_station(Station* st_db)
{
	printf("Cars in Queue at Station ID: %d\n", st_db->id);
	carNode* corrent = st_db->carQueue.front;
	while (corrent != NULL)
	{
		printf("Car License: %s\n", corrent->car->nLicense);
		corrent = corrent->next;
	}
}

void print_db_car(car_db* head)
{
	if (head == NULL) {
		//error code:
		printf("No cars in the database.\n");
		return;
	}
	while (head != NULL) {
		printf("License: %s, Model: %d, Total Payed: %8.2lf, Station ID: %4d, Port: %3d, In Queue: %1d\n",
			head->nLicense, head->type, head->totalPayed, head->st_id, head->port, head->inqueue);
		head = head->next;
	}
}

void print_nLicense_in_tree(tCar* root) 
{
	if (root == NULL)
		return;

	// In-order traversal: left -> root -> right
	print_nLicense_in_tree(root->left);
	printf("License: %s\n", root->car->nLicense);
	print_nLicense_in_tree(root->right);
}

Station* print_station_by_id(Station* head)
{
	if (!head)
	{
		return NULL;//got to the end
	}
	print_station_by_id(head->left); // go to the left side (smaller)
	printf("ID: %u, Name: %s, Ports: %d, Coordinates: (%.2lf, %.2lf)\n",
		head->id, head->name, head->nPorts, head->coord[0], head->coord[1]);
	print_station_by_id(head->right); // go to the right side (bigger)
}

void print_port_in_st(Station* st)
{
	if (st == NULL){
		//error code:
		return;
	}
	Port* current = st->portList;
	while (current)
	{
		printf("Port Num: %d, Type: %4s, Status: %d, License: %s, Time In: %04d-%02d-%02d %02d:%02d\n",
			current->num, port_type_to_string(current->type), current->status,
			current->p2car ? current->p2car->nLicense : "None    ",
			current->tin.Year, current->tin.Month, current->tin.Day,
			current->tin.Hour, current->tin.Min);
		current = current->next;
	}
}


void print_port_in_st_only_working(Station* st)
{
	Port* current = st->portList;
	while (current)
	{
		if (current->status == 1|| current->status == 2) // Only print free or occupied ports (not disabled)
		{
			printf("Port Num: %d, Type: %4s, Status: %d\n",
				current->num, port_type_to_string(current->type), current->status);
		}
		current = current->next;
	}
}

void print_ALL_ports(Station* st_db) 
{
	if (st_db == NULL) {
		//error code:
		return;
	}
	printf("station ID: %d\n", st_db->id);
	print_port_in_st(st_db); // Print ports in the current station
	print_ALL_ports(st_db->left); // Recursively print ports in left subtree
	print_ALL_ports(st_db->right); // Recursively print ports in right subtree
}

void print_coord(coord c)
{
	printf("Coordinates: (%.2lf, %.2lf)\n", c.x, c.y);
}

char *port_type_to_string(portType port)
{
	if (port == FAST)
	{
		return "FAST";
	}
	if (port == MID)
	{
		return "MID";
	}
	if (port == SLOW)
	{
		return "SLOW";
	}
	return NULL;
}

void showOpening() {
	printf("_____                     _____  _                               \n");
	printf("|  ___|                  /  __ \\| |                              \n");
	printf("| |__   __ _  ___  _   _ | /  \\/| |__    __ _  _ __  __ _   ___ \n");
	printf("|  __| / _` |/ __|| | | || |    | '_ \\  / _` || '__|/ _` | / _ \\\n");
	printf("| |___| (_| |\\__ \\| |_| || \\__/\\| | | || (_| || |  | (_| ||  __/\n");
	printf("\\____/ \\__,_||___/ \\__, | \\____/|_| |_| \\__,_||_|   \\__, | \\___|\n");
	printf("                    __/ |                            __/ |      \n");
	printf("                   |___/                            |___/       \n");
}

void printDate(Date d) 
{
	printf("Date: %04d-%02d-%02d %02d:%02d\n", d.Year, d.Month, d.Day, d.Hour, d.Min);
}

void print_port(Port* port_p, int port_choise[6],int car_choise[5])
{
	if (!port_choise)
	{
		return;
	}
	if (port_choise[0])
	{
		printf("port number: %d\n", port_p->num);
	}
	if (port_choise[1])
	{
		printf("port number: %s\n", port_type_to_string(port_p));
	}
	if (port_choise[2])
	{
		printf("port_status: %d\n", port_p->status);
	}
	if (port_choise[3])
	{
		printf("charge time: ");
		
	}
	if (port_choise[4])
	{
		print_car(port_p->p2car, car_choise, 0);
	}
}
//print_choise[nLicense,type,totalPayed,inqueu
void print_car(Car* car_p, int car_choise[5],int port_choise[5])
{

	if (car_choise[0])
	{
		printf("car license: %d\n", car_p->nLicense);
	}
	if (car_choise[1])
	{
		printf("charge type: %s\n", port_type_to_string(car_p->type));
	}
	if (car_choise[2])
	{
		printf("tatal paid: %lf\n",car_p->totalPayed);
	}
	if (car_choise[3])
	{
		if (car_p->inqueue)
		{
			printf("car is inqueue\n");
		}
		else
		{
			printf("car is NOT inqueue\n");
		}
	}
	if (car_choise[4])
	{
		print_port(car_p->pPort, port_choise);
	}
}