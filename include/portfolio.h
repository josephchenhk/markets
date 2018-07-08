#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <Eigen/Dense>
#include <queue>

#include "position_summary.h"
#include "order.h"


/**
 * @class Portfolio
 * @author t
 * @date 06/07/18
 * @file portfolio.h
 * @brief This represents the top level of all accounting details. 
 * 1. (or 2.) Read all fills and update positions accordingly (readAllFills)
 * 2. (or 1.) Read in the most recent prices and update positions/info accordingly ( readNewPrices ) 
 * 3. Receive new ideal weights, calculate orders, and then submit these orders. (updateOnNewIdealWts)
 */
template<size_t dimobs>
class Portfolio 
{
    
private:
    double m_liquidation_value;
    MarketSnapshot m_last_snapshot;
    std::vector<Instrument> m_ordered_tickers;
    PositionSummary m_pos_summary;
    unsigned int m_todo_step;

public:

    // ctor
    Portfolio(const double& starting_cash, const std::vector<std::string>& tickers, CommissionStyle cs = CommissionStyle::IBFixed);
    
    // call these in order
    void readAllFills(std::queue<Fill>& q);
    void readNewPrices(MarketSnapshot ms);
    void updateOnNewIdealWts(const Eigen::Matrix<double,dimobs,1>& ideal_wts_to_be, std::queue<Order>& order_q);
};




template<size_t dimobs>
Portfolio<dimobs>::Portfolio(const double& starting_cash, const std::vector<std::string>& tickers, CommissionStyle cs) 
    : m_liquidation_value(starting_cash)
    , m_pos_summary(tickers, cs)
    , m_todo_step(1)
{
    if(tickers.size() != dimobs)
        throw std::invalid_argument("too many or too few tickers");

    for(size_t i = 0; i < dimobs; ++i)
        m_ordered_tickers.push_back(Instrument(tickers[i]));

}


template<size_t dimobs>
void Portfolio<dimobs>::readAllFills(std::queue<Fill>& q)
{
    if(m_todo_step != 1)
        throw std::runtime_error("Portfolio methods are being called out of order");
    
    while(!q.empty()){
        std::cout << "reading a fill!\n";
        m_pos_summary.onFill(q.front());
        q.pop();
    }
    
    // allow the next method to execute
    m_todo_step = 2;
}


template<size_t dimobs>
void Portfolio<dimobs>::readNewPrices(MarketSnapshot ms)
{
    if(m_todo_step != 2)
        throw std::runtime_error("Portfolio methods are being called out of order");

    m_last_snapshot = ms;
    m_pos_summary.onSnapshot(ms);    
    
    // allow the next method to execute
    m_todo_step = 3;
}


template<size_t dimobs>
void Portfolio<dimobs>::updateOnNewIdealWts(const Eigen::Matrix<double,dimobs,1>& ideal_wts_to_be, std::queue<Order>& order_q)
{

    // generate and submit orders based on these new ideal weights.
    // to calculate the order_qty, keep in mind that:
    // 
    //             signed_order_qty * this_order_price + current_position_mkt_value <= ideal_wt * liquidation_value
    //
    // where current_position_mkt_value = current_position_signed_qty * current_ave_price
    
    // NB1: the fixed order_price might be "outdated" if it's based on the previously most recent price
    // NB3: this function assumes that it is only called after the previous UNFILLED order is expired. 
    // It does not check for unfilled orders...just previously filled orders (via the position). 
    // If this function were called too quickly, it would potentially submit multiple orders on 
    // top of other orders for the same symbols. 
    
    if(m_todo_step != 3)
        throw std::runtime_error("Portfolio methods are being called out of order");
    
    int signed_qty;
    unsigned int pos_qty;
    for(size_t i = 0; i < m_ordered_tickers.size(); ++i){
        
        Instrument instr(m_ordered_tickers[i]);
        double this_orders_price = m_last_snapshot[instr].close();
        double current_position_value = m_pos_summary.getInstrumentsMktVal(instr);
        signed_qty = std::trunc((m_liquidation_value*ideal_wts_to_be[i] - current_position_value)/this_orders_price);
        
        if(signed_qty < 0){ // sell order
            pos_qty = -signed_qty;
            order_q.push(Order(instr, OrderType::limitSell, this_orders_price, pos_qty));
            std::cout << "successfully submitted an order!\n";
            
        }else{ // buy order
            pos_qty = signed_qty;
            order_q.push(Order(instr, OrderType::limitBuy, this_orders_price, pos_qty));
            std::cout << "successfully submitted an order!\n";
        }
    }
    
    // allow next method to execute
    m_todo_step = 1;
}





#endif // PORTFOLIO_H