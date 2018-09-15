#include "market_snapshot.h"

#include <map>
#include <sstream> // stringstream
#include <iomanip> // std::get_time

#include "instrument.h"
#include "market_bar.h"


MarketSnapshot::MarketSnapshot(const std::map<Instrument,MarketBar> &bars)
    : m_bars(bars) 
{
    // take the first element's time and make sure everything is the same
    MarketBar::Time firstTime = m_bars.begin()->second.time();
    for (auto &elem : bars){
        if (elem.second.time() != firstTime) throw std::invalid_argument("Inconsistent time");
    }
}


auto MarketSnapshot::time() const -> MarketBar::Time
{
    return m_bars.begin()->second.time();
}


std::map<Instrument,MarketBar> MarketSnapshot::bars() const
{
    return m_bars;
}


MarketBar& MarketSnapshot::operator[](Instrument instr_name)
{
    return m_bars[instr_name];
}


MarketBar& MarketSnapshot::operator[](std::string instr_name)
{
    return m_bars[Instrument(instr_name)];
}
