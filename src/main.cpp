#include <cstring>
#include <iostream>
#include <string>
#include <vector> 
#include "insert.h"

int main(int argc, char** argv) 
{
	std::string server_host("localhost");  
	int port = 6274;                       

	if (argc < 8) {
		std::cout << "Usage: <table> <filename> <thread> <stage 1> <stage 2> <stage 3> <stage 4> <stage 5>" << std::endl;
		return 1;
	}

	std::string table_name(argv[1]);
	std::string db_name = "arrow";
	std::string user_name = "ming";
	std::string passwd = "123";
	const char* filename = argv[2];
	int threadnumber = atoi(argv[3]);
	int s1 = atoi(argv[4]);
	int s2 = atoi(argv[5]);	
	int s3 = atoi(argv[6]);	
	int s4 = atoi(argv[7]);	
	int s5 = atoi(argv[8]);
	auto timer1 = std::chrono::steady_clock::now();
    insertdata(table_name,filename,threadnumber,s1,s2,s3,s4,s5);

	std::chrono::duration<double> timePassed1 = std::chrono::steady_clock::now() - timer1;
	std::cout << "Total time:  " <<  timePassed1.count() << '\n';

	return 0;
}
