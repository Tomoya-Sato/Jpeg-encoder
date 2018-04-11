main: main.o encode.o
	g++ -o $@ $^

clean:
	rm -f main *.o

