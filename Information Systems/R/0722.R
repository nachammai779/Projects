dataF=read.csv(file="C:/Users/Nachammai/Desktop/INFSYS6891/Homework/Data_with_column_names.csv")
View(dataF)

dataF$Area

dataF$Floor

dataF=read.csv(file.choose())

dataF

dataF$Area[1]

dataF$Area[1]=10

dataF$Area[2]=dataF$Area[2]+1

dataF$Area[3]=dataF$Area[3]-1

write.csv(dataF,file = "C:/Users/Nachammai/Desktop/INFSYS6891/Homework/new.csv")

dataF = read.csv(file.choose(),header=FALSE)

fix(dataF)

x = 10
if(x>5){
  print("x is greater than 5")
}

x = 5
if(x>10){
  print("x is greater than 10")} else {
    print("x is less than or equal to 10")
  }

x=5
if(x>10){
  print("x is greater than 10")}else if(x<=10 & x>4){
    print("between 4 and 10")
  }else{
    print("less than 4")
  }

for(i in 1:10){
  print(i)
}

for(i in c(1:5, 15:20)){
  print(i)
}

x<-10
while(x>0){
  print(x)
  x=x-1
}

for (i in 1:10){
  print(i)
  if(i>4){
    break
  }
}

for(i in 1:10){
  if(i>4){
    next
  }
  print(i)
}

my_addition_fun<-function(x,y){
  return (x+y)}
my_addition_fun(5,4)
my_addition_fun(3,2)

my_addition_fun<-function(x=2,y=5){
  return (x+y)}
my_addition_fun()
my_addition_fun(x=2)
my_addition_fun(y=4)

data_a<-read.csv(file.choose())

data_a$Floor[1]=data_a$Floor[1]+1
for(i in 1:length(data_a$Floor)){
  data_a$Floor[i]=data_a$Floor[i]+1
  print(i)
}

write.csv(data_a,file="result.csv")

for(i in 1:length(data_a$Area)){
  print(i)
  if(data_a$Area[i]>800){
  data_a$Area[i]=data_a$Area[i]+1}else {
    #mycode
  }
}