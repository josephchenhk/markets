#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>

#include "order.h"

#define PREC .0001
#define QTY 100
#define PRICE 15999.97

TEST(test_order){

    Instrument instr("ABC");
    Order o(instr, 
            OrderType::marketBuy,
            PRICE,
            QTY);

    // check all public members
    CHECK(o.m_instr == instr);
    CHECK(o.m_otype == OrderType::marketBuy);
    CHECK_EQUAL(QTY, o.m_qty);
    CHECK_CLOSE(PRICE, o.m_price, PREC);

}
