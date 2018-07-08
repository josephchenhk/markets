#include <iostream>

#include <map>

#include "pnl_calculator.h"
#include "data_readers.h"
#include "data_handlers.h"
#include "portfolio.h"


int main(int argc, char **argv)
{

    std::vector<std::string> files;
    files.push_back("/home/t/s/data/XLB.csv");
    files.push_back("/home/t/s/data/XLE.csv");
    std::vector<std::string> tickers;
    tickers.push_back("XLB");
    tickers.push_back("XLE");
    MarketSnapshotsMakerFromCsv dr(files, ",", tickers);
    std::vector<MarketSnapshot> data = dr.data();

    // iterate through the data with a handler1 and portfolio object
    DataHandler dh(tickers);
    Portfolio<2> my_port(10000.0, tickers);
    std::queue<Fill> fills_q;
    ExecHandler orders_q;
    for(size_t i = 0; i < data.size(); ++i){
        
        std::cout << "current time: " << i+1 << "\n";
        
        
        // data handler just to help converting market snapshots to Eigen::Vectors 
        // so math stuff can be done outside Portfolio object
        dh.onData(data[i]);
        
        
        // Broker 1. check for orders that were submitted at (the end of ) the last time period
        // this code should be hidden from the end user (you)
        orders_q.process_orders_yield_fills(data[i], fills_q);

        // portfolio 1. fills
        my_port.readAllFills(fills_q);
        
        // portfolio 2. prices
        my_port.readNewPrices(data[i]);
        
        /// Math stuff: here is all the hard work where you will compute predictions, 
        // and then obtain the weights
        Eigen::Matrix<double,2,1> ideal_wts;
        if(i%3 == 0){
            ideal_wts = Eigen::Matrix<double,2,1>::Zero();
        }else if(i % 3 == 1){
            ideal_wts = Eigen::Matrix<double,2,1>::Constant(.5);            
        }else{
            ideal_wts = Eigen::Matrix<double,2,1>::Zero();
            ideal_wts(0) = 1.0;
        }
        
        // portfolio 3. submit orders based on these new ideal weights
        my_port.updateOnNewIdealWts(ideal_wts, orders_q);
        
    }


    
    
    
    
	return 0;
}
