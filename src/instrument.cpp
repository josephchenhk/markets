#include "instrument.h"

#include <stdexcept>
#include <algorithm>

Instrument::Instrument(const std::string& symbol)
    : m_symbol(symbol) 
{
    if( std::any_of(symbol.begin(), symbol.end(), &::islower) ){
        throw std::invalid_argument("String must contain only uppercase letters");
    }
}
