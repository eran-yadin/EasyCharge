#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"
#include "print_lib.h"
#include "inputer.h"
#include "port.h"
#include "binery.h"
#include "error_handle.h"


int main() {
	showOpening();
	printf("welcome:\n");
	log_error(0, "Program started successfully in main");
	int decision = 0;
	do
	{
		DB_holder* DB = loadFiles();
		if (DB == NULL) {
			
			log_error(9999, "Failed to load database in main");
			fprintf(stderr, "Failed to load database.\n");
			return EXIT_FAILURE;
		}
		decision = menu_decision();
		fun_executer(decision, DB);
		//print_ALL_DB(DB->st_db);
		save_files(DB);
	} while (decision != 0);
}