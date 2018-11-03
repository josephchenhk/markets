#include "execution_handler.h"


ExecHandler::ExecHandler(const double& marketSlippage, bool log) : m_ave_slip(marketSlippage), m_logging(log)
{
    if(m_ave_slip < 0)
        throw std::invalid_argument("slippage must be positive\n");
}


void ExecHandler::process_orders_yield_fills(MarketSnapshot ms, std::queue<Fill>& fills_q)
{
    while(!m_orders.empty()){

        Order o = m_orders.front();
        
        // process order
        // if market, always completely fill, but add user-chosen-slippage
        if(o.type == OrderType::marketBuy){
            
            //NB: if order is submitted with ridiculous price, it will still fill, but slippage calculation will be erroneous
            
            // TODO: what is the purpose of order AND fill id...must they match to show correspondence
            // or must all orders not match with other orders, 
            // or must all fills not match with other fills?
            // or are fill ids just set to the order id so that they can match up with the orders?
            
            // TODO: disallow ridiculous order prices

            // market buys are assumed to always be filled COMPLETELY, but there will be slippage
            fills_q.push(Fill(o.instrument, // order's instrument
                              ms.time(), // current time
                              Exchange::NYSE, // this is just random 
                              o.quantity,   // full order unsigned quantity
                              Direction::BOUGHT, // not flipping the order's direction...
                              o.price, // order price
                              o.price + m_ave_slip, // execute price 
                              o.id)); // set it equal to the order ID
        }else if(o.type == OrderType::marketSell){
            
            // TODO: respect difference between DIRECTION::SOLD and DIRECTION::SHORT
            
            // market buys are assumed to always be filled COMPLETELY WITH NO SLIPPAGE
            fills_q.push(Fill(o.instrument, // order's instrument
                              ms.time(), // current time
                              Exchange::NYSE, // this is just random 
                              o.quantity,   // full order unsigned quantity
                              Direction::SOLD, 
                              o.price,  // order price
                              o.price - m_ave_slip, // execute price 
                              o.id)); // just set it equal to the order ID
            
        }else if(o.type == OrderType::limitBuy){

            // only fill if the high is above the limit
            if(ms[o.instrument].low() < o.price){
                fills_q.push(Fill(o.instrument, // order's instrument
                                  ms.time(), // current time
                                  Exchange::NYSE, // this is just random 
                                  o.quantity,   // full order unsigned quantity
                                  Direction::BOUGHT, // pick SOLD instead of SHORT 100% of the time 
                                  o.price, // order price
                                  o.price, // execute price (same because limit order)  
                                  o.id)); // just set it equal to the order ID                
            }else{

                // TODO: think more carefully about what you want todo here, and whether the order should stay in the queue
                if(m_logging)
                    std::cerr << "limit buy failed to fill for " << o.instrument.symbol << "\n";
            }

            
        }else if(o.type == OrderType::limitSell){
            // only fill if the high is above the limit
            if(ms[o.instrument].high() > o.price){
                fills_q.push(Fill(o.instrument, // order's instrument
                                  ms.time(), // current time
                                  Exchange::NYSE, // this is just random 
                                  o.quantity,   // full order unsigned quantity
                                  Direction::SOLD, // pick SOLD instead of SHORT 100% of the time 
                                  o.price, // order price 
                                  o.price, // execute price (same because limit order) 
                                  o.id)); // just set it equal to the order ID                
            }else{

                // TODO: think more carefully about wha tyou want to do here... should the order stay here or disappear
                
                if(m_logging)
                    std::cerr << "limit buy failed to fill for " << o.instrument.symbol << "\n";
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
