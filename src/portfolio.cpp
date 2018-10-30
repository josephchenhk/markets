#include "portfolio.h"


Portfolio::Portfolio(double starting_cash, std::vector<std::string> tickers, CommissionStyle cs)
    : m_pos_summary(starting_cash, tickers, cs)
    , m_todo_step(1)
{   
     for(size_t i = 0; i < tickers.size(); ++i)
        m_ordered_tickers.push_back(Instrument(tickers[i]));
}


void Portfolio::readAllFills(std::queue<Fill>& q)
{   
    if(m_todo_step != 1)
        throw std::runtime_error("Portfolio methods are being called out of order");
    
    while(!q.empty()){
        m_pos_summary.onFill(q.front());
        std::cout << "reading a fill for " << q.front().instrument.symbol << " at price " << q.front().execute_price << " for " << q.front().quantity << "shares\n";
        q.pop();
    }
    
    // allow the next method to execute
    m_todo_step = 2;
}


void Portfolio::readNewPrices(MarketSnapshot ms)
{   
    if(m_todo_step != 2)
        throw std::runtime_error("Portfolio methods are being called out of order");
    
    m_last_snapshot = ms;
    m_pos_summary.onSnapshot(ms);
    
    // allow the next method to execute
    m_todo_step = 3;
}


Eigen::VectorXd Portfolio::getWeights1(const Eigen::MatrixXd& Sigma, const Eigen::VectorXd& mu, const double& riskTolerance)
{
    // this uses quadProg1 
    // it ignores transaction costs
    // Q = 2 Sigma
    // c = - riskTol * mu
    // A = (1,1,1,1,1,1)
    // b = 1 (scalar)
    if(riskTolerance < 0.0) throw std::invalid_argument("riskTolerance must be positive\n");
    Eigen::MatrixXd A = Eigen::MatrixXd::Constant(1, mu.rows(), 1.0);
    Eigen::MatrixXd b = Eigen::MatrixXd::Constant(1,1,1.0);
    return quadProg1(2*Sigma, -riskTolerance*mu, A, b);
}


Eigen::VectorXd Portfolio::quadProg1(const Eigen::MatrixXd& Q, const Eigen::VectorXd& c, const Eigen::MatrixXd&A, const Eigen::VectorXd& b)
{
    //finds min_x .5 x'Qx + x'c 
    // such that A x = b
    // also: we assume A is full (row) rank and 
    // Q is positive definite. This will make KKT matrix invertible.
    Eigen::MatrixXd Qinv = Q.inverse();
    Eigen::VectorXd lambda = - (A * Qinv * A.transpose()).inverse() * (A * Qinv * c + b );
    return - Qinv * A.transpose() * lambda - Qinv * c;
}


void Portfolio::updateOnNewIdealWts(const Eigen::VectorXd& ideal_wts_to_be, ExecHandler& order_q)
{
    // generate and submit orders based on these new ideal weights.
    // to calculate the order_qty, keep in mind that:
    // 
    //      signed_order_qty * this_order_price + current_position_mkt_value <= ideal_wt * total_balance
    //
    // where current_position_mkt_value = current_position_signed_qty * current_ave_price and
    // total balance is the starting cash plus all realized pnls from all positions

    // NB1: the fixed order_price might be "outdated" if it's based on the previously most recent price
    // NB2: this function assumes that it is only called after the previous UNFILLED order is expired. 
    // It does not check for unfilled orders...just previously filled orders (via the position). 
    // If this function were called too quickly, it would potentially submit multiple orders on 
    // top of other orders for the same symbols. 

    if(m_todo_step != 3)
        throw std::runtime_error("Portfolio methods are being called out of order");

    int signed_qty;
    unsigned int pos_qty;
    for(size_t i = 0; i < m_ordered_tickers.size(); ++i){

        Instrument instr(m_ordered_tickers[i]);
        double this_orders_price = m_last_snapshot[instr].close();
        double current_position_value = m_pos_summary.getInstrumentMktVal(instr);
        signed_qty = std::trunc((m_pos_summary.getBalance()*ideal_wts_to_be[i] - current_position_value)/this_orders_price);

        if(signed_qty < 0){ 
            pos_qty = -signed_qty;
            order_q.addOrder(Order(instr, OrderType::limitSell, this_orders_price, pos_qty));
            std::cout << "submitted an order for symbol " << m_ordered_tickers[i].symbol << ": " << signed_qty << " shares at price " << this_orders_price << "    \n";

        }else if(signed_qty > 0){ 
            pos_qty = signed_qty;
            order_q.addOrder(Order(instr, OrderType::limitBuy, this_orders_price, pos_qty));
            std::cout << "submitted an order for symbol " << m_ordered_tickers[i].symbol << ": " << signed_qty << " shares at price " << this_orders_price << "    \n";

        }else{ // signed_qty == 0
            std::cout << "sending no order for symbol " << m_ordered_tickers[i].symbol << " at price " << this_orders_price << "\n";
        }
    }

    // allow next method to execute
    m_todo_step = 1;
}


double Portfolio::getBalance() const
{
    return m_pos_summary.getBalance();
}


double Portfolio::getMktVal(const std::string& sym) const
{
    return m_pos_summary.getInstrumentMktVal(sym);
}



