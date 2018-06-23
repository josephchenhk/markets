#ifndef PNL_CALCULATOR_H
#define PNL_CALCULATOR_H

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
    void on_fill(const int& fill_qty, const double& price);
    
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
    
    
private:
    int m_qty; // this is signed 
    double m_cost; // total dollar amount invested (negative for short)
    double m_mkt_val; // total dollar amount currently worth
    double m_rlzd_pnl; // realized profit and loss
    double m_avg_price; // cost / qty
    int sgn(const double& val); 
};

#endif // PNL_CALCULATOR_H