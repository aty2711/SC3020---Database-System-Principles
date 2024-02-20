#include "Record.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Record::Record(std::string tconst, double averageRating, int numVotes)
    : tconst(tconst), averageRating(averageRating), numVotes(numVotes) {}

void Record::print() const
{
    std::cout << tconst << "\t" << std::fixed << std::setprecision(1) << averageRating << "\t" << numVotes << std::endl;
}

std::string Record::serialize() const
{
    std::ostringstream oss;
    oss << tconst << " " << averageRating << " " << numVotes;
    return oss.str();
}