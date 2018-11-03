#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <string>

/**
 * @class Instrument
 * @author Benjamin
 * @date 17/05/18
 * @file instrument.h
 * @brief Represents a tradable instrument in the financial markets
 */

class Instrument
{
public:
    
    // uniquely identifying ticker symbol
    const std::string symbol;

    
//    /**
//     * @brief Constructs an Instrument with the provided Instrument.
//     * @param instr is the instrument you're copying. 
//     */
//    Instrument(const Instrument& instr);


    /**
     * @brief Constructs an Instrument with the provided symbol
     * @param sym denotes the symbol
     */
    Instrument(const std::string& sym);
    

    /**
     * @brief relational operator that orders the ticker symbols
     * @param left
     * @param right
     */
    bool operator<(const Instrument& other) const;
    
    
    /**
     * @brief equality operator for the ticker symbols
     * @param other the Instrument object you're comparing to
     */
    bool operator==(const Instrument& other) const;
    
};

#endif // INSTRUMENT_H
