#ifndef _INPUTER_H
#define _INPUTER_H

#include "lib.h"
#include "inputer.h"
//get user input string

char *get_user_string();
//get Data from time.h

Date getCurrentDate();
//print Date for testing

//get Date from user

Date get_user_Data();
//make sute the value is in the range

Station* get_user_station(Station* st_db);

int is_valid(int val, int max, int min);

//get user coordinates in struct coord(fl x,fl y)
coord get_user_coord();
//get user input for car id
char* get_user_nLisence();

void trim_newline(char* str);

void wait_for_user();

void clean_stdin();

unsigned get_user_st_ID();

tCar* get_user_new_car();

int get_user_port_num();

unsigned* turn_string_to_us_int(const char* str);

unsigned int get_charge_min(Date a, Date b);

void delete_last_two_lines(const char* filename);

portType get_port_type_from_user();
#endif