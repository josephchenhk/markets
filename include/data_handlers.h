#ifndef DATA_HANDLERS_H
#define DATA_HANDLERS_H

#include <vector>
#include <string>
#include <Eigen/Dense>

#include "market_snapshot.h"

/**
 * @class DataHandler
 * @author taylor
 * @file data_handlers.h
 * @brief helps convert MarketSnapshot events into Eigen::VectorXds
 */
class DataHandler
{  
public:
    
    /**
     * @brief ctor
     * @param ordered_tickers the order of the info you want
     */
    DataHandler(const std::vector<std::string>& ordered_tickers);
    
    
    /**
     * @brief takes in a MarketSnapshot event for processing
     * @param new_data your most recent MarketSnapshot
     */
    void onData(const MarketSnapshot& new_data);


    /**
     * @brief returns the vector of log returns
     * @return an Eigen::VectorXd of the returns
     */
    Eigen::VectorXd logReturns() const;


    /**
     * @brief returns the vector of arithmetic returns
     * @return an Eigen::VectorXd of returns
     */
    Eigen::VectorXd arithReturns() const;


    /**
     * @brief returns the vector of open prices
     * @return an Eigen::VectorXd of the open prices
     */
    Eigen::VectorXd opens() const;


    /**
     * @brief returns the vector of high prices
     * @return an Eigen::VectorXd of highs
     */
    Eigen::VectorXd highs() const;


    /**
     * @brief returns the vector of low prices
     * @return an Eigen::VectorXd of low prices
     */
    Eigen::VectorXd lows() const;


    /**
     * @brief returns the vector of closes
     * @return an Eigen::VectorXd of close prices
     */
    Eigen::VectorXd closes() const;


private:
    std::vector<std::string> m_ordered_tickers;
    MarketSnapshot m_old_snapshot;
    MarketSnapshot m_new_snapshot;
    unsigned int m_num_data_seen;
};




#endif // DATA_HANDLERS_H
