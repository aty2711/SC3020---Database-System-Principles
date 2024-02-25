#ifndef RECORD_H
#define RECORD_H

#include <string>

class Record
{
public:
    std::string tconst;   // 10 bytes
    double averageRating; // 8 bytes
    int numVotes;         // 4 bytes

    Record(std::string tconst, double averageRating, int numVotes);
    void print() const;
    std::string serialize() const;
};

#endif // RECORD_H
