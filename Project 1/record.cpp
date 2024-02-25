#include "record.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

Record::Record(std::string tconst, double averageRating, int numVotes)
    : tconst(tconst), averageRating(averageRating), numVotes(numVotes) {}

void Record::print() const
{
    cout << tconst << "\t" << std::fixed << std::setprecision(1) << averageRating << "\t" << numVotes << std::endl;
}

std::string Record::serialize() const
{
    std::ostringstream oss;
    oss << tconst << " " << averageRating << " " << numVotes;
    return oss.str();
}