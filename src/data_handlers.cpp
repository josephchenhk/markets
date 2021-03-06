#include "data_handlers.h"


DataHandler::DataHandler(const std::vector<std::string>& ordered_tickers) : m_ordered_tickers(ordered_tickers), m_num_data_seen(0)
{
}


void DataHandler::onData(const MarketSnapshot& new_data)
{
    if(m_num_data_seen >= 2){
        m_old_snapshot = m_new_snapshot;
        m_new_snapshot = new_data;
    }else if(m_num_data_seen == 1){
        m_old_snapshot = m_new_snapshot;
        m_new_snapshot = new_data;
    }else{
        m_new_snapshot = new_data;
    }
    
    m_num_data_seen++;
}


Eigen::VectorXd DataHandler::logReturns() const
{
    if(m_num_data_seen < 2)
        throw std::runtime_error("there is not enough data to get a return vector");
    
    unsigned int n = m_ordered_tickers.size();
    Eigen::VectorXd rets(n);
    std::map<Instrument,MarketBar> new_bars = m_new_snapshot.bars();
    std::map<Instrument,MarketBar> old_bars = m_old_snapshot.bars();
    for(size_t i = 0; i < n; ++i){
        rets(i) = 100*std::log(new_bars[Instrument(m_ordered_tickers[i])].close()) - 100*std::log(old_bars[Instrument(m_ordered_tickers[i])].close());
    }
    
    
    return rets;
}


Eigen::VectorXd DataHandler::arithReturns() const
{
    if(m_num_data_seen < 2)
        throw std::runtime_error("there is not enough data to get a return vector");

    unsigned int n = m_ordered_tickers.size();
    Eigen::VectorXd rets(n);
    std::map<Instrument,MarketBar> new_bars = m_new_snapshot.bars();
    std::map<Instrument,MarketBar> old_bars = m_old_snapshot.bars();
    for(size_t i = 0; i < n; ++i){
        rets(i) = 100*(new_bars[Instrument(m_ordered_tickers[i])].close() - old_bars[Instrument(m_ordered_tickers[i])].close())/(old_bars[Instrument(m_ordered_tickers[i])].close());

    }
    return rets;
}


Eigen::VectorXd DataHandler::opens() const
{
    if(m_num_data_seen == 0) throw std::runtime_error("you need at least 1 datum to look at the opens");

    unsigned int n = m_ordered_tickers.size();
    Eigen::VectorXd opens(n);
    std::map<Instrument,MarketBar> new_bars = m_new_snapshot.bars();
    for(size_t i = 0; i < n; ++i){
        opens(i) = new_bars[Instrument(m_ordered_tickers[i])].open();
    }
    return opens;
}


Eigen::VectorXd DataHandler::highs() const
{
    if(m_num_data_seen == 0)
        throw std::runtime_error("you need atleast 1 datum to look at highs");
        
    unsigned int n = m_ordered_tickers.size();
    Eigen::VectorXd highs(n);
    std::map<Instrument,MarketBar> new_bars = m_new_snapshot.bars();
    for(size_t i = 0; i < n; ++i)
        highs(i) = new_bars[Instrument(m_ordered_tickers[i])].high();
    
    return highs;
}


Eigen::VectorXd DataHandler::lows() const
{
    if(m_num_data_seen == 0)
        throw std::runtime_error("you need at least 1 datum to look at the lows");
        
    unsigned int n = m_ordered_tickers.size();
    Eigen::VectorXd lows(n);
    std::map<Instrument,MarketBar> new_bars = m_new_snapshot.bars();
    for(size_t i = 0; i < n; ++i)
        lows(i) = new_bars[Instrument(m_ordered_tickers[i])].low();
        
    return lows;
}


Eigen::VectorXd DataHandler::closes() const
{
    if(m_num_data_seen == 0)
        throw std::runtime_error("you need atleast 1 datum to look at closes");

    unsigned int n = m_ordered_tickers.size();
    Eigen::VectorXd closes(n);
    std::map<Instrument,MarketBar> new_bars = m_new_snapshot.bars();
    for(size_t i = 0; i < n; ++i)
        closes(i) = new_bars[Instrument(m_ordered_tickers[i])].close();
        
    return closes;
}


