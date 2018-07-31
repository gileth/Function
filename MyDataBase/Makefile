all : main
main : main.o MyDataBase.o
	g++ -o main main.o MyDataBase.o -l mysqlclient
MyDataBase.o : MyDataBase.cpp
	g++ -std=c++11 -c MyDataBase.cpp
main.o : main.cpp
	g++ -std=c++11 -c main.cpp
clean:
	rm *.o
