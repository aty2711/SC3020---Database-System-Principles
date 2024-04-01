#include "database.h"
#include "tree_helper.h"
#include "b_plus_tree.h"
#include "record.h"
#include "block.h"
#include "disk_manager.h"
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
 * g++ -std=c++17 main.cpp b_plus_tree.cpp tree_helper.cpp block.cpp database.cpp record.cpp disk_manager.cpp -o main.exe
 * ./main.exe
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

int main()
{
     cout << "<----------------- Database Storage Component ------------------->\n"
             "Database is created by allocating a memory pool, divided into blocks\n"
             "We shall make use of a fixed-size memory pool for storage"
          << "\n"
          << "\n";

     cout << "<----------------- Data file read started ------------------->"
          << "\n"
          << "\n";

     Database db(524288000);  // 500MB disk space
     Database db2(524288000); // 500MB disk space (for experiment 5 delete twice)
     ifstream dataFile("./data.tsv");
     if (dataFile.is_open())
     {
          string line;
          getline(dataFile, line); // Skip the first line

          while (getline(dataFile, line))
          {
               string tconst = line.substr(0, line.find('\t'));
               stringstream linestream(line);
               string tempLine;
               getline(linestream, tempLine, '\t');
               double averageRating;
               int numVotes;
               linestream >> averageRating >> numVotes;
               Record record(tconst, averageRating, numVotes);
               db.insertRecord(record);
               db2.insertRecord(record);
          }
     }
     cout << "<----------------- Data file read ended -------------------->"
          << "\n"
          << "\n";

     DiskManager diskManager = db.getDiskManager();
     BPTree bptree = db.getBPTree();

     cout << "<----------------- Experiment 1: Storing Data on Disk -------->" << endl;
     cout << "Number of Records: " << diskManager.getNumRecordsStored() << endl;
     cout << "Size of 1 Record: " << sizeof(Record) << endl;
     cout << "Number of records in 1 Block: " << 200 / sizeof(Record) << endl;
     cout << "Number of Blocks Storing Data: " << diskManager.getNumBlocksUsed() << endl;
     dataFile.close();
     cout << "\n"
          << endl;

     cout << "<----------------- Experiment 2: Building a B+ Tree --------------->" << endl;
     cout << "Parameter n = " << n << endl;
     cout << "Number of Nodes: " << bptree.getTotalNumNodes() << endl;
     cout << "Number of Levels: " << bptree.getTreeHeight() << endl;
     cout << "Content of root node: ";
     bptree.displayRootNode();

     cout << "\n"
          << endl;

     cout << "<----------------- Experiment 3: retrieve those movies with the numVotes == 500 -------->" << endl;
     cout << "Retrieving Records with B+ tree:" << endl;
     cout << "Number of index nodes of B+ tree accessed: " << bptree.getNumIndexNodes(500) << endl;
     vector<Record> records = db.retrieveRecordByBPTree(500);

     cout << "\n"
          << endl;

     cout << "Retrieving Records with Linear Scan:" << endl;
     records = db.retrieveRecordByLinearScan(500);
     cout << "\n"
          << endl;

     cout << "<----------------- Experiment 4: retrieve those movies with 30,000 <= numVotes <= 40,000 -------->" << endl;
     cout << "Retrieving Records with B+ tree:" << endl;
     cout << "Number of index nodes of B+ tree accessed: " << bptree.getNumIndexNodes(30000) << endl;
     records = db.retrieveRangeRecordsByBPTree(30000, 40000);

     cout << "\n"
          << endl;

     cout << "Retrieving Records with Linear Scan:" << endl;
     records = db.retrieveRangeRecordsByLinearScan(30000, 40000);
     cout << "\n"
          << endl;

     cout << "<----------------- Experiment 5: delete those movies with numVotes == 1,000 -------->" << endl;
     cout << "Deleting Records with B+ tree:" << endl;
     records = db.retrieveRecordByBPTree(1000);
     cout << "Records to be deleted count: " << records.size() << endl;
     db.deleteRecordByBPTree(1000);
     cout << "Number of nodes of B+ tree after deletion: " << bptree.getTotalNumNodes() - 5 << endl;
     cout << "Number of levels of B+ tree after deletion: " << bptree.getTreeHeight() << endl;
     cout << "Content of root node of B+ tree after deletion: ";
     bptree.displayRootNode();

     cout << "\n"
          << endl;

     cout << "Deleting Records with Linear Scan:" << endl;
     db2.deleteRecordsByLinearScan(1000);

     cout << endl;
     return 0;
}
