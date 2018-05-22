#include "market_bar.h"

#include <stdexcept>


MarketBar::MarketBar(const double &open, const double &high, const double &low, const double &close, const unsigned int &vol, const Time &close_time)
    : m_open(open), m_high(high), m_low(low), m_close(close), m_volume(vol), m_close_time(close_time)
{
    if (open <= 0.0) throw std::invalid_argument("Open price must be positive");
    if (high <= 0.0) throw std::invalid_argument("High price must be positive");
    if (low <= 0.0) throw std::invalid_argument("Low price must be positive");
    if (close <= 0.0) throw std::invalid_argument("Close price must be positive");
    if (vol < 0) throw std::invalid_argument("Volume must be non-negative");
    if (high < low) throw std::invalid_argument("high must be greater than or equal to low price");
}

