ts: trafficsim.o sim_kernel.o print_message.o setup.o
	mpic++ -L/usr/local/cuda-10.0/lib64 -o ts sim_kernel.o setup.o print_message.o trafficsim.o -lcuda -lcudart -lstdc++
	scp ts ubuntu@Master:/home/ubuntu
	scp ts ubuntu@Slave1:/home/ubuntu
	scp ts ubuntu@Slave2:/home/ubuntu

trafficsim.o: trafficsim.cpp
	mpic++ -c trafficsim.cpp

print_message.o: print_message.cpp
	mpic++ -c print_message.cpp

setup.o: setup.cpp
	mpic++ -c setup.cpp

sim_kernel.o: sim_kernel.cu
	nvcc -arch=sm_30 -c sim_kernel.cu

clean:
	rm -f trafficsim.o sim_kernel.o print_message.o setup.o ts
