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
   
public:

    /**
     * @brief ctor
     * @param marketSlippage is the dollar amount of slippage that will be assessed
     */
    ExecHandler(const double& marketSlippage = .01);


    /**
     * @brief process orders to see if they were filled
     * @param ms the most recent MarketSnapshot
     * @param fills_q is the place that fills will be put
     */
    void process_orders_yield_fills(MarketSnapshot ms, std::queue<Fill>& fills_q);


    /**
     * @brief add an order to the queue
     * @brief incoming_order an order that may or may not be triggered
     */
    void addOrder(const Order& incoming_order);

private:
    std::queue<Order> m_orders;
    double m_ave_slip;    
};


#endif // EXECUTION_HANDLER_H
