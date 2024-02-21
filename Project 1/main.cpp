#include "database.h"
#include "b_plus_tree.h"
#include "record.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
using namespace std;

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~ IMPORTANT ~~~~~~~~~~~~~~~~~~~~~~~
 * If you are using Visual Studio Code as your IDE,
 * running the compiler with F5 may not work.
 * 
 * If you are facing this problem, then paste these line-by-line in
 * your CLI / terminal: (include all .cpp files in the list)
 * 
 * cd "Project 1"
 * g++ main.cpp b_plus_tree.cpp block.cpp database.cpp record.cpp -o main
 * ./main
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

int main()
{
    cout << "<------------------- Database Storage Component ------------------->\n"
            "Database is created by allocating a memory pool, divided into blocks\n"
            "We shall make use of a fixed-size memory pool for storage"
         << "\n"
         << "\n";

    cout << "<------------------- Data file read started ------------------->"
         << "\n"
         << "\n";

    ifstream dataFile("C:/Users/Ng Yong Jian/OneDrive - Nanyang Technological University/Desktop/NTU/Y3S2/SC3020 Database System Principles/Projects/data.tsv");

    Database db(500000000);

    // Example: Add records to the database
    // db.addRecord(Record("tt0000001", 5.6, 1645));
    // db.addRecord(Record("tt0000002", 6.1, 198));

    bool flag = true;
    // int count = 0;
    if (dataFile.is_open())
    {
        string line;

        while (getline(dataFile, line))
        {
            if (flag)
            {
                flag = false;
                continue;
            }

            string tempLine;

            string tconst = line.substr(0, line.find('\t'));

            stringstream linestream(line);
            getline(linestream, tempLine, '\t');
            double averageRating;
            int numVotes;
            linestream >> averageRating >> numVotes;

            Record record(tconst, averageRating, numVotes);

            if (!db.addRecord(record))
            {
                cout << "Failed to add record to database." << endl;
            }
            else
            {
                // count += 1;
                // cout << "Record successfully added to database." << endl;
            }
        }
    }
    cout << "<------------------- Data file read ended ------------------->"
         << "\n"
         << "\n";

    cout << "<------------------- Database Statistics ------------------->"
         << "\n";
    cout << "1. Size of Memory Pool: " << db.getDatabaseSize() << "\n ";
    cout << "2. Number of blocks available: " << db.getDatabaseSize() / 200 << "\n";
    cout << "3. Number of allocated blocks: " << db.getNumAllocBlks()
         << "\n";
    cout << "4. Number of available blocks: " << db.getNumAvailBlks() << "\n"
         << '\n';

    dataFile.close();

    // Initialize BPTree
    BPTree tree = BPTree();

    return 0;
}
