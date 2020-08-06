#include<iostream>
#include<arrow/api.h>
#include<arrow/io/api.h>
#include<arrow/ipc/api.h>
#include<vector>
#include<string>

class Arrowreader
{
	public:
		Arrowreader(long int* in_index, char* in_data, int in_row, int thread);
		void print();
		std::string getString();
	private:
		void buildRecordBatch();
		void build(char* data, long int* index, int row);//, std::vector<std::shared_ptr<arrow::Array>>);
	//	void makeRecordBatch(std::vector<std::shared_ptr<arrow::Array>> in);
	//	void toString();

		long int *index;
		char *data;
	//	std::shared_ptr<arrow::RecordBatch> recordbatch;
		int row;
		int thread;
		std::string outputString;
		

};

