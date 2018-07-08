#ifndef EXECUTION_HANDLER_H
#define EXECUTION_HANDLER_H

#include <queue>
#include "order.h"
#include "fill.h"
#include "market_snapshot.h"


/**
 * @class ExecHandler
 * @author t
 * @date 08/07/18
 * @file execution_handler.h
 * @brief 
 */
class ExecHandler{

private:
    std::queue<Order> m_orders;
    
public:
    ExecHandler();
    void process_orders_yield_fills(MarketSnapshot ms, std::queue<Fill>& fills_q);
    void addOrder(const Order& incoming_order);
    
};


#endif // EXECUTION_HANDLER_H