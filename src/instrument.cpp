#include "instrument.h"

#include <stdexcept>
#include <algorithm>

Instrument::Instrument(const std::string& sym)
    : symbol(sym)
{
    if( std::any_of(symbol.begin(), symbol.end(), &::islower) ){
        throw std::invalid_argument("ticker symbol must be upper-case\n");
    }
}


bool Instrument::operator<(const Instrument& other) const
{
    return symbol < other.symbol;
}


bool Instrument::operator==(const Instrument& other) const
{
    return symbol == other.symbol;
}
