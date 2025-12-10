dataF=read.csv(file="C:/Users/Nachammai/Desktop/INFSYS6891/Homework/Data_with_column_names.csv")
View(dataF)

for(i in 1:length(dataF$Floor)){
  dataF$Floor[i]=dataF$Floor[i]+1
  print(i)
}

for(i in 1:length(dataF$Area)){
  print(i)
  if(dataF$Area[i]>800){
    dataF$Area[i]=dataF$Area[i]+1}else {
      dataF$Area[i]=dataF$Area[i]-1
    }
}
#Writes to documents folder
write.csv(dataF,file="result.csv")
View(dataF)