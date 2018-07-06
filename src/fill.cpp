#include "fill.h"


Fill::Fill(
    const Instrument &instr, 
    const Time &fill_time, 
    const Exchange &exch, 
    const unsigned int &qty, 
    const Direction &dir, 
    const double &oPrice, 
    const double &ePrice, 
    const double &totalCommission,
    const unsigned int &order_id)
    : m_instr(instr), m_fill_time(fill_time), m_exch(exch)
    , m_qty(qty), m_direction(dir), m_orderPrice(oPrice)
    , m_executePrice(ePrice), m_commission(totalCommission)
    , m_order_id(order_id)
{    
}


double Fill::getSlippage() const
{
    if(m_direction == Direction::BOUGHT){
        return m_executePrice - m_orderPrice;
    }else{ // sold or shorted
        return m_orderPrice - m_executePrice;
    }
}

