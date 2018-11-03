#ifndef POSITION_SUMMARY_H
#define POSITION_SUMMARY_H

#include <map>
#include <instrument.h>
#include <vector>

#include "market_snapshot.h"
#include "pnl_calculator.h"
#include "fill.h"

// TODO: figure out how to get total liquidation value in terms of realized pnl, unrealized pnl, current market value, cost basis

/**
 * @class PositionSummary
 * @author t
 * @date 06/07/18
 * @file position_summary.h
 * @brief Provides a collection of Position objects.
 */
class PositionSummary
{
    
public:
  
    /**
     * @brief sets up a zero position for each symbol
     * @param tickers, the ordered collection of which stocks you want to keep track of
     */
    PositionSummary(double initial_capital, const std::vector<std::string>& tickers, CommissionStyle cs);
  
    
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
  
  
    /**
     * @brief gets the market value for a particular position.
     * @param instr an instrument you're interested in.
     * @return the current market value for Instrument "instr"
     */
    double getInstrumentMktVal(const Instrument& instr) const;


    /**
     * @brief gets the market value for a particular position
     * @param a std::string of the parameter you're interested in
     * @return the current market value for Instrument "instr"
     */
    double getInstrumentMktVal(const std::string& sym) const;


    /**
     * @brief gets the number of shares currently held (negative if short)
     * @param an instrument of type Instr
     * @return the number of shares as an int
     */
    int getInstrumentShares(const Instrument& instr) const;


    /**
     * @brief gives you the number of shares currently held (negative for short)
     * @param sym the ticker symbol in all caps
     * @return the current number of shares for sym
     */
    int getInstrumentShares(const std::string& sym) const;


    /**
     * @brief gets the current balance (starting cash plus all realized pnls)
     * @return the balance as a double
     */
    double getBalance() const;


private:

    double m_starting_cash; // starting_cash + realized pnls of all positions
    std::map<Instrument,pnl_calc> m_positions; 
    CommissionStyle m_cs;

};

#endif // POSITION_SUMMARY_H
