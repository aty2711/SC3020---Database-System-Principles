#include "database.h"
#include <iostream>
using namespace std;

int main()
{
    Database db(100000000);

    // Example: Add records to the database
    db.addRecord(Record("tt0000001", 5.6, 1645));
    db.addRecord(Record("tt0000002", 6.1, 198));


    return 0;
}
