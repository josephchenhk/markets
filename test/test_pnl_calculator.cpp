#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>

#include "pnl_calculator.h"

// NB1: BUYQTY must be divisible by 2
// NB2: this assumes you're triggering the minimum commission from IB
// NB3: hardcoding selling/FINRA fees
// NB4: doesn't really test the average price too hard

#define PREC .0001
#define BUYQTY 100
#define BUYPRICE 50.0
#define PRICEINC 1.0
#define COMM 1.0

#include <iostream> // in case you need std::cerr, otherwise unnecessary

TEST(successful_buy){
    
    // this test checks the 
    // quantity, market value, realized pnl, and average cost 
    // at each stage in the process of a successul long position

    // initialize
    pnl_calc mc;
   
    // check that you'r flat before you buy anything
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(0.0, mc.get_rpnl(), PREC);
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);

    // step 1: buy 100 shares at $50
    mc.on_fill(BUYQTY, BUYPRICE, CommissionStyle::IBFixed);    

    // check all four
    CHECK_CLOSE(BUYQTY*BUYPRICE, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(BUYQTY, mc.get_qty()); 
    CHECK_CLOSE(-COMM, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC);

    // let the price move with you a dollar
    double newPrice = BUYPRICE + PRICEINC;
    mc.on_price(newPrice);

    // check market value, qty, and realized pnl
    CHECK_CLOSE(newPrice*BUYQTY, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(BUYQTY, mc.get_qty());
    CHECK_CLOSE(-COMM, mc.get_rpnl(), PREC);

    // close half the position
    mc.on_fill(-BUYQTY/2, newPrice, CommissionStyle::IBFixed);
    double extraSellComm = 0.0000130*BUYQTY/2*newPrice;
    extraSellComm += 0.000119 * BUYQTY/2;

    // check everything again
    CHECK_CLOSE(BUYQTY/2*newPrice, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(BUYQTY/2, mc.get_qty());
    CHECK_CLOSE(-2*COMM-extraSellComm+(BUYQTY/2), mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC); 
    
    // close out the rest of the shares
    mc.on_fill(-BUYQTY/2, newPrice, CommissionStyle::IBFixed);
    extraSellComm += 0.0000130*BUYQTY/2*newPrice;
    extraSellComm += 0.000119 * BUYQTY/2;

    // check everything for the final time
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(-3*COMM - extraSellComm + BUYQTY, mc.get_rpnl(), PREC); // blur the precision because shorting has higher commissions than buying
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);
    
}

TEST(unsuccessful_buy){

    // initialize
    pnl_calc mc;
   
    // check that you'r flat before you buy anything
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(0.0, mc.get_rpnl(), PREC);
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);

    // step 1: buy 100 shares at $50
    mc.on_fill(BUYQTY, BUYPRICE, CommissionStyle::IBFixed);    

    // check all four
    CHECK_CLOSE(BUYQTY*BUYPRICE, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(BUYQTY, mc.get_qty()); 
    CHECK_CLOSE(-COMM, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC);

    // let the price move against you a dollar
    double newPrice = BUYPRICE - PRICEINC;
    mc.on_price(newPrice);

    // check market value, qty, and realized pnl
    CHECK_CLOSE(newPrice*BUYQTY, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(BUYQTY, mc.get_qty());
    CHECK_CLOSE(-COMM, mc.get_rpnl(), PREC);

    // close half the position
    mc.on_fill(-BUYQTY/2, newPrice, CommissionStyle::IBFixed);
    double extraSellComm = 0.0000130*BUYQTY/2*newPrice;
    extraSellComm += 0.000119 * BUYQTY/2;

    // check everything again
    CHECK_CLOSE(BUYQTY/2*newPrice, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(BUYQTY/2, mc.get_qty());
    CHECK_CLOSE(-2*COMM - extraSellComm - (BUYQTY/2), mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC); 
    
    // close out the rest of the shares
    mc.on_fill(-BUYQTY/2, newPrice, CommissionStyle::IBFixed);
    extraSellComm += 0.0000130*BUYQTY/2*newPrice;
    extraSellComm += 0.000119 * BUYQTY/2;

    // check everything for the final time
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(-3*COMM - extraSellComm - BUYQTY, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);
    
}

TEST(successful_short){

    // initialize
    pnl_calc mc;
   
    // check that you'r flat before you buy anything
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(0.0, mc.get_rpnl(), PREC);
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);

    // step 1: sell 100 shares at $50
    mc.on_fill(-BUYQTY, BUYPRICE, CommissionStyle::IBFixed);    
    double extraSellComm = 0.0000130*BUYQTY*BUYPRICE;
    extraSellComm += 0.000119 * BUYQTY;

    // check all four
    CHECK_CLOSE(-BUYQTY*BUYPRICE, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(-BUYQTY, mc.get_qty()); 
    CHECK_CLOSE(-COMM-extraSellComm, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC);

    // let the price move with you a dollar
    double newPrice = BUYPRICE - PRICEINC;
    mc.on_price(newPrice);

    // check market value, qty, and realized pnl
    CHECK_CLOSE(-newPrice*BUYQTY, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(-BUYQTY, mc.get_qty());
    CHECK_CLOSE(-COMM - extraSellComm, mc.get_rpnl(), PREC);

    // close half the position
    mc.on_fill(BUYQTY/2, newPrice, CommissionStyle::IBFixed);

    // check everything again
    CHECK_CLOSE(-BUYQTY/2*newPrice, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(-BUYQTY/2, mc.get_qty());
    CHECK_CLOSE(-2*COMM-extraSellComm + BUYQTY/2, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC); 
    
    // close out the rest of the shares
    mc.on_fill(BUYQTY/2, newPrice, CommissionStyle::IBFixed);

    // check everything for the final time
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(-3*COMM - extraSellComm + BUYQTY, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);
}

TEST(unsuccessful_short){

    // initialize
    pnl_calc mc;
   
    // check that you'r flat before you buy anything
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(0.0, mc.get_rpnl(), PREC);
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);

    // step 1: sell 100 shares at $50
    mc.on_fill(-BUYQTY, BUYPRICE, CommissionStyle::IBFixed);    
    double extraSellComm = 0.0000130*BUYQTY*BUYPRICE;
    extraSellComm += 0.000119 * BUYQTY;

    // check all four
    CHECK_CLOSE(-BUYQTY*BUYPRICE, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(-BUYQTY, mc.get_qty()); 
    CHECK_CLOSE(-COMM-extraSellComm, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC);

    // let the price move against you a dollar
    double newPrice = BUYPRICE + PRICEINC;
    mc.on_price(newPrice);

    // check market value, qty, and realized pnl
    CHECK_CLOSE(-newPrice*BUYQTY, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(-BUYQTY, mc.get_qty());
    CHECK_CLOSE(-COMM - extraSellComm, mc.get_rpnl(), PREC);

    // close half the position
    mc.on_fill(BUYQTY/2, newPrice, CommissionStyle::IBFixed);

    // check everything again
    CHECK_CLOSE(-BUYQTY/2*newPrice, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(-BUYQTY/2, mc.get_qty());
    CHECK_CLOSE(-2*COMM-extraSellComm - BUYQTY/2, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(BUYPRICE, mc.get_avg_price(), PREC); 
    
    // close out the rest of the shares
    mc.on_fill(BUYQTY/2, newPrice, CommissionStyle::IBFixed);

    // check everything for the final time
    CHECK_CLOSE(0.0, mc.get_mkt_val(), PREC);
    CHECK_EQUAL(0, mc.get_qty());
    CHECK_CLOSE(-3*COMM - extraSellComm - BUYQTY, mc.get_rpnl(), PREC); 
    CHECK_CLOSE(0.0, mc.get_avg_price(), PREC);
}

TEST(specific_situation){

    pnl_calc pnc;

    // 22 shares, 
    pnc.on_fill(22, 44.15, CommissionStyle::ZERO);
    CHECK_CLOSE(22*44.15, pnc.get_mkt_val(), PREC);
    CHECK_CLOSE(22, pnc.get_qty(), PREC);
    CHECK_CLOSE(44.15, pnc.get_avg_price(), PREC); 

}
