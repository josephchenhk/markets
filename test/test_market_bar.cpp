#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>

#include "market_bar.h"

#define PREC .0001

// NB1: does not test time

TEST(test_market_bar){

    MarketBar mb1;
    MarketBar mb2(100.00, 100.10, 99.90, 100.00, 1000, std::chrono::system_clock::now());
    MarketBar mb3(100.00, 100.10, 99.90, 100.00, 1000, "2018-12-23 09:30:00");
    MarketBar mb4 = mb3;

    CHECK_CLOSE(100.00, mb2.open(), PREC);
    CHECK_CLOSE(100.00, mb3.open(), PREC);
    CHECK_CLOSE(100.00, mb4.open(), PREC);

    CHECK_CLOSE(100.10, mb2.high(), PREC);
    CHECK_CLOSE(100.10, mb3.high(), PREC);
    CHECK_CLOSE(100.10, mb4.high(), PREC);
    
    CHECK_CLOSE(99.90, mb2.low(), PREC);
    CHECK_CLOSE(99.90, mb3.low(), PREC);
    CHECK_CLOSE(99.90, mb4.low(), PREC);
   
    CHECK_CLOSE(100.00, mb2.close(), PREC);
    CHECK_CLOSE(100.00, mb3.close(), PREC);
    CHECK_CLOSE(100.00, mb4.close(), PREC);

}
