#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>
#include <iostream>

#include "position_summary.h"


#define PREC .00001
#define OPRICE 100.00
#define EPRICE 100.00
#define NUMSHARES 100
#define CMSN 1
#define PRICEINC 2.00

TEST(test_pos_summary){

    // initialize a position summary object
    std::vector<std::string> tickers;
    tickers.push_back(std::string("ABC"));
    tickers.push_back(std::string("XYZ"));
    double start_cap = 100000;
    CommissionStyle cs = CommissionStyle::IBFixed;
    PositionSummary ps(start_cap, tickers, cs);

    // check everything starts correctly
    CHECK_CLOSE(ps.getBalance(), start_cap, PREC);
    for(auto& ticker : tickers){
        CHECK_CLOSE(ps.getInstrumentMktVal(ticker), 0.0, PREC);
    }

    // add a fill
    Instrument instr("ABC");
    Fill myFill(instr, 
                std::chrono::system_clock::now(),
                Exchange::NYSE,
                NUMSHARES, // shares
                Direction::BOUGHT,
                OPRICE, 
                EPRICE,
                123); // fill id
    ps.onFill(myFill);
 
    // check everything is good after a fill
    CHECK_CLOSE(start_cap - CMSN, ps.getBalance(), PREC);
    CHECK_CLOSE(EPRICE*NUMSHARES, ps.getInstrumentMktVal("ABC"), PREC);
    CHECK_CLOSE(0.0, ps.getInstrumentMktVal("XYZ"), PREC);
    
    // construct a market snapshot to test the "onSnapshot" event
    MarketBar::Time t1 = std::chrono::system_clock::now();
    MarketBar bar(EPRICE+PRICEINC, EPRICE+PRICEINC,EPRICE+PRICEINC,EPRICE+PRICEINC, 1000, t1);
    std::map<Instrument,MarketBar> test_bars;
    test_bars.insert(std::pair<Instrument,MarketBar>(instr, bar));
    MarketSnapshot ms(test_bars);

    // test onSnapshot()
    ps.onSnapshot(ms);

    // check everything is good after running onSnapshot
    CHECK_CLOSE(start_cap - CMSN, // balance only changes with realized profits. so far just a commission
                ps.getBalance(), 
                PREC); 
    CHECK_CLOSE((EPRICE+PRICEINC)*NUMSHARES, // increased unrealized value without commissions
                ps.getInstrumentMktVal("ABC"), 
                PREC);
    CHECK_CLOSE(0.0, ps.getInstrumentMktVal("XYZ"), PREC);
    


}
