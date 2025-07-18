#ifndef _INPUTER_H
#define _INPUTER_H
#include "lib.h"
#include "inputer.h"
//get user input string

char *get_user_string();
//get Data from time.h

Date getCurrentDate();
//print Date for testing

void printDate(Date d);
//get Date from user

Date inputDateFromUser();
//make sute the value is in the range

int is_valid(int val, int max, int min);

//get user coordinates in struct coord(fl x,fl y)
coord get_user_coord();
//get user input for car id
char* get_car_id_from_user();

void trim_newline(char* str);

void wait_for_user();

void clean_stdin();
#endif