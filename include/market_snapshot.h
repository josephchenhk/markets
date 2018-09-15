
#ifndef MARKET_SNAPHOT_H
#define MARKET_SNAPHOT_H

#include <map>

#include "instrument.h"
#include "market_bar.h"

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
    
    std::map<Instrument,MarketBar> m_bars;

public:
    
    /**
     * @brief default constructor
     */
    MarketSnapshot() = default;
    

    /**
     * @brief Constructs a new MarketSnapshot from the provided information
     * @param bars denotes the snapshot market data
     */
    MarketSnapshot(const std::map<Instrument,MarketBar> &bars);

    
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


    /**
     * @brief returns the MarketBar with instrument name `instr_name`
     * @param instr_name
     * @return the associated MarketBar item
     */
    MarketBar& operator[](Instrument instr_name);


    /**
     * @brief returns the MarketBar with the instrument name 'instr_name'
     * @param instr_name the name of which instrument/stock
     * @return the associated MarketBar item
     */
    MarketBar& operator[](std::string instr_name);

};

#endif // MARKET_SNAPHOT_H
