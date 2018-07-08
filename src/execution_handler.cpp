#include "execution_handler.h"

ExecHandler::ExecHandler()
{
}


void ExecHandler::process_orders_yield_fills(MarketSnapshot ms, std::queue<Fill>& fills_q)
{
    while(!m_orders.empty()){

        Order o = m_orders.front();
        
        // process order
        // if market, always completely fill
        if(o.m_otype == OrderType::marketBuy){
            
            //NB: if order is submitted with ridiculous price, it will still fill
            // TODO: what is the purpose of order AND fill id...must they match to show correspondence
            // or must all orders not match with other orders, 
            // or must all fills not match with other fills?
            // or are fill ids just set to the order id so that they can match up with the orders?
            
            // market buys are assumed to always be filled COMPLETELY WITH NO SLIPPAGE
            fills_q.push(Fill(o.m_instr, // order's instrument
                              ms.time(), // current time
                              Exchange::NYSE, // this is just random 
                              o.m_qty,   // full order unsigned quantity
                              Direction::BOUGHT, // not flipping the order's direction...
                              o.m_price, 
                              o.m_price, //ms[o.m_instr].close(), // not current close... asked for price 
                              o.m_id)); // just set it equal to the order ID
        }else if(o.m_otype == OrderType::marketSell){
            
            // TODO: respect difference between DIRECTION::SOLD and DIRECTION::SHORT
            
            // market buys are assumed to always be filled COMPLETELY WITH NO SLIPPAGE
            fills_q.push(Fill(o.m_instr, // order's instrument
                              ms.time(), // current time
                              Exchange::NYSE, // this is just random 
                              o.m_qty,   // full order unsigned quantity
                              Direction::SOLD, 
                              o.m_price, 
                              o.m_price, //ms[o.m_instr].close(), // not current close... asked for price 
                              o.m_id)); // just set it equal to the order ID
            
        }else if(o.m_otype == OrderType::limitBuy){
            // only fill if the high is above the limit
            if(ms[o.m_instr].low() < o.m_price){
                fills_q.push(Fill(o.m_instr, // order's instrument
                                  ms.time(), // current time
                                  Exchange::NYSE, // this is just random 
                                  o.m_qty,   // full order unsigned quantity
                                  Direction::BOUGHT, // pick SOLD instead of SHORT 100% of the time 
                                  o.m_price, 
                                  o.m_price, //ms[o.m_instr].close(), // not current close... asked for price 
                                  o.m_id)); // just set it equal to the order ID                
            }

            
        }else if(o.m_otype == OrderType::limitSell){
            // only fill if the high is above the limit
            if(ms[o.m_instr].high() > o.m_price){
                fills_q.push(Fill(o.m_instr, // order's instrument
                                  ms.time(), // current time
                                  Exchange::NYSE, // this is just random 
                                  o.m_qty,   // full order unsigned quantity
                                  Direction::SOLD, // pick SOLD instead of SHORT 100% of the time 
                                  o.m_price, 
                                  o.m_price, //ms[o.m_instr].close(), // not current close... asked for price 
                                  o.m_id)); // just set it equal to the order ID                
            }

        }else{
            throw std::runtime_error("unexpected order type\n");
        }
        
        // remove order
        m_orders.pop();
    }
}


void ExecHandler::addOrder(const Order& incoming_order)
{
    m_orders.push(incoming_order);
}