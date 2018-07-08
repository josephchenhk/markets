#include "order.h"

unsigned int Order::totalOrders = 1;

Order::Order(const Instrument &instr, const OrderType &otype, const double& price, const unsigned int &qty)
    : m_instr(instr)
    , m_otype(otype)
    , m_qty(qty)
    , m_price(price)
{
    m_id = totalOrders;
    totalOrders++;
}

unsigned int Order::id() const
{
    return m_id;
}

