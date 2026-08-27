# EasyCharge

A management system for electric-vehicle charging stations, written in C for a college
systems-programming course. It runs as a menu-driven console app over a plain-text database.

Thirteen operations cover the life of a charging session: find the nearest station, put a car
on a port, check on it, stop the charge and bill it. Behind that sit the data structures the
course was actually about — stations in a binary search tree, cars in a second tree, ports as a
linked list per station, and a FIFO queue of cars waiting when every port is busy.

## Build

The repository is a Visual Studio solution (`app.sln`, VS 2022). Open it and build `app` —
Debug or Release, x86 or x64. That is the supported path and it needs no configuration.

**On Linux/macOS it does not build as-is.** Two changes are needed:

1. `app/error_handle.c` uses `errno` without including `<errno.h>` (MSVC pulls it in
   transitively; gcc and clang do not).
2. `app/binery.c:22` returns the integer `0x222222201` as a `Station*` end-of-file sentinel.
   GCC 14+ rejects that outright.

With the include added, this compiles and runs:

```sh
cd app
gcc -std=c11 -w -Wno-error=int-conversion -Wno-error=incompatible-pointer-types \
    -o easycharge *.c -lm
./easycharge
```

Run it from `app/`, not from the repository root — the data files are opened by relative path.

## The database

Four CSV-style text files in `app/`, each with a header row. They are read at the top of every
menu iteration and rewritten after it, so the on-disk state is always current.

| file | row |
|---|---|
| `Stations.txt` | `ID,StationName,NumOfPorts,CoordX,CoordY` |
| `Ports.txt` | `StationID,PortNumber,PortType,Status,Year,Month,Day,Hour,Min,CarLicense` |
| `Cars.txt` | `License,PortType,TotalPayed,StationID,PortNumber,InQueue` |
| `LineOfCars.txt` | `License,StationID` — the waiting queues |

Coordinates are decimal degrees. `PortType` is `FAST`, `MID` or `SLOW`. `Status` is `1` occupied,
`2` free, `3` out of order.

A fifth file, `error_log.txt`, is written by the program (see **Error logging**) — it is output,
not input.

Note that `.gitignore` lists `Cars.txt`, `LineOfCars.txt`, `Ports.txt` and `Station.txt`, but the
sample data is committed anyway, and the last entry is a typo for `Stations.txt`. The sample data
is what the program starts from; deleting it leaves nothing to load.

## Menu

```
1.  Locate Closest Station        8.  Display Top Customers
2.  Charge Car                    9.  Add New Port
3.  Check Car Status             10.  Release Charging Port
4.  Stop Charge                  11.  Remove Out of Order Ports
5.  Display All Stations         12.  Remove Customer
6.  Display Cars at Station      13.  Close Station
7.  Station Statistics            0.  Exit
```

**1** takes user coordinates and walks the station tree for the smallest Euclidean distance.
**2** assigns a free port of the requested type, or queues the car if there is none. **8** does a
single pass over the car tree keeping the five highest `totalPayed` values, rather than sorting.
**13** closes a station and frees its ports, queue and subtree.

## Layout

| file | what is in it |
|---|---|
| `main.c` | the load → menu → dispatch → save loop |
| `lib.c` / `lib.h` | the data model and all thirteen operations |
| `binery.c` | file I/O and the tree/list/queue primitives |
| `port.c` | port allocation, release and station-side port lists |
| `inputer.c` | input reading and validation |
| `print_lib.c` | menu, banner and all formatted output |
| `error_handle.c` | the error list and `log_error` |

`binery.c` is spelled that way in the source; despite the name it is the text-file layer, not a
binary format. `binary.h` is a three-line leftover and is not used.

## Error logging

Every operation reports through `log_error(code, message)`, which appends a timestamped line to
`error_log.txt`. Code `0` means success, so the log is a full trace of the run rather than only a
record of failures. Input validation is centralised in `inputer.c` — out-of-range menu choices and
malformed coordinates are rejected and re-prompted rather than propagated.

## License

GPL-3.0. See `LICENSE.txt`.
