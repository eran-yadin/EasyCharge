#pragma once
#ifndef _Port_H_
#define _Port_H_

Port* createPort(int num, portType type);
portType which_port_type_st(const char* mod);
portType which_port_type_int(int p_t);
raw_db_port* getRowData_Port_from_file(char const* filename);
void print_raw_db_port(raw_db_port* head);
raw_que** get_raw_que_from_file(char const* filename);
void print_raw_que(raw_que** head);
void add_ports_to_stations(Station* st_db, raw_db_port* port_data, tCar* car_db);
Port* linker_port(int id, raw_db_port* port_data, tCar* car_db);
raw_db_port* find_port_raw_data_by_id(raw_db_port* head, int id);
void free_port_list(Port* head);
void free_raw_db_port(raw_db_port* head);
void add_que_to_st(Station* st_db, tCar* car_db, raw_que** head);
int port_occupid(Station* st);
void write_que_to_file(Station* st_db, char const* file_name);
void write_que_to_file_rec(Station* st_db, FILE* pf);
void write_que_list_to_file(carNode* head, int id, FILE* pf);
void write_port_to_file(Station* st_db, const char* file_name);
void write_port_to_file_rec(Station* st_db, FILE* pf);
void write_port_list_to_file(Port* head, int id, FILE* pf);
Port* is_port_type_exist(Station* st_db, portType port);

#endif // !_Port_H_