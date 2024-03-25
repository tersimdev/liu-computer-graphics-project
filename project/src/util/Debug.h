/*
    File containing debug macros, relies on printf
*/

// comment the line below to disable logs
#define DEBUG 1

#include <string>
#include <iostream>

#ifdef DEBUG

/* Console Colors */
#define COL_RST  "\x1B[0m"
#define COL_RED  "\x1B[31m"
#define COL_GRN  "\x1B[32m"
#define COL_YEL  "\x1B[33m"
#define COL_BLU  "\x1B[34m"
#define COL_MAG  "\x1B[35m"
#define COL_CYN  "\x1B[36m"
#define COL_WHT  "\x1B[37m"


//to find out line number
#define debug_where() printf("%s: line %d\n", __FILE__, __LINE__)
//for printing variables
#define debug_log(...) printf(COL_GRN); printf(__VA_ARGS__); printf(COL_RST)
#define debug_warn(...) printf(COL_YEL "[WARN] "); printf(__VA_ARGS__); printf(COL_RST)
#define debug_error(...) printf(COL_RED "[ERROR] "); debug_where(); printf(__VA_ARGS__); printf(COL_RST)
#else
//empty macros for when logging disabled 
#define debug_where() do{}while(0)
#define debug_log(...) do{}while(0)
#define debug_warn(...) do{}while(0)
#define debug_error(...) do{}while(0)
#endif