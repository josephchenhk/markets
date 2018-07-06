#include "instrument_record.h"

//IRec::Irec(const Instrument &instr) 
//    : m_instrument(instr), m_ave_price(0.0), m_signed_shares(0)
//{
//}
//
//
//int IRec::getSignedShares(const Fill &fill) // private helper function
//{
//    if(fill.m_direction == Direction::BOUGHT){
//        return fill.m_qty;
//    }else{ // sold or shorted
//        return -1 * fill.m_qty;
//    }
//}
//
//
//void IRec::deleteOrder(const Fill &fill) // private helper function
//{
//    m_open_orders.remove_if([](order)
//        {
//            if(fill.m_order_id == order.id()){ // if the fill id matches the order id
//                return true;
//            }else{
//                return false;
//            }
//        });
//}
//
//
//void IRec::update(const Fill &fill)
//{
//    // remove the corresponding open order
//    // the fill and the order will have the same order_id
//    deleteOrder(fill);
//    
//    // update shares (possibly add or subtract)
//    m_signed_shares += getSignedShares(fill);
//    
//    // update average price
//    
//}
//
//
//void IRec::update(const MarketBar &bar)
//{
//    // update average price
//    
//    // check for take-profit
//    
//    // check for stop-loss
//    
//}
//
//
//void IRec::update(const Order &order)
//{
//    // add open order to the list
//    m_open_orders.push_back(order);
//}