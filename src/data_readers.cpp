#include "data_readers.h"

#include <fstream>      // std::ifstream
#include <sstream>      // std::istringstream


CSVReader::CSVReader(std::string filename, std::string delm, unsigned int meaningless_rows) 
    : m_fileName(filename), m_delimeter(delm[0]), m_ignore_rows(meaningless_rows)
{ 
    if(delm.length() > 1){
        throw std::invalid_argument("the delimeter argument must be of length 1\n");
    }
}


std::vector<std::vector<std::string> > CSVReader::getData()
{
    std::vector<std::vector<std::string> > allData;
    std::ifstream file(m_fileName);

    std::string line = "";
    unsigned int c(1); // this is not the row number of the data
    while (std::getline(file, line))
    {
        if(c > m_ignore_rows){
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(line);
            while(std::getline(tokenStream, token, m_delimeter))
            {
                tokens.push_back(token);
            }
            allData.push_back(tokens);
        }
        c++;
    }
	
    file.close();
    return allData;
}

// TODO: make sure this doesn't get choked up when two of the things have the same data
MarketSnapshotsMaker::MarketSnapshotsMaker(
                                    std::vector<std::string> filenames, 
                                    std::string delimiter, 
                                    std::vector<std::string> tickers) 
    : m_tickers(tickers)
{
    // NB: assumes all files have the same number of rows
    // NB: you kind of have to iterate in a weird way because you need to go a row at a time for each data file for each market snapshot! 
    
    // the assumed format for each file is: 
    // period,bid_price_open,bid_price_high,bid_price_low,bid_price_close,ask_price_open,ask_price_high,ask_price_low,ask_price_close,volume
    // 2000-01-03 00:00:00,17.48,17.49,17.00,17.02,17.48,17.49,17.00,17.02,140500

    unsigned int num_tickers, num_rows, csv_reader_start_row; 
    num_tickers = m_tickers.size();
    csv_reader_start_row = 1; // skip the column names
    std::vector<std::vector<std::vector<std::string>>> all_raw_data;

    // store all raw data so we can iterate over it in a weird way
    for(size_t ticker = 0; ticker < num_tickers; ++ticker){
        
        // raw string content for each ticker
        CSVReader csvr(filenames[ticker], delimiter, csv_reader_start_row);
        all_raw_data.push_back(csvr.getData());

        if(ticker == 0){
            num_rows = all_raw_data[0].size();
        }

    }

    // iterate over each time point/row of data
    for(size_t time = 0; time < num_rows; ++time){

        // go snapshot by snapshot...
        double open, high, low, close;
        unsigned int vol;
        std::map<Instrument,MarketBar> temp_map;
        for(size_t ticker = 0; ticker < num_tickers; ++ticker){

            // make one ticker's bar
            auto tstamp =            all_raw_data[ticker][time][0];
            open        = std::stod( all_raw_data[ticker][time][1]);
            high        = std::stod( all_raw_data[ticker][time][2]);
            low         = std::stod( all_raw_data[ticker][time][3]);
            close       = std::stod( all_raw_data[ticker][time][4]);
            vol         = std::stoul(all_raw_data[ticker][time][9]);
    
            MarketBar bar(open, high, low, close, vol, tstamp);
            Instrument instr(m_tickers[ticker]);
            temp_map.insert(std::pair<Instrument,MarketBar>(instr, bar));
        }

        // add the complete snapshot (made from the now-complete map)
        MarketSnapshot ms(temp_map);
        m_data.push_back(ms);
    }
}


std::vector<MarketSnapshot> MarketSnapshotsMaker::data() const
{
    return m_data;
}
