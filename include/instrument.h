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
    
    // uniquely identifying symbol
    const std::string m_symbol;
    
    /**
     * @brief Constructs an Instrument with the provided symbol
     * @param s denotes the symbol
     */
    Instrument(const std::string& symbol);
    

    /**
     * @brief relational operator required by std::map
     * @param left
     * @param right
     */
    bool operator<(const Instrument& other) const;
};

#endif // INSTRUMENT_H