#ifndef EXECUTION_HANDLER_H
#define EXECUTION_HANDLER_H

#include <queue>
#include "order.h"
#include "fill.h"
#include "market_snapshot.h"

class ExecHandler{

private:
    std::queue<Order> m_orders;
    
public:
    ExecHandler();
    void process_orders_yield_fills(MarketSnapshot ms, std::queue<Fill>& fills_q);
    
};


#endif // EXECUTION_HANDLER_H