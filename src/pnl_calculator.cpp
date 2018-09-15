#include "pnl_calculator.h"

#include <algorithm> // min

pnl_calc::pnl_calc(const std::string& ticker) : m_ticker(ticker), m_qty(0), m_cost(0.0), m_mkt_val(0.0), m_rlzd_pnl(0.0), m_avg_price(0.0)
{    
}


void pnl_calc::on_fill(const int& fill_qty, const double& price, CommissionStyle cstyle)
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

    // subtract commissions
    m_rlzd_pnl -= get_commission(fill_qty, price, cstyle);

    // call this to update market value
    pnl_calc::on_price(price);

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


const Instrument& pnl_calc::instr() const
{
    return m_ticker;
}


const double& pnl_calc::get_mkt_val() const
{
    return m_mkt_val;
}


double pnl_calc::get_commission(const int& qty, const double& price, CommissionStyle cstyle)
{
    if( cstyle == CommissionStyle::IBFixed){
        // North American Fixed pricing scheme for equities 
        // https://www.interactivebrokers.com/en/index.php?f=1590&p=stocks1
        // this ignores the "exceptions" which include API orders
        
        // qty may be negative
        unsigned int abs_qty = std::abs(qty);
        double comm = abs_qty*.005 > 1.0 ? abs_qty*.005 : 1.0;
        double trade_val = abs_qty*price;
        double ib_max = .01*trade_val;
        if(ib_max > 1.0){
            comm = ib_max > comm ? comm : ib_max;
        }

        if(qty < 0){
            comm += 0.0000130 * trade_val; // transaction fees
            comm += 0.000119 * abs_qty; // finra fees
        }
        return comm;
    }
}

