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
/// <summary>
/// error codes and their descriptions
/// 0 - No error: stantard message for basic operations in system (start, end, etc.)
/// 5 - Input error: when input is not valid, such as wrong type or out of range.
/// 400 - Null pointer error: when a pointer is NULL where it shouldn't be.
/// 1001 - Memory allocation error: when memory allocation fails.
/// 5000 - File open error: when a file cannot be opened.
/// 9999 - General error: when an unexpected error occurs.
/// </summary>

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
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fprintf(log_file, "Error Code: %d, Message: %s ,time: ", error, error_str);
	fprintf(log_file,"Date: %04d-%02d-%02d %02d:%02d:%02d\n", timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min ,timeinfo->tm_sec);
}