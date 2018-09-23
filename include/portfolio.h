#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <Eigen/Dense>
#include <queue>
#include <iostream>

#include "position_summary.h"
#include "execution_handler.h"

//TODO: shouldn't this contain a fill queue?
//TODO: allow changing the execution strategy

/**
 * @class Portfolio
 * @author t
 * @date 06/07/18
 * @file portfolio.h
 * @brief This represents the top level of all accounting details. 
 * 1. (or 2.) Read all fills and update positions accordingly (readAllFills)
 * 2. (or 1.) Read in the most recent prices and update positions/info accordingly ( readNewPrices ) 
 * 3. Receive new ideal weights, calculate orders, and then submit these orders. (updateOnNewIdealWts)
 * NB: this is meant to be subclassed
 */
class Portfolio 
{

public:

    /**
     * @brief ctor
     * @param starting_cash
     * @param tickers: all tickers that might have a position on at least once
     * @param cs commission style
     */
    Portfolio(double starting_cash, std::vector<std::string> tickers, CommissionStyle cs = CommissionStyle::IBFixed);
    
    
    /**
     * @brief reads fills from a queue and processes them
     * @param q the queue of fills (passed by non-const reference)
     */
    void readAllFills(std::queue<Fill>& q);


    /**
     * @brief processes new market data
     * @param ms a market snapshot
     */
    void readNewPrices(MarketSnapshot ms);
    
    
    /**
     * @brief take in suggested weights from a statistical model for one time period ahead in the future, compute orders, then submit orders to a queue
     * @param ideal_wts_to_be these are based on an optimization procedure (probably) using the mean vector and covariace matrix of the future log returns
     * @param order_q, the order_q you submit orders to
     */
    void updateOnNewIdealWts(const Eigen::VectorXd& ideal_wts_to_be, ExecHandler& order_q);
    

    /**
     * @brief gets your balance
     * @return the balance in dollars (double)
     */
    double getBalance() const;

    
    /**
     * @brief gets the outstanding market value of a position on a given symbol
     * @
     */
    double getMktVal(const std::string& sym) const;


private:
    
    MarketSnapshot m_last_snapshot;
    std::vector<Instrument> m_ordered_tickers;
    PositionSummary m_pos_summary;
    unsigned int m_todo_step;

};

#endif // PORTFOLIO_H
