
#ifndef POSITION_SUMMARY_H
#define POSITION_SUMMARY_H

#include <map>
#include <instrument.h>
#include <vector>

#include "market_snapshot.h"
#include "pnl_calculator.h"
#include "fill.h"


/**
 * @class PositionSummary
 * @author t
 * @date 06/07/18
 * @file position_summary.h
 * @brief Provides a collection of Position objects.
 */
class PositionSummary
{
    
private:

    std::vector<pnl_calc> m_positions;
    CommissionStyle m_cs;

public:

  
    /**
     * @brief sets up a zero position for each symbol
     * @param tickers, the ordered collection of which stocks you want to keep track of
     */
    PositionSummary(const std::vector<std::string>& tickers, CommissionStyle cs);
  
    
    /**
     * @brief updates the positions with prices changing.
     * @param ms a market snapshot 
     */
    void onSnapshot(const MarketSnapshot& ms);
    
    
    /**
     * @brief updates the positions after a fill has been received.
     * @param fill a Fill event
     */
    void onFill(const Fill& fill);
  

};

#endif // POSITION_SUMMARY_H