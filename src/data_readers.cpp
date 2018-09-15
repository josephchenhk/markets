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
    unsigned int c(1);
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




MarketSnapshotsMakerFromCsv::MarketSnapshotsMakerFromCsv(
                                    std::vector<std::string> filenames, 
                                    std::string delimeter, 
                                    std::vector<std::string> tickers
                                                        ) : m_tickers(tickers)
{
    // we assume the filenames and the tickers are given in the same order!
    
    // make a bunch of raw file content arrays
    std::vector<std::vector<std::vector<std::string>>> datasets;
    datasets.resize(filenames.size());
    for(unsigned int i = 0; i < filenames.size(); ++i){
        CSVReader csvr(filenames[i], ",", 0);
        datasets[i] = csvr.getData();
    }
    
    // iterate through eaach row of data
    for(unsigned int i = 1; i < datasets[0].size(); ++i){
        
        // the assumed format for each file is: 
        // period,bid_price_open,bid_price_high,bid_price_low,bid_price_close,ask_price_open,ask_price_high,ask_price_low,ask_price_close,volume
        // 2000-01-03 00:00:00,17.48,17.49,17.00,17.02,17.48,17.49,17.00,17.02,140500
        std::map<Instrument, MarketBar> le_map;
        for(unsigned int j = 0; j < datasets.size(); ++j){
            
            auto time = datasets[j][i][0];
            double open = std::stod(datasets[j][i][1]);
            double high = std::stod(datasets[j][i][2]);
            double low = std::stod(datasets[j][i][3]);
            double close = std::stod(datasets[j][i][4]);
            unsigned int vol = std::stoul(datasets[j][i][9]);
            MarketBar bar(open, high, low, close, vol, time);
            Instrument instr(m_tickers[j]);
            le_map.insert(std::pair<Instrument,MarketBar>(instr, bar));
        }
        MarketSnapshot ms(le_map); 
        m_data.push_back(ms);        
    } 
}


std::vector<MarketSnapshot> MarketSnapshotsMakerFromCsv::data() const
{
    return m_data;
}
