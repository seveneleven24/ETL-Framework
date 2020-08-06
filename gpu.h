#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <semaphore.h>

void __global__ findRow(char* data, char* index);
void gpu(char* h_data, char* h_index, long int size, int thread);
