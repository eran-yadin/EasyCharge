#pragma once
#ifndef _print_lib_app_H
#define _print_lib_app_H

void print_menu();
void print_ALL_DB(Station* st_db, int st_choise[7], int port_choise[5], int car_choise[5]);
void print_que_in_station(Station* st_db);
void print_db_car(car_db* head);
void print_nLicense_in_tree(tCar* root);
Station* print_station_by_id(Station* head);
void print_port_in_st(Station* st);
void print_port_in_st_only_working(Station* st);
void print_ALL_ports(Station* st_db);
void print_coord(coord c);
char* port_type_to_string(portType port);
void showOpening();
void printDate(Date d);
void print_port(Port* port_p, int port_choise[6], int car_choise[5]);
void print_car(Car* car_p, int car_choise[5], int port_choise[5]);
void print_station(Station* st, int st_choise[8], int port_choise[5], int car_choise[5]);

#endif // !_print_lib_app_H