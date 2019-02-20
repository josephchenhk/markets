#include "position_summary.h"
#include <iostream> // std::cerr

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
    // TODO: what happens when bar cannot be found?
    for(auto& pos : m_positions){
	    Instrument symbol = pos.first;
	    
        // wrap this in a try block because sometimes the snapshots won't have all the data that we're keeping track of in our portfolio
        try
        {
            MarketBar data_bar = ms.bars().at(symbol);	
            double cl = data_bar.close();
	        pos.second.on_price(cl);	
        }
        catch (std::out_of_range& e) 
        {
            //std::cerr << e.what() << "PositionSummary::onSnapshot() is checking for data that doesn't exist (which is fine)\n";
        }
   }
}


//void PositionSummary::onSnapshot(const MarketSnapshot& ms)
//{
//    // for each one of your positions, get the relevant bar, and register the last price
//    // if the bar cannot be found, do nothing
//    for(auto& pos : m_positions){
//
//        auto right_bar = ms.bars().find(pos.first);
//        if( right_bar != ms.bars().end() ){ // if the key is found in the data
//            
//            pos.second.on_price(right_bar->second.close());
//            std::cerr << "PositionSummary::onSnapshot() " << "pos: " << pos.first.symbol << ", price: " << right_bar->second.close() << "\n";
//        }else{ // if key is not found in data
//            throw std::runtime_error("error! data are not being found/recognized\n");
//        }
//   }
//}
//

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

