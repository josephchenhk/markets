#library(PerformanceAnalytics)

args <- commandArgs(TRUE)
wealth <- read.csv(args[1], sep=",", header=F)
logrets <- diff(log(wealth[,1]))
sharpe <- mean(logrets)/sd(logrets)
cat("sharpe: ", sharpe, "\n" )

pdf("wealthcurve.pdf")
plot.ts(wealth)
dev.off()
