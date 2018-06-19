
#ifndef MARKET_BAR_H
#define MARKET_BAR_H

#include <chrono>

/**
 * @class MarketBar
 * @author Benjamin
 * @date 17/05/18
 * @file market_bar.h
 * @brief Represents a summary of market data for a particular instrument over a given
 * period of time
 */

class MarketBar
{
public:

    using Time = std::chrono::time_point<std::chrono::system_clock>;
    
    
    /**
     * @brief Default constructor
     */
    MarketBar() = default;
    
    
    /**
     * @brief Constructs a MarketBar from the provided parameters
     * 
     * @param open denotes the instrument opening price of the period
     * @param high denotes the instrument high price during the period
     * @param low denotes the instrument low price during the period
     * @param close denotes the instrument closing price of the period
     * @param vol denotes the instrument volume over the period
     * @param close_time is the time of the close
     */
    MarketBar(const double &open, const double &high, const double &low, const double &close, const unsigned int &vol, const Time &close_time);
    
    
    /**
     * @brief default copy assignment operator generated by the compiler
     * @param other, the assigner MarketBar object
     */
    MarketBar& operator=(const MarketBar& other) = default;
    
    
    /**
     * @brief gets open
     * @return open
     */
    const double& open() const;
    

    /**
     * @brief gets open
     * @return open
     */
    const double& high() const;



    /**
     * @brief gets open
     * @return open
     */
    const double& low() const;



    /**
     * @brief gets open
     * @return open
     */
    const double& close() const;


    /**
     * @brief time getter
     * @return the time
     */
    const Time& time() const;

    
private:

    double m_open;
    double m_high;
    double m_low;
    double m_close;
    unsigned int m_volume;
    Time m_close_time;

};

#endif // MARKET_BAR_H