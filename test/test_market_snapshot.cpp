#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>

#include "instrument.h"
#include "market_bar.h"
#include "market_snapshot.h"

#define PREC .001

// NB: uppercase/lowercase?
// NB: check inconsistent times?

TEST(test_market_snapshot){

    // set up stuff you need
    MarketBar::Time t1 = std::chrono::system_clock::now();
    Instrument instr1("ABC");
    Instrument instr2("XYZ");
    MarketBar bar1(100.0, 101.00, 99.00, 100.0, 1000, t1);
    MarketBar bar2(120.0, 121.00, 119.00, 120.00, 1000, t1);
    std::map<Instrument,MarketBar> test_bars;
    test_bars.insert(std::pair<Instrument,MarketBar>(instr1, bar1));
    test_bars.insert(std::pair<Instrument,MarketBar>(instr2, bar2)); 

    // make market snapshots the two available ways
    MarketSnapshot ms1;
    MarketSnapshot ms2(test_bars);

    // TODO check the times are equal
    CHECK_CLOSE(100, ms2[instr1].open(), PREC);
    CHECK_CLOSE(120, ms2[instr2].open(), PREC);
}
