#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <string>
#include <queue> 

#include "data_readers.h"
#include "portfolio.h"
#include "execution_handler.h"
#include "data_handlers.h"

#define dimobs 10

int main(int argc, char* argv[]){

    ////////////////////////////////////////
    // read in stuff from the command line
    ///////////////////////////////////////
    if(argc != 5){
        std::cerr << "please enter:\n"
                     "1. starting cash\n"
                     "2. data directory (each file corresponds with a ticker, and it includes train data *and* test data) \n"
                     "3. slippage on market orders\n"
                     "4. the first out of sample row (start counting at 0) \n";
        return 1;
    }

    double start_cash = atof(argv[1]);
    std::string data_dir = argv[2];
    double market_order_slippage = atof(argv[3]);
    int first_oos_row = atoi(argv[4]);

    
    //////////////////////
    // perform analysis //
    //////////////////////

    MarketSnapshotsMaker msm(data_dir, ",");
    std::vector<std::string> ordered_tickers = msm.ordered_tickers();
    std::vector<MarketSnapshot> data = msm.data();
    Portfolio port(start_cash, ordered_tickers, CommissionStyle::IBFixed, false);
    ExecHandler exec_handler(market_order_slippage); 
    std::queue<Fill> fills;    
    DataHandler data_handler(ordered_tickers); // TODO: how can we not rely on the ordering of DataHandler's vectors? if they don't coincide all hell breaks loose

    double tmp(start_cash);
    Eigen::VectorXd arithmetic_returns;
    bool out_of_sample = false;
    Eigen::Matrix<double,dimobs,1> weights = Eigen::Matrix<double,dimobs,1>::Constant(1.0/(double)dimobs);
    for(size_t t = 0; t < data.size(); ++t){

        // parse data
        data_handler.onData(data[t]);

        // Broker 1. check for orders that were submitted at (the end of ) the last time period
        exec_handler.process_orders_yield_fills(data[t], fills);

        // portfolio 1: fills
        port.readAllFills(fills);

        // portfolio 2: prices
        port.readNewPrices(data[t]);   // bug here!


        // if you're in the test_data 
//        out_of_sample = t > first_oos_row;
//        if(out_of_sample){
//
//            arithmetic_returns = data_handler.arithReturns();
//            //std::cout << arithmetic_returns.transpose() << "\n";
//
//            // update models
//
//
//        }
//
        port.react_and_send_orders(weights, exec_handler);
        std::cout << port.getBalance() << "\n";
    }


    return 0;
}
