#ifndef RECORD_H
#define RECORD_H

#include <string>

class Record
{
public:
    std::string tconst;
    double averageRating;
    int numVotes;

    Record(std::string tconst, double averageRating, int numVotes);
    void print() const;
    std::string serialize() const;
};

#endif // RECORD_H
