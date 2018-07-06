
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
private:    
    
    MarketBar::Time m_time;
    std::map<Instrument,MarketBar> m_bars;

public:
    
    /**
     * @brief default constructor
     */
    MarketSnapshot() = default;
    

    /**
     * @brief Constructs a new MarketSnapshot from the provided information
     * @param time denotes the snapshot time
     * @param bars denotes the snapshot market data
     */
    MarketSnapshot(const MarketBar::Time &time, const std::map<Instrument,MarketBar> &bars);


    /**
     * @brief Constructs a new MarketSnapshot from the provided information
     * @param time denotes the snapshot time
     * @param bars denotes the snapshot market data
     * @param time_format denotes the string_format of the time
     */
    MarketSnapshot(const std::string& time, const std::map<Instrument,MarketBar> &bars, const std::string& time_format = "%Y-%m-%d %H:%M:%S");

    
    /**
     * @brief gets the time
     * @return time
     */
    MarketBar::Time time() const;


    /**
     * @brief gets the map of bars 
     * @return the OHLC bars
     */
    std::map<Instrument,MarketBar> bars() const;

};

#endif // MARKET_SNAPHOT_H