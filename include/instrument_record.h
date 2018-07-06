#ifndef INSTRUMENT_RECORD_H
#define INSTRUMENT_RECORD_H

#include "instrument.h"
#include "fill.h"
#include "market_bar.h"
#include "order.h"

#include <list>

class IRec
{
private:
    Instrument m_instrument;
    double m_ave_price;
    int m_signed_shares; // positive means long, negative means short
    std::list<Order> m_open_orders;

    int getSignedShares(const Fill &fill);
    void deleteOrder(const Fill &fill);

public:

    /**
     * @brief ctor
     * @param instr the instrument to keep track of
    */
    IRec(const Instrument &instr);
    
    /**
     * @brief Updates everything upon receiving new fill
     * @param fill
     */
    void update(const Fill &fill);
    
    
    /**
     * @brief Updates everything upon receiving new market bar
     * @param bar
     */
    void update(const MarketBar &bar);
    
    
    /**
     * @brief Updates everything upon receiving new order event
     * @param order
     */
    void update(const Order &order);
    
};

#endif // INSTRUMENT_RECORD_H