#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lib.h"
#include "inputer.h"
#include "error_handle.h"
#include "print_lib.h"

#define max_error_list 15
/// Error handling module for the application
/// code: [0][1][2][3]
/// [0]: app code /type
/// [1]: place
/// [2][3]: error code

//----------Error codes----------
//error code: 1000-1999 memory allocation errors and DB errors
#define E_1001 "Database failed to load"
#define E_1002 "Memory allocation failed"
#define E_1002	
#define E_1002
#define E_1002
#define E_1002
#define E_1002
#define E_1002
#define E_1002


//error code: 2000-2999 input errors
// 
//----------Error codes----------

void error_code(Error** error_list, unsigned int code)
{
	if (error_list == NULL) {
		fprintf(stderr, "Error list is NULL\n");
		return;
	}
	Error* new_error = create_error(code);
	if (new_error == NULL) {
		fprintf(stderr, "Failed to create new error\n");
		return;
	}
	add_error_to_list(error_list, new_error);
}

void print_error(Error* error)
{
	if (!error)
	{
		return;
	}
	printf("-------------------\n");
	print_Date(error->error_date);
	printf("Error Code: %u\n", error->error_code);
	printf("Error Message: %s\n", error->error_message);
}

Error* create_error_list()
{
	// Allocate memory for the error list
	// If allocation fails, print an error message and return NULL
	Error** head = (Error**)malloc(sizeof(Error*)*max_error_list);
	if (head == NULL) {
		fprintf(stderr, "Memory allocation failed for error list\n");
		return NULL;
	}
	return head;
}

Error* create_error(unsigned int code)
{
	Error* new_error = (Error*)malloc(sizeof(Error));
	if (new_error == NULL) {
		fprintf(stderr, "Memory allocation failed for Error\n");
		return NULL;
	}
	new_error->error_code = code;
	new_error->error_message = my_strerror(code);
	new_error->error_date = getCurrentDate(); // Assuming getCurrentDate() is defined in inputer.c
	return new_error;
}

Error* add_error_to_list(Error** head, Error* new_error)
{
	if (head == NULL || new_error == NULL) {
		fprintf(stderr, "Invalid parameters to add_error_to_list\n");
		return NULL;
	}

	// Find the first empty slot in the error list
	for (int i = max_error_list; i > 0; i--) {
		if (head[i] == NULL) {
			head[i] = new_error;
			return new_error;
		}
	}

	for (int i = max_error_list; i > 0; i--)
	{
		head[i] = head[i-1]; // Shift errors to the right
	}
	head[0] = new_error; // Add the new error at the beginning
	return head[0];
}

void print_error_list(Error** head)
{
	if (head == NULL) {
		fprintf(stderr, "Error list is NULL\n");
		return;
	}
	for (int i = 0; i < max_error_list; i++) {
		if (head[i] != NULL) {
			print_last_error(head[i]);
		}
	}
}

void print_last_error(Error* head)
{
	if (!head)
	{
		return;
	}
	printf("----------last-error---------\n");
	print_Date(head[0].error_date);
	printf("Error Code: %u\n", head[0].error_code);
	printf("Error Message: %s\n", head[0].error_message);
}

const char* my_strerror(int code)
{
    switch (code) {
    case 1001: return E_1001;
	case 1002: return E_1002;
	case 2001: return "Invalid input format";
    default: return "Unknown error";
    }
}

void test_error_list(Error** error_list)
{
	for (size_t i = 0; i < 45; i++)
	{
		error_code(error_list, i);
	}
	print_error_list(error_list);
}

void print_errno()
{
	int err_code = errno; // Get the current error code
	if (err_code != 0) {
		fprintf(stderr, "Error: %s\n", strerror(err_code)); // Print the error message
	} else {
		fprintf(stderr, "No error occurred.\n");
	}
}

void log_error(int error, char* const error_str)
{
	FILE* log_file = fopen("error_log.txt", "a");
	if (log_file == NULL) {
		fprintf(stderr, "Could not open error log file.\n");
		return;
	}
	fprintf(log_file, "Error Code: %d, Message: %s ,time: ", error, error_str);
	Date d = getCurrentDate(); // Assuming getCurrentDate() is defined in inputer.c
	fprintf(log_file,"Date: %04d-%02d-%02d %02d:%02d\n", d.Year, d.Month, d.Day, d.Hour, d.Min);
}