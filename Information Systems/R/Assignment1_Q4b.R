MLB_myvector<-MLB[["interest_over_time"]][["hits"]]
NBA_myvector<-NBA[["interest_over_time"]][["hits"]]
NFL_myvector<-NFL[["interest_over_time"]][["hits"]]
sum(NFL_myvector>MLB_myvector)