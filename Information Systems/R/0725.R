library()

search()

install.packages('gtrendsR', dependencies = T)
library('gtrendsR')
bitcoin=gtrends("bitcoin")
obama=gtrends("obama")

install.packages('RMySQL')
library('RMySQL')

connection1<-dbConnect(MySQL(), user='root',password='hello123',
            db_name='db',host='db.coqa5chqqklu.us-east-1.rds.amazonaws.com')

rs <- dbSendQuery(connection1, 'show databases')
rs <- dbSendQuery(connection1, 'create database nach')

data1 <- fetch(rs, n=-1)
data1

rs <- dbSendQuery(connection1, 'use mysql')
rs <- dbSendQuery(connection1, 'show tables')


