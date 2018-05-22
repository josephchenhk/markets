
#ifndef MARKET_BAR_H
#define MARKET_BAR_H

#include <stdexcept>

/**
 * @class MarketBar
 * @author Benjamin
 * @date 17/05/18
 * @file market_bar.h
 * @brief Represents a summary of market data for a particular instrument over a given
 * period of time
 */

class MarketBar
{
public:

    // market bar elements
    const double open_price;
    const double high_price;
    const double low_price;
    const double close_price;
    const long volume;
    
    /**
     * @brief Constructs a MarketBar from the provided parameters
     * 
     * @param open denotes the instrument opening price of the period
     * @param high denotes the instrument high price during the period
     * @param low denotes the instrument low price during the period
     * @param close denotes the instrument closing price of the period
     * @param vol denotes the instrument volume over the period
     */
    
    MarketBar(double open, double high, double low, double close, long vol): 
    open_price(open), high_price(high), low_price(low), close_price(close), volume(vol)
    {
        if (open_price <= 0.0) throw std::invalid_argument("Open price must be positive");
        if (high_price <= 0.0) throw std::invalid_argument("High price must be positive");
        if (low_price <= 0.0) throw std::invalid_argument("Low price must be positive");
        if (close_price <= 0.0) throw std::invalid_argument("Close price must be positive");
        if (volume < 0) throw std::invalid_argument("Volume must be non-negative");
    }
};

#endif // MARKET_BAR_H