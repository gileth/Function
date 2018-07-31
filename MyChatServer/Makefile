all : main client
main : main.o Server.o MyDataBase.o
	g++ -o main main.o MyDataBase.o Server.o -lmysqlclient -lpthread 
Server.o : Server.cpp
	g++ -std=c++11 -c Server.cpp
MyDataBase.o : MyDataBase.cpp
	g++ -std=c++11 -c MyDataBase.cpp
main.o : main.cpp
	g++ -std=c++11 -c main.cpp
client: client.o
	g++ -std=c++11 -o client client.o
client.o: client.cpp
	g++ -std=c++11 -c client.cpp
clean:
	rm *.o
