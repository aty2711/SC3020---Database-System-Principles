#include "record.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cmath>

using namespace std;

Record::Record(std::string tconst, float averageRating, int numVotes)
    : averageRating(averageRating), numVotes(numVotes)
{
    try
    {
        if (tconst.length() == 9)
        {
            tconst += " "; // padding with space
        }
        if (tconst.length() != 10)
        {
            throw std::invalid_argument("tconst must be exactly 9/10 characters. " + tconst + " is " + std::to_string(tconst.length()) + " characters long.");
        }
        // Copy the tconst to the char array
        strncpy(this->tconst, tconst.c_str(), sizeof(this->tconst) - 1); // Copy only up to 10 characters
        this->tconst[sizeof(this->tconst) - 1] = '\0';                   // Ensure null termination

        // Check for valid float (not NaN or infinity)
        if (std::isnan(averageRating) || std::isinf(averageRating))
        {
            throw std::invalid_argument("averageRating must be a valid float.");
        }

        // Check for non-negative numVotes
        if (numVotes < 0)
        {
            throw std::invalid_argument("numVotes must be non-negative.");
        }
        this->numVotes = numVotes;
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
void Record::print() const
{
    cout << tconst << "\t" << std::fixed << std::setprecision(1) << averageRating << "\t" << numVotes << std::endl;
}
