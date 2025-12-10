MLB_myvector<-MLB[["interest_over_time"]][["hits"]]    

MLB_myvector    

index_to_find=12*(2015-2004)+1    

index_to_find    

jan2015=MLB_myvector[(1:187)>=index_to_find]    

jan2015    

min_index=which.min(jan2015)    

min_index   

years=min_index%/%12    

years   

months=min_index%%12    

months   

year_vector=2004:2019    

month_vector=c("jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec")    

paste("year is ",year_vector[2015-2004+1+years])    

paste("month is ", month_vector[months]) 