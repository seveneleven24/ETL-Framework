#include "arrowreader.h"

Arrowreader::Arrowreader(long int* in_index, char* in_data, int in_row, int in_thread)
{
	index = in_index;
	data = in_data;
	row = in_row;
	thread = in_thread;
	buildRecordBatch();
}

void Arrowreader::buildRecordBatch()
{
	build(data, index, row);
}

void Arrowreader::build(char* data, long int* index, int row)
{
	std::vector<std::shared_ptr<arrow::Array>> arrayvector;
	arrow::Status st;	
	arrow::StringBuilder builderarray[17];
	

	for(int i=0;i<17;i++)
	{
		builderarray[i].Reset();
		builderarray[i].Resize(row);
		builderarray[i].ReserveData(row*200);
	}

	for(int i=0;i<row;i++)
	{
		for(int j=0;j<17;j++)
		{
			builderarray[j].UnsafeAppend(data+index[i * 17 + j]+2, index[i * 17 + j + 1] - index[i * 17 + j] - 3);
		}
	}
	std::shared_ptr<arrow::Array> a1;
	st = builderarray[0].Finish(&a1);
	if(!st.ok())
		std::cout << "Error building array 1";

	int dd;
	std::shared_ptr<arrow::Array> a2;
	st = builderarray[1].Finish(&a2);
	if(!st.ok())
		std::cout << "Error building array 2";

	std::shared_ptr<arrow::Array> a3;
	st = builderarray[2].Finish(&a3);
	if(!st.ok())
		std::cout << "Error building array 3";

	std::shared_ptr<arrow::Array> a4;
	st = builderarray[3].Finish(&a4);
	if(!st.ok())
		std::cout << "Error building array 4";

	std::shared_ptr<arrow::Array> a5;
	st = builderarray[4].Finish(&a5);
	if(!st.ok())
		std::cout << "Error building array 5";

	std::shared_ptr<arrow::Array> a6;
	st = builderarray[5].Finish(&a6);
	if(!st.ok())
		std::cout << "Error building array 6";

	std::shared_ptr<arrow::Array> a7;
	st = builderarray[6].Finish(&a7);
	if(!st.ok())
		std::cout << "Error building array 7";

	std::shared_ptr<arrow::Array> a8;
	st = builderarray[7].Finish(&a8);
	if(!st.ok())
		std::cout << "Error building array 8";

	std::shared_ptr<arrow::Array> a9;
	st = builderarray[8].Finish(&a9);
	if(!st.ok())
		std::cout << "Error building array 9";

	std::shared_ptr<arrow::Array> a10;
	st = builderarray[9].Finish(&a10);
	if(!st.ok())
		std::cout << "Error building array 10";

	std::shared_ptr<arrow::Array> a11;
	st = builderarray[10].Finish(&a11);
	if(!st.ok())
		std::cout << "Error building array 11";

	std::shared_ptr<arrow::Array> a12;
	st = builderarray[11].Finish(&a12);
	if(!st.ok())
		std::cout << "Error building array 12";

	std::shared_ptr<arrow::Array> a13;
	st = builderarray[12].Finish(&a13);
	if(!st.ok())
		std::cout << "Error building array 13";

	std::shared_ptr<arrow::Array> a14;
	st = builderarray[13].Finish(&a14);
	if(!st.ok())
		std::cout << "Error building array 14";

	std::shared_ptr<arrow::Array> a15;
	st = builderarray[14].Finish(&a15);
	if(!st.ok())
		std::cout << "Error building array 15";

	std::shared_ptr<arrow::Array> a16;
	st = builderarray[15].Finish(&a16);
	if(!st.ok())
		std::cout << "Error building array 16";

	std::shared_ptr<arrow::Array> a17;
	st = builderarray[16].Finish(&a17);
	if(!st.ok())
		std::cout << "Error building array 17";

	arrayvector.push_back(a1);
	arrayvector.push_back(a2);
	arrayvector.push_back(a3);
	arrayvector.push_back(a4);
	arrayvector.push_back(a5);
	arrayvector.push_back(a6);
	arrayvector.push_back(a7);
	arrayvector.push_back(a8);
	arrayvector.push_back(a9);
	arrayvector.push_back(a10);
	arrayvector.push_back(a11);
	arrayvector.push_back(a12);
	arrayvector.push_back(a13);
	arrayvector.push_back(a14);
	arrayvector.push_back(a15);
	arrayvector.push_back(a16);
	arrayvector.push_back(a17);

	std::vector<std::shared_ptr<arrow::Field>> fieldvector = {arrow::field("f1", arrow::utf8()), arrow::field("f2", arrow::utf8()), arrow::field("f3", arrow::utf8()), arrow::field("f4", arrow::utf8()),  arrow::field("f5", arrow::utf8()), arrow::field("f6", arrow::utf8()), arrow::field("f7", arrow::utf8()), arrow::field("f8", arrow::utf8()), arrow::field("f9", arrow::utf8()), arrow::field("f10", arrow::utf8()), arrow::field("f11", arrow::utf8()), arrow::field("f12", arrow::utf8()), arrow::field("f13", arrow::utf8()), arrow::field("f14", arrow::utf8()), arrow::field("f15", arrow::utf8()), arrow::field("f16", arrow::utf8()), arrow::field("f17", arrow::utf8())}; 

	auto schema = std::make_shared<arrow::Schema>(fieldvector);

	std::shared_ptr<arrow::RecordBatch> record = arrow::RecordBatch::Make(schema,row,arrayvector);
	
	auto outputStream =  arrow::io::BufferOutputStream::Create(1);
	auto stream = std::move(outputStream).ValueOrDie();
	std::shared_ptr<arrow::ipc::RecordBatchWriter> writer;
	st = arrow::ipc::RecordBatchStreamWriter::Open(stream.get(), schema, &writer);	

	if(!st.ok())
		std::cout << st.ToString();


		st = writer->WriteRecordBatch(*record);
	if(!st.ok())
		std::cout << st.ToString();

	st = writer->Close();
	if(!st.ok())
		std::cout << st.ToString();

	auto finishstream = stream->Finish();
	if(!st.ok())
		std::cout << st.ToString();

	std::shared_ptr<arrow::Buffer> buffer = finishstream.ValueOrDie();
	outputString = buffer->ToString();
}




std::string Arrowreader::getString()
{
	return outputString;
}
