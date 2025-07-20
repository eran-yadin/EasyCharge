#ifndef _LIB_H
#define _LIB_H

typedef struct Port Port;
typedef struct Car Car;



// ---------------- Date ----------------
typedef struct {
    short Year;
    short Month;
    short Day;
    short Hour;
    short Min;
} Date;

// ---------------- Enums ----------------
typedef enum {
    FAST,
    MID,
    SLOW
} portType;

// ---------------- Car ----------------
struct Car {
    char nLicense[10];
    portType type;
    double totalPayed;
    Port* pPort;   // Pointer to current port (if any)
    short inqueue;
};

// ---------------- Binary Tree of Cars ----------------
typedef struct tCar {
    Car* car;
    struct tCar* right;
    struct tCar* left;
} tCar;

// ---------------- Queue Node ----------------
typedef struct carNode {
    Car* car;
    struct carNode* next;
} carNode;

// ---------------- Queue Manager ----------------
typedef struct {
    carNode* front;
    carNode* rear;
} qCar;

// ---------------- Port ----------------
typedef struct Port {
    int num;
    portType type;
    short status; // 1 - occupied, 2 - free, 3 - out of order
    Car* p2car;
    Date tin;
    Port* next;
}Port;


// ---------------- Station ----------------
typedef struct Station{
    unsigned id;
    char* name;
    int nPorts;
    double coord[2];
    Port *portList;   // Head of linked list of ports
    int nCars;
    qCar carQueue;
	struct Station* left;   // Left child in binary tree
	struct Station* right;  // Right child in binary tree
} Station;

typedef struct car_db {
    char nLicense[10];
    portType type;
    double totalPayed;
    int st_id;
    int port;// Pointer to current port (if any)
    int inqueue;
	struct car_db* next;
}car_db;

typedef struct {
	double x;
	double y;
} coord;

// ---------------- Raw Data Structures ----------------
//-----raw port data-----
typedef struct Raw_DB_Port{
    int station_id;
    int port_num;
    portType type;
	int status; // 1 - occupied, 2 - free, 3 - out of order
    char nLicense[10];
	Date tin;
	struct Raw_DB_Port* next;
}raw_db_port;

//-----raw queue data-----
typedef struct RawQueue  // Structure for queue row
{
	char nLicense[10];
    int id;
}raw_que;

//----------- DB-holder -----------
typedef struct
{
    Date date; // Date of the last update
    tCar* car_db; // Pointer to the binary tree of cars
    Station* st_db; // Pointer to the binary tree of stations
}DB_holder;


DB_holder* loadFiles();
DB_holder* save_files(DB_holder* db_holder);
void free_DB_holder(DB_holder* db_holder);
int menu_decision();
void fun_executer(int decision, DB_holder* db_holder);

//func 1:
Station* locNearSt(Station* st_db);
Station* locNearSt_rec(Station* st_db, coord user_coord);
double distance(coord a, coord b);
coord convert_coord(double x, double y);
void print_nearest_Station(Station* st, coord user_coord);

//func 2:
void charge_car(DB_holder* db_holder);

//func 3:
DB_holder checkCarStatus(DB_holder db);
#endif