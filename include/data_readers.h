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
 * @brief reads csv files, 
 * tokenizes column-row elements into strings, 
 * but does not parse anything
 */
class CSVReader
{
private:

    std::string m_fileName;
    char m_delimeter;
    unsigned int m_ignore_rows;
 
public:

    /**
     * @brief constructor
     * @param filename the path of the file you want to read in
     * @param delm the delimiter
     * @param meaningless_rows how many rows you want to skip
     */ 
    CSVReader(std::string filename, std::string delm, unsigned int meaningless_rows = 0);
 

    /**
     * @brief returns the data in a useable format
     * @return a block of string data
     */
    std::vector<std::vector<std::string> > getData();
};



/**
 * @class MarketSnapshotsMaker
 * @author t
 * @date 04/07/18
 * @file data_readers.h
 * @brief reads in data (assumed in a certain format), 
 * and parses it into a vector of MarketSnapshots
 */
class MarketSnapshotsMaker
{
private:
    std::vector<std::string> m_tickers;
    std::vector<MarketSnapshot> m_data;

public:

    /**
     * @brief ctor
     * @param filenames the paths to the files you want to read in
     * @param delimiter the column delimiter
     * @param tickers the names of the tickers in the same order as the filenames
     */
    MarketSnapshotsMaker(std::vector<std::string> filenames, std::string delimiter, std::vector<std::string> tickers);


    /**
     * @brief returns data in a useable format
     */
    std::vector<MarketSnapshot> data() const;

};



#endif // DATA_READERS_H
