/**
 * @file record.h
 * @brief Defines the Record class for storing movie or show records.
 *
 * The Record class encapsulates the data associated with IMDb movie or show records,
 * including its unique identifier (tconst), average rating, and number of votes.
 * This class enforces data validation upon initialization to ensure the integrity
 * and consistency of the record data. This facilitates maintaining a
 * fixed and constant record size, which simplifies data access and manipulation since each record
 * is guaranteed to be within a single block. This can improve performance for read and write operations
 * and make transaction management easier. Meanwhile, the drawback of space wastage is slightly alleviated
 * because in a 200 byte block, storing 19 bytes, accounting to 10 bytes of metaData. Thus space wastage is minimal
 * The class stores its data in private member variables and provides public accessors so that it can
 * enforce its validation criteria.
 */

#ifndef RECORD_H
#define RECORD_H
#include <string>

class Record
{
public:
    /**
     * Constructor for the Record class.
     * Initializes a Record object with specified values, enforcing data validation.
     *
     * @param tconst A unique identifier for the record. Must be 9/10 characters long +1 null terminator. 11 bytes
     * @param averageRating Must be a valid float. 4 bytes
     * @param numVotes  Must be non-negative. 4 bytes
     *
     * @throw std::invalid_argument If any parameter does not meet its validation criteria.
     */
    Record(std::string tconst, float averageRating, int numVotes);

    std::string getTconst() const
    {
        std::string str = tconst;
        return str;
    }
    float getAverageRating() const { return averageRating; }
    int getNumVotes() const { return numVotes; }
    void print() const;

private:
    char tconst[11];
    float averageRating;
    int numVotes;
};

#endif // RECORD_H
