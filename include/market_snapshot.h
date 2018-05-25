
#ifndef MARKET_SNAPHOT_H
#define MARKET_SNAPHOT_H

#include <map>
#include <instrument.h>
#include <market_bar.h>

/**
 * @class MarketSnapshot
 * @author Benjamin
 * @date 22/05/18
 * @file market_snapshot.h
 * @brief Represents a snapshot of the market at a particular point in time
 */

class MarketSnapshot
{
public:

    // market info
    const MarketBar::Time m_time;
    const std::map<Instrument,MarketBar> m_bars;
    
    /**
     * @brief Constructs a new MarketSnapshot from the provided information
     * @param time denotes the snapshot time
     * @param bars denotes the snapshot market data
     */
    
    MarketSnapshot(const MarketBar::Time &time,
                   const std::map<Instrument,MarketBar> &bars):
                   m_time(time), m_bars(bars) 
    {
        for (auto &elem : bars)
        {
            if (elem.second.m_close_time != this->m_time)
            {
                throw std::invalid_argument("Inconsistent time");
            }
        }
    }
};

#endif // MARKET_SNAPHOT_H