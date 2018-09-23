#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>
#include <iostream>

#include "execution_handler.h"

#define SLIP .01
#define PREC .0001


TEST(test_exec_handler){

    // init a handler
    ExecHandler e(SLIP);

    // make a market snapshot
    MarketBar::Time t1 = std::chrono::system_clock::now();
    Instrument instr1("ABC");
    Instrument instr2("XYZ");
    MarketBar bar1(100.0, 101.00, 99.00, 100.0, 1000, t1);
    MarketBar bar2(120.0, 121.00, 119.00, 120.00, 1000, t1);
    std::map<Instrument,MarketBar> test_bars;
    test_bars.insert(std::pair<Instrument,MarketBar>(instr1, bar1));
    test_bars.insert(std::pair<Instrument,MarketBar>(instr2, bar2));
    MarketSnapshot ms(test_bars);

    // make a successful limit buy and add
    Order o1(instr1, OrderType::limitBuy,100.00,100);
    e.addOrder(o1);

    // make a successful limit sell and add
    Order o2(instr1, OrderType::limitSell, 100.00,100);
    e.addOrder(o2);

    // make an unsuccessful limit buy and add
    Order o3(instr1, OrderType::limitBuy, 1.00, 100);
    e.addOrder(o3);

    // make an unsucessful limit sell and add
    Order o4(instr1, OrderType::limitSell, 999.99, 100);
    e.addOrder(o4);

    // make a market buy and add
    Order o5(instr1, OrderType::marketBuy, 100.0, 100);
    e.addOrder(o5);

    // make a market sell and add
    Order o6(instr1, OrderType::marketSell, 100.00, 100);
    e.addOrder(o6);

    // make a queue of fills
    std::queue<Fill> fq;

    // process orders
    e.process_orders_yield_fills(ms, fq);

    // make sure fills are cool
    while(!fq.empty()){

        Fill fill = fq.front();
        
        // should be successful limit buy
        if(fill.order_id == o1.id){
            CHECK(fill.instrument == o1.instrument);
            CHECK(fill.quantity == o1.quantity);
            CHECK(fill.execute_price == fill.order_price);
            CHECK(fill.order_price == o1.price);

        // should be successful limit sell
        }else if(fill.order_id == o2.id){
            CHECK(fill.instrument == o2.instrument);
            CHECK(fill.quantity == o2.quantity);
            CHECK(fill.execute_price == fill.order_price);
            CHECK(fill.order_price == o2.price);

        // should be unsuccessful limit buy
        }else if(fill.order_id == o3.id){
    
            // this should never be triggered because it implies a fill was generated
            CHECK(false);

        // should be unsuccessful limit sell
        }else if(fill.order_id == o4.id){

            // should never be generated
            CHECK(false);

        // should be a market buy
        }else if(fill.order_id == o5.id){

            CHECK(fill.instrument == o5.instrument);
            CHECK(fill.quantity == o5.quantity);
            CHECK_CLOSE(fill.execute_price - fill.order_price, SLIP, PREC);

        // should be a market sell
        }else if(fill.order_id == o6.id){

            CHECK(fill.instrument == o6.instrument);
            CHECK(fill.quantity == o6.quantity);
            CHECK_CLOSE(fill.execute_price - fill.order_price, -SLIP, PREC);

        }else{
            std::cerr << "unexpected fill id\n";
            CHECK(false);
        }
        
        fq.pop();
    }

}
