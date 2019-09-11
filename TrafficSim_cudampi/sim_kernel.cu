#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

#define THREADS_PER_BLOCK 512


typedef struct _MyData{
	char name[8];
        int my_num;
}MyData;

__global__ void dot(MyData* my_data, int rank, int data_size) {
	for(int i = 0 ; i < data_size ; i++){
		my_data[i].my_num *= rank;
		printf("%dprocess[%d]:%d \n", rank, i, my_data[i].my_num);
	}
}

void runGPU(vector<MyData>* v, int rank){
	MyData* dev_data;
	MyData* host_data;

	host_data = &(v->at(0));
        
	cudaMalloc((void**) & dev_data, v->size() * sizeof(MyData));

	cudaMemcpy(dev_data, host_data, v->size() * sizeof(MyData), cudaMemcpyHostToDevice);

	for (int i = 0; i < v->size(); i++) {
		//printf("%dprocess[%d]:%d \n", rank, i, host_data[i].my_num);
	}

	dot<<< 1, 1 >>>(dev_data, rank, v->size());


	cudaMemcpy(host_data, dev_data, v->size() * sizeof(MyData), cudaMemcpyDeviceToHost);


	FILE *fp = fopen("cuda_mpi.txt", "w");
	for (int i = 0; i < v->size(); i++) {
		fprintf(fp, "%dprocess[%d]:%d \n", rank, i, host_data[i].my_num);
	}
	fclose(fp);	

	cudaFree(dev_data);
}
