
#ifndef POSITION_SUMMARY_H
#define POSITION_SUMMARY_H

#include <map>
#include <instrument.h>
#include <market_bar.h>
#include <stdexcept>

enum PositionType {LONG, SHORT, NONE}

/**
 * @class PositionSummary
 * @author Benjamin
 * @date 24/05/18
 * @file position_summary.h
 * @brief Provides summary info for a particular position
 */

class PositionSummary
{
public:

    // summary info
    const PositionType m_position_type;
    const unsigned m_quantity;
    const double m_floating_pl;
    
    /**
     * @brief Constructs a new PositionSummary with the provided information
     * @param position_type denotes the type of aggregate position taken
     * @param quantity denotes the net quantity of the position
     * @param floating_pl denotes the current profit/loss of this position
     */
    
    PositionSummary(PositionType position_type, 
                    unsigned quantity, 
                    double floating_pl): m_position_type(position_type), 
                                         m_quantity(quantity), 
                                         m_floating_pl(floating_pl) 
    {
        if (quantity < 0) throw std::invalid_argument("Quantity must be positive");
    }
};

#endif // POSITION_SUMMARY_H