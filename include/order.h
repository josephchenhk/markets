#ifndef ORDER_H
#define ORDER_H

#include "instrument.h"

enum class OrderType {marketBuy, marketSell, limitBuy, limitSell};

class Order
{
    static unsigned int totalOrders;
    
public:
    
    // data 
    const Instrument m_instr;
    const OrderType m_otype;
    const unsigned int m_qty;
    double m_price; 
    unsigned int m_id;
    
    
    /**
     * @brief ctor
     */
    Order(const Instrument &instr, const OrderType &otype, const double& price, const unsigned int &qty);
    
    
    /**
     * @brief get the order id
     * @return the order id
     */
    unsigned int id() const;
};

#endif // ORDER_H