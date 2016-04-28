#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// the define directive creates tokens the all caps strings which the compiler replaces with the 
// params following. You can also define functions, which is pretty cool:
// #define multiply 
#define MAX_DATA 512
#define MAX_ROWS 100

// declaring the struct for each Address in the db
struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

// defining the struct for the Database, which will have space for MAX_ROWS. Since this definition
// refers to Address, Address must be predeclared.
struct Database {
    struct Address rows[MAX_ROWS];
};

// Defining the connection to a file.
struct Connection {
    // the FILE struct is defined by the C standard library. So, you don't have to actually implement file
    // operations.
    FILE *file;
    struct Database *db;
};

// Destroys a database connection by taking the pointer to a connection.
void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) free(conn->db);
        free(conn);
    }
}

// DIE DIE DIE: a simple function to quit the program if things go wrong. The message pointer is pretty good;
// makes die into a metafunction.
void die(const char *message, struct Connection *conn)
{   
    // errno and perror are base functions that return numerical codes for errors.
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
        Database_close(conn);
    }

    exit(1);
}

// Function to print out information of an Address; takes a pointer to an address.
void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

// Function to connect to a DB file. Makes use of some basic function such as fread and size of; references the
// fields of the connection pointer with the -> operator. Important to note that this operates on the database 
// in place: nothing is returned by the function.
void Database_load(struct Connection *conn)
{
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to load database.", conn);
}

// Function for opening a database; returns a pointer to a Connection; takes a filename and a mode char. Allocates
// mem on the heap to do this.
struct Connection *Database_open(const char *filename, char mode)
{
    // Allocate memory for the database on the heap. Throw an error if allocations fails.
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    // Allocate space for the database on the heap; associate it with the connection pointer.
    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error", conn);

    // Accept modes for reading, writing, etc.
    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");
        // Attempt to load the file into the conn pointer.
        if(conn->file) {
            Database_load(conn);
        }
    }

    // If the file fails to load, throw a different error.
    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to write database.", conn);

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;
    // WARNING: read "How to Break it" and fix
    char *res = strncpy(addr->name, name, MAX_DATA);
    // demonstrate the strncpy bug
    if(!res) die("Name copy failed", conn);

    res = strncpy(addr->email, email, MAX_DATA);
    if(!res) die("Email copy failed", conn);

}

void Database_get(struct Connection *conn, int id)
{
    // The address call returns a pointer to the heap
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i=0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];
        
        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", conn);

    if(argc > 3) id = atoi(argv[3]);
    if(id >= MAX_ROWS) die("There are not that many records.", conn);

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die ("Need an id to get.", conn);

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

        default:
            die("Invalad action. Please use valid: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}
