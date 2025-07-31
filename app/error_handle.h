#ifndef _ERROR_H_H
#define _ERROR_H_H

typedef struct {
	unsigned int error_code;
	const char* error_message;
	Date error_date;
} Error;

void error_code(Error** error_list, unsigned int code);
void print_error(Error* error);
Error* create_error_list();
Error* create_error(unsigned int code);
Error* add_error_to_list(Error** head, Error* new_error);
void print_error_list(Error** head);
void print_last_error(Error* head);
const char* my_strerror(int code);
void test_error_list(Error** error_list);
void print_errno();
void log_error(int error,char* const error_str);

#endif