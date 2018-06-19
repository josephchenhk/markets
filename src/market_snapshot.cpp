#include "market_snapshot.h"

#include <map>

#include "instrument.h"
#include "market_bar.h"

MarketSnapshot::MarketSnapshot(const MarketBar::Time &time, const std::map<Instrument,MarketBar> &bars)
    : m_time(time), m_bars(bars) 
{
    // TODO: do we even need MarketSnapshot to have its own time member, then?
    for (auto &elem : bars){
        if (elem.second.time() != this->m_time) throw std::invalid_argument("Inconsistent time");
    }
}