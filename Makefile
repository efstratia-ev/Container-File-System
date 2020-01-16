all: cfs

cfs: main.o cfs_functions.o
	g++ main.o cfs_functions.o cfs_file.o various.o -o cfs

cfs_functions.o: cfs_functions.cpp cfs_functions.h
	g++ -c cfs_functions.cpp cfs_file.cpp various.cpp

cfs_file.o: cfs_file.cpp cfs_file.h
	g++ -c cfs_file.cpp

various.o: various.cpp various.h
	g++ -c various.cpp

clean:
	rm *.o cfs