#ifndef FILL_H
#define FILL_H

#include <chrono>
#include "instrument.h"

enum class Exchange {NYSE, NASDAQ};
enum class Direction {BOUGHT, SOLD, SHORT};

class Fill
{
public:

    using Time = std::chrono::time_point<std::chrono::system_clock>;

    const Instrument m_instr;
    const Time m_fill_time;
    const Exchange m_exch;
    const unsigned int m_qty;
    const Direction m_direction;
    const double m_orderPrice;
    const double m_executePrice;
    const unsigned int m_order_id;

    /**
     * @brief ctor
     */
    Fill(
        const Instrument &instr, 
        const Time &fill_time, 
        const Exchange &exch, 
        const unsigned int &qty, 
        const Direction &dir, 
        const double &oPrice, 
        const double &ePrice, 
        const unsigned int &order_id);

    /**
     * @brief gets the slippage
     * @return how much slippage you endured (as a positive number)
     */
    double getSlippage() const;

};


#endif // FILL_H