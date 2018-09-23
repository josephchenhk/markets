#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>

#include "fill.h"

#define PREC .0001
#define OPRICE 99.99
#define EPRICE 100.00

// NB only tests buying

TEST(test_fill){

    Instrument instr("ABC");
    Fill myFill(instr, 
                std::chrono::system_clock::now(),
                Exchange::NYSE,
                100,
                Direction::BOUGHT,
                OPRICE,
                EPRICE,
                123);

	// check slippage works
    CHECK_CLOSE(EPRICE - OPRICE, myFill.getSlippage(), PREC);

}
