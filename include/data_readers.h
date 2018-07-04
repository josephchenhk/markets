#ifndef DATA_READERS_H
#define DATA_READERS_H

#include <string>
#include <vector>

#include "market_snapshot.h"


/**
 * @class CSVReader
 * @author t
 * @date 04/07/18
 * @file csv_reader.h
 * @brief reads csv files, tokenizes column-row elements into strings
 */
class CSVReader
{
private:

	std::string m_fileName;
    char m_delimeter;
    unsigned int m_ignore_rows;
 
public:

	CSVReader(std::string filename, std::string delm, unsigned int meaningless_rows = 0);
 
	std::vector<std::vector<std::string> > getData();
};



/**
 * @class MarketSnapshotsMakerFromCsv
 * @author t
 * @date 04/07/18
 * @file data_readers.h
 * @brief here is an example:
 *     
    std::vector<std::string> files;
    files.push_back("/home/t/s/data/XLB.csv");
    files.push_back("/home/t/s/data/XLE.csv");
    std::vector<std::string> tickers;
    tickers.push_back("XLB");
    tickers.push_back("XLE");
    MarketSnapshotsMakerFromCsv dr(files, ",", tickers);
    
    std::vector<MarketSnapshot> data = dr.data();
    MarketSnapshot first_datum = data[0];
    std::map<Instrument,MarketBar> bars = first_datum.bars();
    std::cout << bars[Instrument("XLB")].close() << "\n";

 */
class MarketSnapshotsMakerFromCsv
{
private:
    std::vector<std::string> m_tickers;
    std::vector<MarketSnapshot> m_data;

public:
    MarketSnapshotsMakerFromCsv(std::vector<std::string> filenames, std::string delimeter, std::vector<std::string> tickers);
    std::vector<MarketSnapshot> data() const;
};



#endif // DATA_READERS_H