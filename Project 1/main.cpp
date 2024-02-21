#include "database.h"
#include "b_plus_tree.h"
#include "record.h"
#include <iostream>
using namespace std;

int main()
{
    Database db(100000000);

    // Example: Add records to the database
    db.addRecord(Record("tt0000001", 5.6, 1645));
    db.addRecord(Record("tt0000002", 6.1, 198));

    // Initialize BPTree
    BPTree tree = BPTree();

    return 0;
}
