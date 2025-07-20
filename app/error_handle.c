`#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lib.h"
#include "inputer.h"
#include "error_handle.h"

#define max_error_list 15
/// Error handling module for the application
/// code: [0][1][2][3]
/// [0]: app code /type
/// [1]: place
/// [2][3]: error code

//----------Error codes----------
//error code: 1000-1999 memory allocation errors
#define MY_DB_ERROR 1001

//error code: 2000-2999 input errors
// 
//----------Error codes----------


void print_error(Error* error)
{
	if (!error)
	{
		return;
	}
	printf("-------------------\n");
	printDate(error->error_date);
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
	return NULL;
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

void print_last_error(Error** head)
{
	if (!head)
	{
		return;
	}
	printf("----------last-error---------\n");
	printDate(head[0]->error_date);
	printf("Error Code: %u\n", head[0]->error_code);
	printf("Error Message: %s\n", head[0]->error_message);
}

const char* my_strerror(int code)
{
    switch (code) {
    case MY_DB_ERROR: return "Database connection failed";
    default: return "Unknown error";
    }
}
