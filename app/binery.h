#ifndef _binery_app_H
#define _binery_app_H

//-----station tree func-----
Station* read_s_one_f_f(FILE *pf);

Station* read_st_from_file(char const *file_name);

Station* add_to_stationDB(Station* head, Station* new_station);

Station* create_station(unsigned id, const char* name, int nPorts, double coordX, double coordY);

Station* find_station_by_id(Station* head, unsigned id);

void write_st_to_file(Station* st_db, char* file_name);

void write_st_to_file_rec(Station* st_db, FILE* pf);

void free_station(Station* st_db);

void free_st_db(Station* st_db);

void free_car_queue(qCar* queue);

//--------car_db & tCar: func---------
car_db* get_db_car_from_file(FILE* pf);

void insert_car_db(car_db* head, car_db* new_car);

tCar* add_car_db_to_tree(car_db* db, tCar* head);

tCar* rec_add_to_tree(tCar* head, tCar* new_tCar);

tCar* remove_from_tCar(tCar* head, const char* nLicense);

void free_tCar_db(tCar* head);

tCar* turn_db_car_to_tree(car_db* db);

tCar* get_tCar_from_file(char const *file_name);

void free_car_linked_list(car_db* head);

tCar* init_tCar();

Car* find_car(tCar* car_db, char* nlis);

void test_find_car();

void check_number_of_cars(Station* st_db);

void check_port_count(Station* st_db);

void write_tCar_to_file(tCar* head, Station* st_db, char const* file_name);

void write_tCar_to_file_rec(tCar* head, Station* st_db, FILE* pf);

int find_st_id_by_licanse(Station* st_db, char const* license);

#endif