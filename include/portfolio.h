#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <Eigen/Dense>
#include <queue>
#include <iostream> // std::cerr


#include "position_summary.h"
#include "execution_handler.h"

enum class ExecutionStyle { Market, Limit };

//TODO: shouldn't this contain a fill queue?
//TODO: allow changing the execution strategy
//TODO: store performance measures

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
     * @param es execution style 
     * @param log whether to log things or not
     * @param limit_markup whether to be more aggressive (positve) or less aggressive (negative) on your limit order submissions. Ignored if Market exec straty.
     * @param share_thresh only submit order if number of shares is greater than this number
     */
    Portfolio(double starting_cash, 
              std::vector<std::string> tickers, 
              CommissionStyle cs = CommissionStyle::IBFixed, 
              ExecutionStyle es = ExecutionStyle::Market, 
              bool log = false, 
              double limit_markup = 0.0,
              unsigned int share_thresh = 0);
    
    
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
     * @brief calculates the weights you need.
     * @param Sigma the positive definite covariance matrix
     * @param mu the mean returns 
     * @param riskTolerance the nonnegative risk tolerance. must be nonnegative. 0 is the most conservative.
     * @param currentSharePrices the most up-to-date sharePrices
     * @param currentWeights the current weights you have
     */    
    Eigen::VectorXd getWeights1(const Eigen::MatrixXd& Sigma, const Eigen::VectorXd& mu, const double& riskTolerance);

    
    /**
     * @brief take in suggested weights from a statistical model for 
     * one time period ahead in the future, 
     * compute orders, then submit orders to a queue. Also, take the 
     * appropriate action if not all conditions are met.
     * @param ideal_wts_to_be these are based on an optimization procedure (probably) using the mean vector and covariace matrix of the future log returns
     * @param order_q, the order_q you submit orders to
     */
     void react_and_send_orders(Eigen::VectorXd ideal_wts_to_be, ExecHandler& order_q);
 

    /**
     * @brief gets your balance (starting cash plus realized pnls)
     * @return the balance in dollars (double)
     */
    double getBalance() const;

    
    /**
     * @brief gets the outstanding market value of a position on a given symbol
     * @
     */
    double getMktVal(const std::string& sym) const;


    /**
     * @brief gets the number of shares held (negative for short)
     * @param sym the ticker symbol in all caps
     * @return the number of shares as int (shares sold are negative)
     */
    int getNumShares(const std::string& sym) const;


private:
    
    // solves quadratic program with equality constraints
    Eigen::VectorXd quadProg1(const Eigen::MatrixXd& Q, const Eigen::VectorXd& c, const Eigen::MatrixXd&A, const Eigen::VectorXd& b);

    // sends orders based on weights 
    void updateOnNewIdealWts(Eigen::VectorXd ideal_wts_to_be, ExecHandler& order_q);
 
    MarketSnapshot m_last_snapshot;
    std::vector<Instrument> m_ordered_tickers;
    PositionSummary m_pos_summary;
    unsigned int m_todo_step;
    bool m_logging;
    ExecutionStyle m_es;
    double m_limit_markup;
    unsigned int m_share_thresh;
};

#endif // PORTFOLIO_H
