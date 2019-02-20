# markets

Description
----------

`markets` is a small event-driven program that backtests trading strategies. It will show the outcome of any strategy can change based on 

- order types
- market impact/slippage
- initial wealth

getting started
----------------

The example trading strategy tries to maintain equal weights in each of ten instruments. To get started, do the following steps.

1. to get the library and build it, type the following into a terminal: 

```
git clone https://github.com/tbrown122387/markets.git
cd markets
mkdir bin
sh build_lib.sh
```

2. after that, take a look at the example by running

```
cd example
make
./run_backtest
```

this will show you what the command line arguments need to be. If you want to run a specific backtest with the example trading strategy using our example data, replace the last line with the following:

```
./run_backtest 100000 weekly_etfs/ .05 20 market 0 0
```

If you want to save those results in a text file and analyze them with your favorite statistical software, type the following:

```
./run_backtest 100000 weekly_etfs/ .05 20 market 0 0 > ~/Desktop/wealth_snapshots.csv
```

The commissions are handled automatically inside the program. You can also simulate slippage by adjusting the third parameter.


tests
-----

We also have unit tests to make sure everything works. Go into the `test` directory and type the following

```
make clean 
make
./run_tests
```

