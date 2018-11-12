#include <UnitTest++/UnitTest++.h>
#include <string>
#include <vector>
#include <algorithm> // std::find

#include "data_readers.h"
#include "market_snapshot.h"

#define PREC .0001
#include <iostream> // tmp
TEST(test_csv_reader){
    
    // initialize csv reader and spit out the raw data
    unsigned int start_row = 0;
    CSVReader csvr("test_data/XLF.csv", ",", start_row); // first row will be header names
    std::vector<std::vector<std::string>> d = csvr.getData();
    CHECK_CLOSE(10.34, std::stod(d[1][1]), PREC); // second row second col (first col is time)
}


TEST(test_market_snapshot_maker_from_csv){

    // constructor 1.
    // construct our ms maker 
    std::vector<std::string> tickers;
    tickers.push_back(std::string("ABC"));
    tickers.push_back(std::string("XYZ"));
    std::vector<std::string> paths;
    paths.push_back(std::string("test_data/XLF.csv"));
    paths.push_back(std::string("test_data/XLF.csv"));
    MarketSnapshotsMaker msmaker(paths, ",", tickers);
    std::vector<MarketSnapshot> data = msmaker.data();

	// check the first row
    // it looks like this in the text file:
    // 2000-01-03 00:00:00,10.34,10.34,9.90,9.97,10.34,10.34,9.90,9.97,1185600
    MarketSnapshot first_snapshot = data[0].bars();
    MarketBar bar1 = first_snapshot["ABC"];
    MarketBar bar2 = first_snapshot["XYZ"];
    CHECK_CLOSE(10.34, bar1.open(),  PREC);
    CHECK_CLOSE(10.34, bar1.high(),  PREC);
    CHECK_CLOSE(9.90,  bar1.low(),   PREC);
    CHECK_CLOSE(9.97,  bar1.close(), PREC);
    CHECK_CLOSE(10.34, bar2.open(),  PREC);
    CHECK_CLOSE(10.34, bar2.high(),  PREC);
    CHECK_CLOSE(9.90,  bar2.low(),   PREC);
    CHECK_CLOSE(9.97,  bar2.close(), PREC);

    // constructor 2. 
    MarketSnapshotsMaker msm2("test_data", ",");
    std::vector<std::string> tickers2 = msm2.ordered_tickers();
    bool xleInTickers = false; 
    if( std::find(tickers2.begin(), tickers2.end(), "XLE") != tickers2.end()){
        xleInTickers = true;   
    }    
    CHECK(xleInTickers);

    // check the first row
    std::vector<MarketSnapshot> data2 = msm2.data();
    MarketSnapshot second_snapshot = data2[0].bars();
    MarketBar bar3 = second_snapshot["XLF"];


}
