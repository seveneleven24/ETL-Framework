#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <mutex>
#include <semaphore.h>
#include "arrowreader.h"
#include "toOmnisci.h"
#include "gpu.h"

static std::mutex gm;

static double ss1=0;
static double ss3=0;
static double ss2=0;
static double ss4=0;
static double ss5=0;

static sem_t sema1;
static sem_t sema2;
static sem_t sema3;
static sem_t sema4;
static sem_t sema5;
static sem_t all;

void printdata(int thread, int size, char* data);
void printindex(int thread, int size, int* index);
void insertdata(std::string table_name,const char* filename,int thread, int st1, int st2, int st3, int st4, int st5);
void func(long int fileoffset, long int size, long int offset, int thread,const char* name, std::string table_name, std::string db_name, std::string user_name, std::string passwd);







