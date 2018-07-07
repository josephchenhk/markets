#include "market_snapshot.h"

#include <map>
#include <sstream> // stringstream
#include <iomanip> // std::get_time

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


MarketSnapshot::MarketSnapshot(const std::string& time, const std::map<Instrument,MarketBar> &bars, const std::string& time_format)
    : m_bars(bars) 
{
    // convert time string into m_time
    std::tm tm = {};
    std::stringstream ss(time);
    ss >> std::get_time(&tm, time_format.c_str());
    m_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    
    for (auto &elem : bars){
        if (elem.second.time() != this->m_time) throw std::invalid_argument("Inconsistent time");
    }
}



auto MarketSnapshot::time() const -> MarketBar::Time
{
    return m_time;
}


std::map<Instrument,MarketBar> MarketSnapshot::bars() const
{
    return m_bars;
}


MarketBar& MarketSnapshot::operator[](Instrument instr_name)
{
    return m_bars[instr_name];
}

