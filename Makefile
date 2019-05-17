CC=g++
FLAGS=-O3
CFILES=main.cpp Input.cpp
LFILES=main.o Input.o
OUTPUT=a.out

all: ${LFILES}
	${CC} ${CFILES} -o ${OUTPUT} 

main.o: main.cpp
	${CC} -c main.cpp -o main.o

Input.o: Input.cpp
	${CC} -c Input.cpp -o Input.o

clean:
	rm -rf ${LFILES}
	rm -rf ${OUTPUT} 
