#ifndef DATA_READERS_H
#define DATA_READERS_H

#include <string>
#include <vector>
#include <filesystem>

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

private:

    std::string m_fileName;
    char m_delimeter;
    unsigned int m_ignore_rows; 
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

    /* the tickers for which data are being produced */
    std::vector<std::string> m_tickers;

    /* the data itself in snapshot form*/
    std::vector<MarketSnapshot> m_data;

    /* a function that gets tickers from the file paths. Assumes files are of the form e.g. TICKER.csv */
    std::vector<std::string> get_tickers_from_paths(const std::vector<std::string>& paths);

public:


    /**
     * @brief alternative ctor. Will read in every file in a directory and try to get the ticker names automatically
     * @param data_directory where the files are...will try to read in all of the files in their 
     * @param delimiter column delimiter
     */
    MarketSnapshotsMaker(const std::string& data_directory, std::string delimiter);

    /**
     * @brief ctor
     * @param filenames the paths to the files you want to read in
     * @param delimiter the column delimiter
     * @param tickers the names of the tickers in the same order as the filenames
     */
    MarketSnapshotsMaker(std::vector<std::string> filenames, std::string delimiter, std::vector<std::string> tickers);


    /**
     * @brief returns data in a useable format
     * @return a std::vector of MarketSnapshots
     */
    std::vector<MarketSnapshot> data() const;


    /**
     * @brief gives you the tickers currently available
     * @ return the tickers in a std::vector<std::string>
     */
    std::vector<std::string> tickers() const;

};



#endif // DATA_READERS_H
