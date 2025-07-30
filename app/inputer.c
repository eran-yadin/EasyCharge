#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "math.h"

#include "lib.h"
#include "inputer.h"
#include "print_lib.h"
#include "binery.h"
#include "port.h"


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
	trim_newline(input);
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


Date get_user_Data() {
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

Station* get_user_station(Station* st_db)
{
	char* st_id_str = get_user_string();
	unsigned int* st_id_unsigned = turn_string_to_us_int(st_id_str);
	Station* st = NULL;
	if (st_id_unsigned) {
		st = find_station_by_id(st_db, *st_id_unsigned);
		
	}
	else
	{
		st = find_station_by_name(st_db, st_id_str);
	}
	if (!st)
	{	//st does not exist
		printf("station does not exist. ");
		return NULL; // Return NULL if the station is not found
	}
	return st; // Return the station found or NULL if not found
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
char* get_user_nLisence()
{
	char buffer[256]; // 9 digits + null terminato
	int ID;
    do
    {
        printf("Enter Car ID[8]: ");
        scanf("%s", &buffer);
		int* lis = turn_string_to_us_int(buffer);
		if (lis == NULL) { ID = -1; }
		else { ID = *lis; } // Convert the string to an integer
    } while (!(ID<99999999 && ID>=0));
	char* car_id = malloc(10 * sizeof(char)); // 9 digits + null terminator
	if (car_id == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	snprintf(car_id, 10, "%08d", ID); // Format as 9-digit string
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
	fflush(stdin); // Ensure the prompt is printed before waiting
	getchar(); // Wait for the user to press Enter
}

void clean_stdin()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF); // Clear the input buffer
}

unsigned get_user_st_ID()
{
	unsigned st_id;
	do
	{
		printf("Enter Station ID: ");
		scanf("%u", &st_id);
	} while (st_id <= 0); // Ensure the station ID is a positive integer
	return st_id;
}

tCar* get_user_new_car()
{
	tCar* new_car = malloc(sizeof(tCar));
	if (!new_car)
	{
		return;
	}
	new_car->car = malloc(sizeof(Car));
	new_car->car->pPort = NULL;
	new_car->left = NULL;
	new_car->right = NULL;

	if (new_car == NULL) {
		fprintf(stderr, "Memory allocation failed for new car.\n");
		return NULL;
	}

	printf("Enter Car License Plate: ");
	char* license_plate = get_user_nLisence();
	if (license_plate == NULL) {
		free(new_car);
		return NULL; // Error handling for license plate input
	}
	strncpy(new_car->car->nLicense, license_plate, 9);
	new_car->car->nLicense[9] = '\0'; // Ensure null termination
	free(license_plate);

	printf("Enter Car Charge Type (0 - FAST, 1 - MID, 2 - SLOW): ");
	int type;
	do
	{

		scanf("%d", &type);
	} while (!is_valid(type, 2, 0));
	new_car->car->type = which_port_type_int(type);

	new_car->car->totalPayed = 0.0; // Initialize total paid to 0
	new_car->car->pPort = NULL; // No port assigned initially
	new_car->car->inqueue = 0; // Not in queue initially

	return new_car;
}

int get_user_port_num()
{
	char buffer[256]; // 9 digits + null terminato
	int num;
	do
	{
		printf("Enter port num: ");
		scanf("%s", &buffer);
		int* lis = turn_string_to_us_int(buffer);
		if (lis == NULL) { num = -1; }
		else { num = *lis; } // Convert the string to an integer
	} while (!(num < 99999999 && num >= 0));
	return num;
}

unsigned* turn_string_to_us_int(const char* str) 
{
	int* p;
	int i = strlen(str)-1;
	int u = 0;
	int sum = 0;
	for (i; i > -1; i)
	{
		if (*str < '0' || *str > '9')
		{
			return NULL; // Return NULL if invalid character found
		}
		int temp = (str[i] - '0');
		sum = sum+ pow(10,u)*(str[i] - '0');
		u++; i--;
	}
	p = &sum;
	return p;
}

unsigned int get_charge_min(Date a, Date b)
{

	struct tm tm_a = { 0 }, tm_b = { 0 }; //date and hour, include in time.h
	//tm_year- years since 1900
	//tm_mon: 0-11. jan=0, feb=1.... dec=11)

	//date a
	tm_a.tm_year = a.Year - 1900;
	tm_a.tm_mon = a.Month - 1;
	tm_a.tm_mday = a.Day;
	tm_a.tm_hour = a.Hour;
	tm_a.tm_min = a.Min;

	//date b
	tm_b.tm_year = b.Year - 1900;
	tm_b.tm_mon = b.Month - 1;
	tm_b.tm_mday = b.Day;
	tm_b.tm_hour = b.Hour;
	tm_b.tm_min = b.Min;

	//Convert to time_t
	time_t time_a = mktime(&tm_a);
	time_t time_b = mktime(&tm_b);

	if (time_a == (time_t)-1 || time_b == (time_t)-1) //convert error
		return 0;

	double diff_seconds = difftime(time_b, time_a);// get the difference in seconds 
	unsigned int diff_minutes = (unsigned int)(diff_seconds / 60.0);// convert to minutes

	
	return diff_minutes > 0 ? diff_minutes : abs(diff_minutes); //return unsigned
}