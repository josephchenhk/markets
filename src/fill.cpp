#include "fill.h"


Fill::Fill(
    const Instrument &instr, 
    const Time &f_time, 
    const Exchange &exch, 
    const unsigned int &qty, 
    const Direction &dir, 
    const double &oPrice, 
    const double &ePrice,
    const unsigned int& o_id)
    : instrument(instr), fill_time(f_time), exchange(exch)
    , quantity(qty), direction(dir), order_price(oPrice)
    , execute_price(ePrice), order_id(o_id)
{    
}


double Fill::getSlippage() const
{
    if(direction == Direction::BOUGHT){
        return execute_price - order_price;
    }else{ // sold or shorted
        return order_price - execute_price;
    }
}

