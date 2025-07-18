#ifndef _ERROR_H_H
#define _ERROR_H_H

typedef struct {
	unsigned int error_code;
	const char* error_message;
	Date error_date;
} Error;


void print_error(Error* error);
Error* create_error_list();
Error* create_error(unsigned int code);
Error* add_error_to_list(Error** head, Error* new_error);
void print_error_list(Error** head);
void print_last_error(Error** head);
const char* my_strerror(int code);

#endif