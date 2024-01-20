CC = g++

all: shycc

shycc: src/main.cpp
	$(CC)  src/main.cpp -o shycc
	chmod +x shycc

clean:
	rm -f *.o *.out