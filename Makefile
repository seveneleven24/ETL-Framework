#Makefile

main: arrowreader.o insert.o toOmnisci.o main.o       
	g++ main.o arrowreader.o insert.o toOmnisci.o -O3 -pthread -g -rdynamic -o 123 /home/dotgg/IMing/csvtest/zzzz/libmapd_sample_thrift.a -lboost_system -lboost_program_options -lboost_regex -ldl /usr/local/lib/libthrift.so -larrow 
#gpu.o: gpu.cu          
#	nvcc gpu.cu -c -G
arrowreader.o: arrowreader.cpp
	g++ arrowreader.cpp -c -O3
toOmnisci.o: toOmnisci.cpp
	g++ -O3 -I/usr/local/cuda/include -I/home/dotgg/IMing/omniscidb/build -DHAVE_THRIFT_STD_SHAREDPTR -DHAVE_THRIFT_THREADFACTORY toOmnisci.cpp -c
insert.o: insert.cpp
	g++ insert.cpp -c -O3 -I/usr/local/cuda/include -I/home/dotgg/IMing/omniscidb/build -DHAVE_THRIFT_STD_SHAREDPTR -DHAVE_THRIFT_THREADFACTORY
main.o: main.cpp             
	g++ main.cpp -c -O3 -I/usr/local/cuda/include -I/home/dotgg/IMing/omniscidb/build -DHAVE_THRIFT_STD_SHAREDPTR -DHAVE_THRIFT_THREADFACTORY 
clean:                 
	    rm *.o

#g++ -pthread -g -rdynamic main.cpp.o -o 123 -Wl,-rpath,/usr/local/lib: libmapd_sample_thrift.a -lboost_system -lboost_program_options -lboost_regex -ldl /usr/local/lib/libthrift.so -larrow
#g++ -I/usr/local/cuda/include -I/home/dotgg/IMing/omniscidb/build -c toOmnisci.h
