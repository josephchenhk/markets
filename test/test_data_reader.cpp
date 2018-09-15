#include <UnitTest++/UnitTest++.h>
#include <string>
#include <vector>

#include "data_readers.h"
#include "market_snapshot.h"

#define PREC .0001

TEST(test_csv_reader){
    
    // initialize csv reader and spit out the raw data
    unsigned int start_row = 0;
    CSVReader csvr("test_data.csv", ",", start_row); // first row will be header names
    std::vector<std::vector<std::string>> d = csvr.getData();
    CHECK_CLOSE(10.34, std::stod(d[1][1]), PREC);
}


TEST(test_market_snapshot_maker_from_csv){

    // construct our ms maker 
    std::vector<std::string> tickers;
    tickers.push_back(std::string("ABC"));
    tickers.push_back(std::string("XYZ"));
    std::vector<std::string> paths;
    paths.push_back(std::string("test_data.csv"));
    paths.push_back(std::string("test_data.csv"));
    MarketSnapshotsMaker msmaker(paths, ",", tickers);
    std::vector<MarketSnapshot> data = msmaker.data();

    // first row:
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

}