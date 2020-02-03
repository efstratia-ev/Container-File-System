all: cfs

cfs: main.o cfs_functions.o cfs_file.o QuickSort.o cfs_elmnt.o src_dest.o various.o
	g++ main.o cfs_functions.o cfs_file.o QuickSort.o cfs_elmnt.o src_dest.o various.o -o cfs

various.o: various.cpp various.h
	g++ -c various.cpp

src_dest.o: src_dest.cpp src_dest.h
	g++ -c src_dest.cpp

cfs_elmnt.o: cfs_elmnt.cpp cfs_elmnt.h
	g++ -c cfs_elmnt.cpp

QuickSort.o: QuickSort.cpp QuickSort.h
	g++ -c QuickSort.cpp cfs_elmnt.cpp

cfs_file.o: cfs_file.cpp cfs_file.h
	g++ -c cfs_file.cpp QuickSort.cpp

cfs_functions.o: cfs_functions.cpp cfs_functions.h
	g++ -c cfs_functions.cpp cfs_file.cpp various.cpp src_dest.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o cfs

push:
	git add *
	git commit -m "final"
	git push origin master

pull:
	git pull