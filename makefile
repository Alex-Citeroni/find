make: output find
output/%.o: source/%.c $(wildcard source/include/*.h)
	gcc -c -g -o $@ $< -I ./source/include -Wall -D_FILE_OFFSET_BITS=64
find: $(patsubst source/%.c, output/%.o, $(wildcard source/*.c))
	gcc -o $@ $^ -I ./source/include -Wall -D_FILE_OFFSET_BITS=64
output:
	mkdir -p output
clean:
	rm -f output/*.o
	rm -f find
	rm -fd output