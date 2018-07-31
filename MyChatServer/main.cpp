#include "Server.h"

int main(int argc, char const *argv[])
{		
	SetUp(9999, AF_INET, SOCK_STREAM, "mydb", "test2");		
	Start();
	Run();
	return 0;
}