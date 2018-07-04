#include "market_bar.h"

#include <stdexcept>
#include <sstream> // stringstream
#include <iomanip> // std::get_time


MarketBar::MarketBar(const double &open, const double &high, const double &low, const double &close, const unsigned int &vol, const Time &close_time)
    : m_open(open), m_high(high), m_low(low), m_close(close), m_volume(vol), m_close_time(close_time)
{
    if (open <= 0.0) throw std::invalid_argument("Open price must be positive");
    if (high <= 0.0) throw std::invalid_argument("High price must be positive");
    if (low <= 0.0) throw std::invalid_argument("Low price must be positive");
    if (close <= 0.0) throw std::invalid_argument("Close price must be positive");
    if (vol < 0) throw std::invalid_argument("Volume must be non-negative");
    if (high < low) throw std::invalid_argument("high must be greater than or equal to low price");
}


MarketBar::MarketBar(const double &open, const double &high, const double &low, const double &close, const unsigned int &vol, const std::string& close_time, const std::string& format)
    : m_open(open), m_high(high), m_low(low), m_close(close), m_volume(vol)
{
    m_close_time = chrono_tp_from_string(close_time, format);
    
    if (open <= 0.0) throw std::invalid_argument("Open price must be positive");
    if (high <= 0.0) throw std::invalid_argument("High price must be positive");
    if (low <= 0.0) throw std::invalid_argument("Low price must be positive");
    if (close <= 0.0) throw std::invalid_argument("Close price must be positive");
    if (vol < 0) throw std::invalid_argument("Volume must be non-negative");
    if (high < low) throw std::invalid_argument("high must be greater than or equal to low price");
}


const double& MarketBar::open() const
{
    return m_open;
}


const double& MarketBar::high() const
{
    return m_high;
}

const double& MarketBar::low() const
{
    return m_low;
}


const double& MarketBar::close() const
{
    return m_close;
}


auto MarketBar::time() const -> const Time&
{
    return m_close_time;
}


auto MarketBar::chrono_tp_from_string(const std::string& str, const std::string& format) -> Time
{
    std::tm tm = {};
    std::stringstream ss(str);
    ss >> std::get_time(&tm, format.c_str());
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return tp;
}
