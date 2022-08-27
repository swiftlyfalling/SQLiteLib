#include <sqlite3.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    sqlite3 *connection;
    int code = sqlite3_open(":memory:", &connection);
    sqlite3_close(connection);
    return (code != SQLITE_OK);
}
