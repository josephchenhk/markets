#include "pnl_calculator.h"

#include <algorithm> // min

pnl_calc::pnl_calc() : m_qty(0), m_cost(0.0), m_mkt_val(0.0), m_rlzd_pnl(0.0), m_avg_price(0.0)
{    
}


void pnl_calc::on_fill(const int& fill_qty, const double& price)
{
    int direction = sgn(fill_qty);
    int prev_direction = sgn(m_qty); 
    
    
    int qty_opening, qty_closing;
    if(prev_direction == direction){ 
        qty_closing = 0;
        qty_opening = fill_qty; 
    }else{  
        qty_closing = std::min(std::abs(m_qty), std::abs(fill_qty)) * direction;  // first case is reversal, second is a partial closeout
        qty_opening = fill_qty - qty_closing;
    }
    
    double new_cost = m_cost + qty_opening*price;
    if(m_qty != 0){
        new_cost += qty_closing*m_cost/m_qty;
        m_rlzd_pnl += qty_closing*(m_cost/m_qty - price);
    }
    
    m_qty += fill_qty;
    m_cost = new_cost;    

    if(m_qty != 0){
        m_avg_price = m_cost / m_qty;
    }else{
        m_avg_price = 0.0;
    }
    
}
    

void pnl_calc::on_price(const double& price)
{
    m_mkt_val = m_qty * price;
}


int pnl_calc::sgn(const double& val) 
{
    return (0.0 < val) - (val < 0.0);
}


const double& pnl_calc::get_rpnl() const
{
    return m_rlzd_pnl;
}


const double& pnl_calc::get_avg_price() const
{
    return m_avg_price;
}


const int& pnl_calc::get_qty() const
{
    return m_qty;
}
