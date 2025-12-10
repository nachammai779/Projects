NBA_myvector<-NBA[["interest_over_time"]][["hits"]]   

NBA_myvector   

index_to_find=12*(2004-2004)+1   

index_to_find   

jan2004=NBA_myvector[(1:187)>=index_to_find]   

jan2004   

min_index=which.min(jan2004)   

min_index  

years=min_index%/%12   

years  

months=min_index%%12   

months  

year_vector=2004:2019   

month_vector=c("jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec")   

paste("year is ",year_vector[2004-2004+1+years])   

paste("month is ", month_vector[months]) 