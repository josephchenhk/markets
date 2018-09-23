#ifndef FILL_H
#define FILL_H

#include <chrono>
#include "instrument.h"

enum class Exchange {NYSE, NASDAQ};
enum class Direction {BOUGHT, SOLD, SHORT};

class Fill
{
public:

    /* type alias for time stuff */
    using Time = std::chrono::time_point<std::chrono::system_clock>;


    /**
     * @brief ctor
     * @param instr an Instrument object
     * @param fill_time time order was filled
     * @param exch the exchange it was filled on
     * @param qty the number of shares
     * @param dir whether they were bought, sold or shorted
     * @param oPrice the price on the order. 
     * @param ePrice the price that was executed
     * @param order_id a unique order id number
     */
    Fill(
        const Instrument &instr, 
        const Time &f_time, 
        const Exchange &exch, 
        const unsigned int &qty, 
        const Direction &dir, 
        const double &oPrice, 
        const double &ePrice,
        const unsigned int& o_id);

    /**
     * @brief gets the slippage
     * @return how much slippage you endured (as a positive number)
     */
    double getSlippage() const;

    /* instrument  */
    const Instrument instrument;

    /* the time of fill */
    const Time fill_time;

    /* exchange filled on*/
    const Exchange exchange;

    /* number of shares */
    const unsigned int quantity;

    /* bought, sold, or shorted? */
    const Direction direction;

    /* price on the order  */
    const double order_price;

    /* price filled at */
    const double execute_price;

    /* unique id number that matches with the order id  */
    const unsigned int order_id;

};


#endif // FILL_H
