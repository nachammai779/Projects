MLB_myvector<-MLB[["interest_over_time"]][["hits"]]
NBA_myvector<-NBA[["interest_over_time"]][["hits"]]
NFL_myvector<-NFL[["interest_over_time"]][["hits"]]
index_start=(2004-2004)*12+1
index_start
index_end=(index_start+11)
index_end
mean(NBA_myvector[index_start:index_end])