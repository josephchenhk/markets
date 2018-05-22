#include "instrument.h"

enum class OrderType {marketBuy, marketSell};

class Order
{
    static unsigned int totalOrders;
    unsigned int m_id;
    
public:
    
    // data 
    const Instrument m_instr;
    const OrderType m_otype;
    const unsigned int m_qty;
    
    
    /**
     * @brief ctor
     */
    Order(const Instrument &instr, const OrderType &otype, const unsigned int &qty);
    
    
    /**
     * @brief get the order id
     * @return the order id
     */
    unsigned int id() const;
};

