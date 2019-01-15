#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>
#include <vector>
#include <string>

#include "data_handlers.h"

#define PREC .0001

// this ordered ticker thing could break...make sur eto test that
// TODO: test for errors to be thrown
// TODO: test for time duration to be positive


TEST(test_data_handler){

    // make some test tickers
    std::vector<std::string> tickers;
    tickers.push_back(std::string("ABC"));
    tickers.push_back(std::string("WXYZ"));
    
    // make a test market snapshot
    MarketBar::Time t1 = std::chrono::system_clock::now();
    Instrument instr1(tickers[0]);
    Instrument instr2(tickers[1]);
    MarketBar bar1(100.0, 101.00, 99.00, 100.0, 1000, t1);
    MarketBar bar2(120.0, 121.00, 119.00, 120.00, 1000, t1);
    std::map<Instrument,MarketBar> test_bars;
    test_bars.insert(std::pair<Instrument,MarketBar>(instr1, bar1));
    test_bars.insert(std::pair<Instrument,MarketBar>(instr2, bar2)); 
    MarketSnapshot ms1(test_bars);
    
    // make the data handler finally
    DataHandler dh(tickers);

    // add some data
    dh.onData(ms1);

    // test everyting except log returns
    CHECK_CLOSE(100.0, dh.opens()(0), PREC);
    CHECK_CLOSE(120.0, dh.opens()(1), PREC);
    CHECK_CLOSE(101.0, dh.highs()(0), PREC);
    CHECK_CLOSE(121.0, dh.highs()(1), PREC);
    CHECK_CLOSE(99.0, dh.lows()(0), PREC);
    CHECK_CLOSE(119.0, dh.lows()(1), PREC);
    CHECK_CLOSE(100.0, dh.closes()(0), PREC);
    CHECK_CLOSE(120.0, dh.closes()(1), PREC);

    // make another market snapshot
    //t1+=1;
    MarketBar bar3(101.0, 102.00, 100.00, 101.0, 1000, t1);
    MarketBar bar4(121.0, 122.00, 120.00, 121.00, 1000, t1);
    std::map<Instrument,MarketBar> test_bars2;
    test_bars2.insert(std::pair<Instrument,MarketBar>(instr1, bar3));
    test_bars2.insert(std::pair<Instrument,MarketBar>(instr2, bar4)); 
    MarketSnapshot ms2(test_bars2);

    // add second data point
    dh.onData(ms2);

    // test ohlc AND log returns
    CHECK_CLOSE(101.0, dh.opens()(0), PREC);
    CHECK_CLOSE(121.0, dh.opens()(1), PREC);
    CHECK_CLOSE(102.0, dh.highs()(0), PREC);
    CHECK_CLOSE(122.0, dh.highs()(1), PREC);
    CHECK_CLOSE(100.0, dh.lows()(0), PREC);
    CHECK_CLOSE(120.0, dh.lows()(1), PREC);
    CHECK_CLOSE(101.0, dh.closes()(0), PREC);
    CHECK_CLOSE(121.0, dh.closes()(1), PREC);
    CHECK_CLOSE(100*std::log(101.0) - 100.0*std::log(100.00), dh.logReturns()(0), PREC);
    CHECK_CLOSE(100.0*std::log(121.0) - 100.0*std::log(120.00), dh.logReturns()(1), PREC);

}
