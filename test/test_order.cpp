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
    CHECK(o.instrument == instr);
    CHECK(o.type == OrderType::marketBuy);
    CHECK_EQUAL(QTY, o.quantity);
    CHECK_CLOSE(PRICE, o.price, PREC);

}
