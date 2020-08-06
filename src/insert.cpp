#include "insert.h"

void printindex(int thread, int size, int* index)
{
	if(thread==0)
	{
		for(int i=0;i<size;i++)
			std::cout << index[i] << " ";

		std::cout << std::endl;
	}
}

void func(long int fileoffset, long int size, long int offset, int thread,const char* name, std::string table_name, std::string db_name, std::string user_name, std::string passwd)
	
{
	sem_wait(&sema1); 
	auto timer1 = std::chrono::steady_clock::now();
	char* data = (char*)malloc(sizeof(char)*(170+size));

	std::fstream f;
	f.open(name,std::ios::in);
	f.seekp(offset,std::ios::beg);
	f.read(data+fileoffset, size+170);

	int i=0;
	if(thread!=0)
	{
		while(true)
		{
			if(data[i]=='\n')
				break;
			i++;
		}
		i++;
	}

	int j=0;
	if(thread!= 0)
	{
		for(;j<170;j++)
		{
			if(data[size+j]=='\n')
				break;
			j++;
		}
		j++;
	}
	size = size+j-i;
	if(size%32!=0)
	{
		for(int i=size;i<size+32;i++)
		{
			if(i%32==0)
			{
					size=i;
					break;
			}
				data[i]=0;
		}
	}
	data=data+i;
	auto timer11 = std::chrono::steady_clock::now();
	sem_post(&sema1);	

	sem_wait(&sema2);	
	auto timer2 = std::chrono::steady_clock::now();
	//cpu version
	long int* index = (long int*)malloc(sizeof(long int)*size/4);
	char* h_index = (char*)malloc(sizeof(char) * size);
	for(long int i=0;i<size;i=i+4)
	{
		if (data[i] == '|' || data[i] == '\n')
		{
			h_index[i/4]=1;
			continue;
		}
		if(data[i+1] == '|' || data[i+1] == '\n')
		{
			h_index[i/4]=2;
			continue;
		}
		if(data[i+2] == '|' || data[i+2] == '\n')
		{
			h_index[i/4]=3;
			continue;
		}
		if(data[i+3] == '|' || data[i+3] == '\n')
		{
			h_index[i/4]=4;
			continue;
		}
		else
			h_index[i/4]=0;
	}
	//
	//gpu version
//	gpu(data,h_index,size,thread);
		
	
	auto timer21 = std::chrono::steady_clock::now();
	sem_post(&sema2);	

	sem_wait(&sema3);	
	auto timer3 = std::chrono::steady_clock::now();
	index[0] = -1;

	int jjj=1;
	for (int i = 1; i < size/4+1; i++)
	{
		if (h_index[i] > 0 && h_index[i]<5)
		{
			index[jjj] = (int)h_index[i] + i * 4 - 1;
			jjj++;
		}
	}
	int row = (jjj-1)/17;
	auto timer31 = std::chrono::steady_clock::now();
	sem_post(&sema3);
	sem_wait(&sema4);	
	auto timer4 = std::chrono::steady_clock::now();

//	printdata(thread,size,data);
//	printindex(thread,rows*17,index);	
	Arrowreader read(index, data, row, thread);
	std::string tothrift = read.getString();	
	auto timer41 = std::chrono::steady_clock::now();

	sem_post(&sema4);

//	std::cout << tothrift << std::endl;
	sem_wait(&sema5);
	
	auto timer5 = std::chrono::steady_clock::now();
	setToThrift(user_name, passwd, db_name, table_name, tothrift);
	auto timer51 = std::chrono::steady_clock::now();
	sem_post(&sema5);

	std::chrono::duration<double> t1 = timer11 - timer1;
	std::chrono::duration<double> t2 = timer21 - timer2;
	std::chrono::duration<double> t3 = timer31 - timer3;
	std::chrono::duration<double> t4 = timer41 - timer4;
	std::chrono::duration<double> t5 = timer51 - timer5;
	std::chrono::duration<double> t6 = timer51 - timer1;

	gm.lock();
//	std::cout << thread+1 << " " << std::endl;
	std::cout << "Thread " << thread+1 << std::endl;
	std::cout << "Stage 1: " <<  t1.count() << '\n';
	std::cout << "Stage 2: " <<  t2.count() << '\n';		
	std::cout << "Stage 3: " <<  t3.count() << '\n';
	std::cout << "Stage 4: " <<  t4.count() << '\n';
	std::cout << "Stage 5: " <<  t5.count() << '\n';
//	std::cout << "ALL Stage: " <<  t6.count() << '\n';
	ss1 = ss1+t1.count();
	ss2 = ss2+t2.count();
	ss3 = ss3+t3.count();
	ss4 = ss4+t4.count();
	ss5 = ss5+t5.count();

	gm.unlock();
//		std::cout << "All Stage: " <<  t4.count() << '\n';
	
}

void insertdata(std::string table_name,const char* filename, int thread, int st1, int st2, int st3, int st4, int st5)
{
	int threadnumber = thread;
//	std::cout << "Thread: " << threadnumber << std::endl;
	FILE* f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	long int filelength = ftell(f);
	fclose(f);
	int g;
	long int size = filelength / threadnumber;
	if(filelength% threadnumber!=0)
		size++;
	std::vector<std::string> newvt;
	for(int i=0;i<threadnumber;i++)
	{
		std::string num = std::to_string(i%32+1);
		std::string newt = table_name + num;
		newvt.push_back(newt);
	}
	std::cout << st1 << " " << st2 << " " << st3 << " " << st4 << " " << st5 << std::endl; 
	int allt = st1+st2+st3+st4+st5;
	sem_init(&sema1, 0, st1);
	sem_init(&sema2, 0, st2);	
	sem_init(&sema3, 0, st3);
	sem_init(&sema4, 0, st4);
	sem_init(&sema5, 0, st5);
//	sem_init(&all,0,allt);

	std::vector<std::thread> threads;
	for(int i=0;i<threadnumber;i++)
	{
		if(i+1==threadnumber)
			threads.push_back(std::thread(func,0,filelength-i*size,size*i,i,filename,newvt[i],"arrow","ming","123"));
		else
			threads.push_back(std::thread(func,0,size,size*i,i,filename,newvt[i],"arrow","ming","123"));
	}
	for(int i=0;i<threadnumber;i++)
	{
		threads[i].join();
	}
	std::cout << "Average time: " << std::endl;
	std::cout << "Stage 1: " << ss1/thread << std::endl;
	std::cout << "Stage 2: " << ss2/thread << std::endl;
	std::cout << "Stage 3: " << ss3/thread << std::endl;
	std::cout << "Stage 4: " << ss4/thread << std::endl;
	std::cout << "Stage 4: " << ss5/thread << std::endl;

}

