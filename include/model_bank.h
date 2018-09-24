#ifndef MODEL_BANK_H
#define MODEL_BANK_H

#include <vector>
#include <string>
#include <memory>

#include "market_snapshot.h"
#include "data_handlers.h"

/**
 * @class MoveAve
 * @author taylor
 * @brief a simple moving average class
 * used to store the most recent model log-likelihoods
 */
class MoveAve{

public:

    /* default constructor disallowed */
    MoveAve() = delete;


    /**
     * @brief ctor
     * @param width how many time points do you want to average over
     */
    MoveAve(const unsigned int& width);


    /**
     * @brief takes in a new data point
     * @param new_data the newest datum
     */
    void onNewData(const double& new_data);


    /** 
     * @brief get the current average value
     * @return the current moving average
     */
    double getCurrentAve() const;


    /**
     * @brief get the current sum of all your data
     * @return the sum
     */
    double getCurrentSum() const;

    bool isFull() const;
private:
    std::vector<double> m_data;
    unsigned int m_width;
    unsigned int m_num_data_seen;
};


/**
 * @class base_mod
 * @author taylor
 * @brief each model must inherit from this abstract base class
 */
class base_mod{

public:
    
    /**
     * @brief processes a new data point in the form of an Eigen::VectorXd
     */
    virtual void on_data(const Eigen::VectorXd& yt) = 0;


    /**
     * @brief returns a point forecast for the one step ahead log-returns
     */
    virtual Eigen::VectorXd meanVec() const = 0;


    /**
     * @brief returns a covariance matrix estimate for the one-step ahead log-returns
     */
    virtual Eigen::MatrixXd covMat() const = 0;


    /**
     * @brief returns the up-to-date log-likelihood
     */
    virtual double getLL() const = 0;
};


/**
 * @brief a container of all your models
 * @author taylor
 * @brief stores all of your models
 * user needs to subclass this and add models to the container of unique pointers
 * example:
 *     m_all_mods.push_back(std::unique_ptr<mod4>(new mod4(param_estimates)));
 * for the bayesian model averaging stuff, assumes uniform priors
 */
class ModelBank{
public:

    /**
     * @brief ctor
     * @param the tickers you're looking at in order
     * @param pred_type: 1 corresponds with the model that has the best posterior, 2 corresponds with an average
     */
    ModelBank(const std::vector<std::string>& ordered_tickers, unsigned int pred_type = 1);

    
    void process_snapshot(const MarketSnapshot& ms);

private:

    // convenience object that converts market snapshots to Eigen::VectorXds
    DataHandler m_data_handler;

    // p(y_n | y_{1:n-1}, k) (not dependend on theta) (aka evidences)
    std::vector<MoveAve> m_time_ave_log_likes;
    
    // p(mod_i | y_{1:n}) not on log-scale
    std::vector<double> m_current_model_posteriors;

    // type of prediction you want to use
    unsigned int m_pred_type;

    // one-step-ahead forecast mean vector
    Eigen::VectorXd m_mu;

    // one-step-ahead forecast covariance matrix
    Eigen::MatrixXd m_cov;

protected:

    // this will be filled in the derived class' constructor
    std::vector<std::unique_ptr<base_mod>> m_all_mods;
};

#endif //MODEL_BANK_H
