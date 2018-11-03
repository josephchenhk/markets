#include "position_summary.h"


PositionSummary::PositionSummary(double initial_capital, const std::vector<std::string>& tickers, CommissionStyle cs) : m_starting_cash(initial_capital), m_cs(cs)
{
    for(auto ticker : tickers){
        Instrument instr(ticker);
        pnl_calc pc;
        m_positions.insert(std::pair<Instrument,pnl_calc>(instr, pc));
    }
}


void PositionSummary::onSnapshot(const MarketSnapshot& ms)
{
    // for each one of your positions, get the relevant bar, and register the last price
    // if the bar cannot be found, do nothing
    for(auto& pos : m_positions){

        auto right_bar = ms.bars().find(pos.first);
        if( right_bar != ms.bars().end() ){ // if the key is found in the data
            pos.second.on_price(right_bar->second.close());
        }else{ // if key is not found in data
            throw std::runtime_error("error! data are not being found/recognized\n");
        }
    }
}


void PositionSummary::onFill(const Fill& fill)
{
    int signed_qty  = (fill.direction == Direction::BOUGHT) ? fill.quantity : -1*(int)fill.quantity;
    m_positions.at(fill.instrument).on_fill(signed_qty, fill.execute_price, m_cs);
    // TODO: take into account unexpected instrument fills
}


double PositionSummary::getInstrumentMktVal(const Instrument& instr) const
{
    return m_positions.at(instr).get_mkt_val();
}


double PositionSummary::getInstrumentMktVal(const std::string& sym) const
{
    Instrument instr(sym);
    return PositionSummary::getInstrumentMktVal(instr);
}


int PositionSummary::getInstrumentShares(const Instrument& instr) const
{

    return m_positions.at(instr).get_qty();
}


int PositionSummary::getInstrumentShares(const std::string& sym) const
{
    Instrument instr(sym);
    return PositionSummary::getInstrumentShares(instr);
}


double PositionSummary::getBalance() const
{
    double balance = m_starting_cash;
    for(auto& pos : m_positions){
        balance += pos.second.get_rpnl();
    }
    return balance;
}

