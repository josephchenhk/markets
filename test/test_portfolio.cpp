#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>
#include <iostream>
#include <cmath>

#include "portfolio.h"


#define PREC .0001
// TODO: this needs to be more complete
// TODO: test optimization stuff

TEST(test_portfolio){

    // instantiate a portfolio
    double start_cash = 100000;
    std::vector<std::string> tickers;
    tickers.push_back("XYZ");
    tickers.push_back("ABC");
    Portfolio p(start_cash, tickers);

    // instantiate a fake order handler
    double aveSlippage = .01;
    ExecHandler exec_handler(aveSlippage);

    // instantiate order and fill queues
    std::queue<Fill> fill_q;

    // check instantiation was successful
    CHECK_CLOSE(start_cash, 
                p.getBalance(), 
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("XYZ"),
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("ABC"),
                0.0);    
    
// ROUND 1:

    // step 1/3 
    // read some fills
    p.readAllFills(fill_q);

    // check to see nothing was there
    CHECK_CLOSE(start_cash, 
                p.getBalance(), 
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("XYZ"),
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("ABC"),
                0.0);    
    
    // step 2 of 3: 
    // process a market snapshot
    
    // make a market snapshot
    MarketBar::Time t1 = std::chrono::system_clock::now();
    Instrument instr1("ABC");
    Instrument instr2("XYZ");
    MarketBar bar1(100.0, 101.00, 99.00, 100.0, 1000, t1);
    MarketBar bar2(120.0, 121.00, 119.00, 120.00, 1000, t1);
    std::map<Instrument,MarketBar> test_bars;
    test_bars.insert(std::pair<Instrument,MarketBar>(instr1, bar1));
    test_bars.insert(std::pair<Instrument,MarketBar>(instr2, bar2));
    MarketSnapshot ms1(test_bars);

    // read those prices
    p.readNewPrices(ms1);

    // check to make sure nothing happened
    CHECK_CLOSE(start_cash, 
                p.getBalance(), 
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("XYZ"),
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("ABC"),
                0.0);    

    // step 3 of 3: 
    // send it some new weights
    // TODO: check to make sure bogus weights throw an error or somethingi
    // TODO: check other weights
    Eigen::VectorXd ideal_weights = Eigen::VectorXd(2);
    ideal_weights(0) = ideal_weights(1) = .5;
    p.updateOnNewIdealWts(ideal_weights, exec_handler);

    // check to make sure nothing happened yet
    CHECK_CLOSE(start_cash, 
                p.getBalance(), 
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("XYZ"),
                PREC);
    CHECK_CLOSE(0.0,
                p.getMktVal("ABC"),
                0.0);    

    // have execution handler look at some data
    // TODO: this could be a huge bug, if the execution handler breaks
    // then fills won't be seen and all hell breaks loose
    MarketBar::Time t2 = std::chrono::system_clock::now();
    MarketBar bar3(100.0, 101.00, 89.00, 100.0, 1000, t2);
    MarketBar bar4(120.0, 121.00, 109.00, 120.00, 1000, t2);
    std::map<Instrument,MarketBar> test_bars2;
    test_bars2.insert(std::pair<Instrument,MarketBar>(instr1, bar3));
    test_bars2.insert(std::pair<Instrument,MarketBar>(instr2, bar4));
    MarketSnapshot ms2(test_bars2);
    exec_handler.process_orders_yield_fills(ms2, fill_q);


// ROUND TWO

    // step 1/3: 
    // check for fills (should get back two)
    p.readAllFills(fill_q);

    // recall that we put even weight in both stocks
    int correct_shares_abc = std::floor(.5*start_cash/100.00);
    int correct_shares_xyz = std::floor(.5*start_cash/120.00);
    // TODO: investigate a potential other bug: if you try to get balance before you see up-to-date data (eg fill came more recently than data)
    CHECK_CLOSE(correct_shares_abc*100.0,
                p.getMktVal("ABC"),
                PREC);
    CHECK_CLOSE(correct_shares_xyz*120.00,
                p.getMktVal("XYZ"),
                PREC);
 
    // step 2/3 round two: 
    // read in the most recent prices
    MarketBar::Time t3 = std::chrono::system_clock::now();
    MarketBar bar5(1000.0, 1010.00, 990.00, 1000.0, 1000, t3);
    MarketBar bar6(1200.0, 1210.00, 1190.00, 1200.00, 1000, t3);
    std::map<Instrument,MarketBar> test_bars3;
    test_bars3.insert(std::pair<Instrument,MarketBar>(instr1, bar5));
    test_bars3.insert(std::pair<Instrument,MarketBar>(instr2, bar6));
    MarketSnapshot ms3(test_bars3);
    p.readNewPrices(ms3);

    // check new prices register with pnl calculcator
    CHECK_CLOSE(1000*correct_shares_abc,
                p.getMktVal("ABC"),
                PREC);
    CHECK_CLOSE(1200*correct_shares_xyz,
                p.getMktVal("XYZ"),
                PREC);

    // TODO: finish this
    // optimization tests!
    // problem:
    // min .5 x'Px + q'x 
    // st Gx \le h
    // Ax = b
    // 
    // we choose P as 
    //      65  -22  -16
    //     -22   14    7
    //     -16    7    5

    //
    // note that P = Sigma
    // q = - lambda \mu
    //
    //p.getweights(



}
