#include "gpu.h"

__global__ void findRow(char* data, char* index)
{
	int num = (blockIdx.x * blockDim.x + threadIdx.x)*4;
	int id2 = num / 4;
	int id = threadIdx.x * 4;
	extern __shared__ char s[];
	s[id] = data[num];
	s[id+1] = data[num+1];
	s[id+2] = data[num+2];
	s[id+3] = data[num+3];
	if (s[id] == '|' || s[id] == '\n')
	{
		index[id2] = 1;
		return;
	}
	if (s[id +1] == '|' || s[id+1] == '\n')
	{
		index[id2] = 2;
		return;
	}
	if (s[id +2] == '|' || s[id+2] == '\n')
	{
		index[id2] = 3;
		return;
	}
	if (s[id +3] == '|' || s[id+3] == '\n')
	{
		index[id2] = 4;
		return;
	}
	else
		index[id2] = 0;
}

void gpu(char* h_data, char* h_index, long int size, int thread)
{
	const int nStreams = 8;
	cudaStream_t streams[nStreams];
	long int dsize = size / nStreams;

	cudaFree(0);
	for (int i = 0; i < nStreams; i++)
	{
		cudaStreamCreate(&streams[i]);
	}	
	cudaError_t err = cudaSuccess;
//	char* h_index;
//	h_index = (char*)malloc(sizeof(char) * dsize * nStreams);

	for (int i = 0; i < nStreams; i++)
	{
		char *d_data, *d_index;

		int offset = dsize * i;

		//Alloc data on device
		err = cudaHostAlloc((void**)&d_data, dsize * sizeof(char), cudaHostAllocDefault);
		//err = cudaMalloc((void**)&d_data, len * sizeof(char));
		if (err != cudaSuccess)
		{
			fprintf(stderr, "Failed to malloc data: %s\n", cudaGetErrorString(err));
			exit(EXIT_FAILURE);
		}

		//alloc index on device
		//err = cudaMalloc((void**)&d_index, dsize * sizeof(char));
		err = cudaHostAlloc((void**)&d_index, dsize * sizeof(char), cudaHostAllocDefault);
		if (err != cudaSuccess)
		{
			fprintf(stderr, "Failed to malloc index: %s\n", cudaGetErrorString(err));
			exit(EXIT_FAILURE);
		}

		//memcopy from host to device
		err = cudaMemcpyAsync(d_data, h_data+offset, dsize * sizeof(char), cudaMemcpyHostToDevice, streams[i]);
		if (err != cudaSuccess)
		{
			fprintf(stderr, "Failed to copy: %s in stream %d\n", cudaGetErrorString(err),i);
			exit(EXIT_FAILURE);
		}

		//kernal execute
		int block = dsize / 4096 + 1;
		findRow << < block, 1024, 4096 * sizeof(char) , streams[i] >> > (d_data, d_index);
		if (err != cudaSuccess)
		{
			fprintf(stderr, "Failed to findRow: %s in stream %d\n", cudaGetErrorString(err),i);
			exit(EXIT_FAILURE);
		}

		//memcopy from device to host
		err = cudaMemcpyAsync(h_index+offset/4, d_index, dsize * sizeof(char) / 4, cudaMemcpyDeviceToHost, streams[i]);
		if (err != cudaSuccess)
		{
			fprintf(stderr, "Failed to return index: %s in stream %d\n", cudaGetErrorString(err),i);
			exit(EXIT_FAILURE);
		}


	}
	for(int i=0;i<nStreams;i++)
	{
		cudaStreamDestroy(streams[i]);
	}
}

