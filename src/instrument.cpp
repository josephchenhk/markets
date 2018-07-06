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


bool Instrument::operator<(const Instrument& other) const
{
    return m_symbol < other.m_symbol;
}


bool Instrument::operator==(const Instrument& other) const
{
    return m_symbol == other.m_symbol;
}