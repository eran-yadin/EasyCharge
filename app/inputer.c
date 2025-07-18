#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lib.h"
#include "inputer.h"
#include "print_lib.h"

char *get_user_string()
{
    char buffer[255];
    fgets(buffer,254,stdin);
    char *input = malloc((strlen(buffer)+1)*sizeof(char));
	if (input == NULL) {
		//error code:
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	strcpy(input, buffer);
    return input;
}


Date getCurrentDate() {
    time_t rawtime;
    struct tm *timeinfo;
    Date currentDate;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    currentDate.Year  = (short)(timeinfo->tm_year + 1900);
    currentDate.Month = (short)(timeinfo->tm_mon + 1);
    currentDate.Day   = (short)timeinfo->tm_mday;
    currentDate.Hour  = (short)timeinfo->tm_hour;
    currentDate.Min   = (short)timeinfo->tm_min;

    return currentDate;
}


void printDate(Date d) {
    printf("Date: %04d-%02d-%02d %02d:%02d\n", d.Year, d.Month, d.Day, d.Hour, d.Min);
}


Date inputDateFromUser() {
    Date d;
	//make sure to error handle the input (scanf)
    do
    {
        printf("Enter Year: ");
        scanf("%hd", &d.Year);
	} while (!is_valid(d.Year, 2100, 1900)); //make sure to change it in the next ara (FUTURE!!!!)
    
    do
    {
        printf("Enter Month (1 12): ");
        scanf("%hd", &d.Month);
    } while (!is_valid(d.Month,12,1));
    
    do
    {
        printf("Enter Day (1 31): ");
        scanf("%hd", &d.Day);
    } while (!is_valid(d.Day,31,1));
    
    do
    {
        printf("Enter Hour (0 23): ");
        scanf("%hd", &d.Hour);
    } while (!is_valid(d.Hour,23,0));
    
    do
    {
        printf("Enter Minute (0 59): ");
        scanf("%hd", &d.Min);
    } while (!is_valid(d.Min,59,0));
    
    return d;
}


int is_valid(int val, int max, int min)
{
    if (val>= min && val <= max)
    {
        return 1;
    }
	return 0;
}

//TODO: test
coord get_user_coord()
{
	coord c;
	do
	{
		printf("Enter X coordinate: ");
		scanf("%lf", &c.x);
	} while (!is_valid(c.x, 180, -180)); // make sure the range is valid for longitude

	do
	{
		printf("Enter Y coordinate: ");
		scanf("%lf", &c.y);
	} while (!is_valid(c.y, 90, -90)); // make sure the range is valid for latitude

	return c;
}

// This function prompts the user to enter a car ID and returns it as a 9-digit string.
char* get_car_id_from_user()
{
    int ID;
    do
    {
        printf("Enter Car ID[9]: ");
        scanf("%d", &ID);
    } while (ID<99999999 && ID>0);
	char* car_id = malloc(10 * sizeof(char)); // 9 digits + null terminator
	if (car_id == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	snprintf(car_id, 10, "%09d", ID); // Format as 9-digit string
	return car_id;
}

//trim the \n character from the end of a string
void trim_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void wait_for_user()
{
	printf("Press Enter to continue...");
	while (getchar() != '\n'); // Clear the input buffer
	getchar(); // Wait for the user to press Enter
}

void clean_stdin()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF); // Clear the input buffer
}