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
    std::queue<Fill>  fills_q;
    std::queue<Order> orders_q;
    for(size_t i = 0; i < data.size(); ++i){
        
        // data handler just to help converting market snapshots to Eigen::Vectors os math stuff can be done outside Portfolio object
        dh.onData(data[i]);

        // portfolio 1. fills
        my_port.readAllFills(fills_q);
        
        // portfolio 2. prices
        my_port.readNewPrices(data[i]);
        
        // portfolio 3. new ideal weights
        if(i % 3 == 0){
            my_port.updateOnNewIdealWts(Eigen::Matrix<double,2,1>::Zero(), orders_q);
        }else if(i % 3 == 1){
            my_port.updateOnNewIdealWts(Eigen::Matrix<double,2,1>::Constant(.5), orders_q);
        }else{
            Eigen::Matrix<double,2,1> wts = Eigen::Matrix<double,2,1>::Zero();
            wts(0) = 1.0;
            my_port.updateOnNewIdealWts(wts, orders_q);
        }
    
    }


    
    
    
    
	return 0;
}
