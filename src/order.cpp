#include "order.h"

unsigned int Order::s_totalOrders = 1;

Order::Order(const Instrument &instr, const OrderType &order_type, const double& order_price, const unsigned int &qty)
    : instrument(instr)
    , type(order_type)
    , quantity(qty)
    , price(order_price)
{
    id = s_totalOrders;
    s_totalOrders++;
}

