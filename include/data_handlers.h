#ifndef DATA_HANDLERS_H
#define DATA_HANDLERS_H

#include <vector>
#include <string>
#include <Eigen/Dense>

#include "market_snapshot.h"

class DataHandler
{
private:
    std::vector<std::string> m_ordered_tickers;
    MarketSnapshot m_old_snapshot;
    MarketSnapshot m_new_snapshot;
    unsigned int m_num_data_seen;
    
public:
    DataHandler(const std::vector<std::string>& ordered_tickers);
    void onData(const MarketSnapshot& new_data);
    Eigen::VectorXd logReturns() const;
    Eigen::VectorXd lows() const;
};




#endif // DATA_HANDLERS_H