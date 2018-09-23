#ifndef ORDER_H
#define ORDER_H

#include "instrument.h"

enum class OrderType {marketBuy, marketSell, limitBuy, limitSell};


/**
 * @class Order
 * @author t
 * @file order.h
 * @brief an order class with const public data members
 */
class Order
{

private:

    static unsigned int s_totalOrders;
    
public:
    
   /**
     * @brief ctor
     * @param instr the instrument 
     * @param order_type the type of order
     * @param order_price the order price (NB: if market order, this only is for slippage calc purposes)
     * @param qty the number of shares
     */
    Order(const Instrument &instr, const OrderType &order_type, const double& order_price, const unsigned int &qty);
    
    /* instrument */ 
    const Instrument instrument;

    /* order type */
    const OrderType type;

    /* number of shares ordered */
    const unsigned int quantity;

    /* the order price */
    double price; 

    /* the unique order id */
    unsigned int id;
};

#endif // ORDER_H
