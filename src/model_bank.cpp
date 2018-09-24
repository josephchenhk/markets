#include "model_bank.h"

#include <algorithm> // std::rotate
#include <stdexcept> // std::invalid_argument
#include <memory> // unique_ptr
#include <numeric> // accumulate

MoveAve::MoveAve(const unsigned int& width) : m_width(width), m_num_data_seen(0)
{
    if(width <= 0) throw std::invalid_argument("width must be a positive integer");
    
    m_data.resize(width);
}


void MoveAve::onNewData(const double& new_data)
{
    if(m_num_data_seen >= m_width){ // data window full
        std::rotate(m_data.begin(), m_data.begin()+1, m_data.end());
        m_data[m_width-1] = new_data;
    }else{ // have not seen a full window of points yet
        m_data[m_num_data_seen] = new_data;
    }
    
    m_num_data_seen++;
}


double MoveAve::getCurrentAve() const
{   
    if(m_num_data_seen<m_width) throw std::length_error("trying to obtain the average of too few numbers");
    return std::accumulate(m_data.begin(), m_data.end(), 0.0)/static_cast<double>(m_width);
}


double MoveAve::getCurrentSum() const
{
    if(m_num_data_seen < m_width) throw std::length_error("trying to obtain the sum of too few numbers");
    return std::accumulate(m_data.begin(), m_data.end(), 0.0);
}


bool MoveAve::isFull() const
{
    return m_num_data_seen >= m_width;
}


ModelBank::ModelBank(const std::vector<std::string>& ordered_tickers, unsigned int pred_type)
    : m_data_handler(ordered_tickers), m_pred_type(pred_type)
{
}


void ModelBank::process_snapshot(const MarketSnapshot& ms)
{
    // take the snapshot and convert it to a Eigen::VectorXd
    m_data_handler.onData(ms);
    Eigen::VectorXd log_returns = m_data_handler.logReturns();

    // update each model and each model's log-likelihood
    unsigned int num_mods = m_all_mods.size();
    for(size_t i = 0; i < num_mods; ++i){
        m_all_mods[i]->on_data(log_returns);
        m_time_ave_log_likes[i].onNewData(m_all_mods[i]->getLL());
    }

    // if you have enough data, change the forecasts
    if(m_time_ave_log_likes[0].isFull()){

        // get all log_likes aka evidences and their maximum
        std::vector<double> current_log_likes(num_mods);
        double max_log_like(-1.0/0.0);
        for(size_t j = 0; j < num_mods; ++j){
            current_log_likes[j] = m_time_ave_log_likes[j].getCurrentSum();
            if(current_log_likes[j] > max_log_like)
                max_log_like = current_log_likes[j];
        }

        // use the maxium and the log-likes to compute the model posterior probabilities
        // p(k | y_{1:n}) propto p(y_{1:n} | k)p(k) \propto p(y_{1:n} | k)
        double norm_const(0.0);
        for(size_t k = 0; k < num_mods; ++k){
            m_current_model_posteriors[k] = std::exp(current_log_likes[k] - max_log_like);
            norm_const += m_current_model_posteriors[k];
        }
    
        // normalize the thing by dividing by the normalizing constant
        std::for_each(m_current_model_posteriors.begin(), 
                      m_current_model_posteriors.end(), 
                      [&norm_const](double &el){el /= norm_const; } );


        // make a forecast
        if(m_pred_type == 1){
                    
            // option 1: predict with the "best" mod
            std::vector<double>::iterator res = std::max_element(m_current_model_posteriors.begin(), m_current_model_posteriors.end());
            auto bestMod = std::distance(m_current_model_posteriors.begin(), res);
            
            m_mu = m_all_mods[bestMod]->meanVec();
            m_cov = m_all_mods[bestMod]->covMat();
        
        }else if(m_pred_type == 2){

            // option 2: predict with the average mod
            Eigen::VectorXd tempMu = Eigen::VectorXd::Zero(num_mods);
            Eigen::MatrixXd secondMoments = Eigen::MatrixXd::Zero(num_mods, num_mods);

            for(size_t l = 0; l < num_mods; ++l){
                tempMu        += m_all_mods[l]->meanVec() * m_current_model_posteriors[l];
                secondMoments += (m_all_mods[l]->covMat() + m_all_mods[l]->meanVec() * m_all_mods[l]->meanVec().transpose()) * m_current_model_posteriors[l];
            }

            m_mu = tempMu;
            m_cov = secondMoments - (tempMu * tempMu.transpose()); 

        }else{
            throw std::runtime_error("unexpected prediction type\n");
        }

        // update your idael weights
        // TODO

    }


}


