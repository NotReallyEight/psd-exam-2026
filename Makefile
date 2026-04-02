program: ./output/main.o
	gcc ./output/main.o -o ./output/program

./output/main.o: ./src/main.c
	gcc -c ./src/main.c -o ./output/main.o
