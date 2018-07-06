#include "position_summary.h"


PositionSummary::PositionSummary(const std::vector<std::string>& tickers, CommissionStyle cs) : m_cs(cs)
{
    for(auto ticker : tickers)
        m_positions.push_back(pnl_calc(ticker));
}


void PositionSummary::onSnapshot(const MarketSnapshot& ms)
{
    for(auto& pos : m_positions){
        double last_price = ms.bars().at(pos.instr()).close();
        pos.on_price(last_price);
    }
}


void PositionSummary::onFill(const Fill& fill)
{
    for(auto& pos : m_positions){
        if(pos.instr() == fill.m_instr){
            int signed_qty = (fill.m_direction == Direction::BOUGHT) ? fill.m_qty : -1*(int)fill.m_qty;
            pos.on_fill(signed_qty, fill.m_executePrice, m_cs);
        }else{
            throw std::invalid_argument("fill is for a symbol not tracked by your positions");
        }
    }
}


