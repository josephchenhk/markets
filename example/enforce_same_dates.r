library(xts)

#setwd("~/markets/example/")
filenames <- list.files(path = "./daily_etfs/", pattern='.csv$')
csvfilepaths <- paste(getwd(), "/daily_etfs/", filenames, sep = "")
myDataFrames <- lapply(X = csvfilepaths, FUN = read.csv, header = T, stringsAsFactors=F)
column_names <- colnames(myDataFrames[[1]])
myIndexCols <- lapply(X = myDataFrames, FUN = function(df) df[,1])
commonDates <- as.Date(Reduce(intersect, myIndexCols))
myXtsDFs <- lapply(myDataFrames, function(df) {as.xts(df, order.by = as.Date(df[,1]))})
myXtsDFs <- lapply(myXtsDFs, function(xtsdf) xtsdf[commonDates])

for(i in 1:length(myXtsDFs)){
  path <- paste(getwd(), "/daily_etfs/clean_data/", filenames[i], sep = "")
  print(path)
  write.csv(myXtsDFs[[i]], file = path, quote = F, row.names = F)
}
