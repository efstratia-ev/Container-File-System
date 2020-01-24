all: cfs

cfs: main.o cfs_functions.o cfs_file.o cfs_elmnt.o various.o
	g++ main.o cfs_functions.o cfs_file.o cfs_elmnt.o various.o -o cfs

various.o: various.cpp various.h
	g++ -c various.cpp

cfs_elmnt.o: cfs_elmnt.cpp cfs_elmnt.h
	g++ -c cfs_elmnt.cpp

cfs_file.o: cfs_file.cpp cfs_file.h
	g++ -c cfs_file.cpp cfs_elmnt.cpp

cfs_functions.o: cfs_functions.cpp cfs_functions.h
	g++ -c cfs_functions.cpp cfs_file.cpp various.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o cfs

push:
	git add *
	git commit -m "cfs_touch & reset_timestamps. na milisoume gia cfs_file.cpp ln148"
	git push origin master

pull:
	git pull