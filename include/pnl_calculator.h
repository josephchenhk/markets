#ifndef PNL_CALCULATOR_H
#define PNL_CALCULATOR_H

#include "instrument.h"

/** Enum class used for deciding which commission formula. */ 
enum class CommissionStyle {IBFixed};

class pnl_calc{

public:

    /**
     * @brief Default Ctor. Sets everything to 0.0
     */
    pnl_calc();


    /**
     * @brief Call this method every time there is a fill.
     * @param fill_qty signed (negative for sold) amount of shares in the most recent transaction
     * @param price the price (taken or received) for the transaction 
     */
    void on_fill(const int& fill_qty, const double& price, CommissionStyle cstyle);


    /**
     * @brief Call this method every time there is a market price movement.
     * @param price the most up-to-date price of an instrument.
     */
    void on_price(const double& price);
    
    
    /**
     * @brief get the realized pnl
     * @return realized pnl
     */
    const double& get_rpnl() const;

    
    /**
     * @brief get the average price (never negative because cost and qty are always the same sign)
     * @return the average price
     */
    const double& get_avg_price() const;    
    
    
    /**
     * @brief get the current quantity of shares owned (or sold if negative)
     * @return the number of shares as an integer
     */
    const int& get_qty() const;
    
    
    /**
     * @brief get the instrument name
     * @return the name of the instrument
     */
    const Instrument& instr() const;
    
    
    /**
     * @brief gets the current market-value 
     * @return the current market value
     */
    const double& get_mkt_val() const;
    
    
private:
    int m_qty; // this is signed 
    double m_cost; // total dollar amount invested (negative for short)
    double m_mkt_val; // total dollar amount position currently worth
    double m_rlzd_pnl; // realized profit and loss
    double m_avg_price; // cost / qty
    
    int sgn(const double& val); 
    double get_commission(const int& qty, const double& price, CommissionStyle cstyle);
};

#endif // PNL_CALCULATOR_H
