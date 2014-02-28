tar: main.o huff_creat.o 
	gcc -o  tar main.o huff_creat.o  -g
main.o: main.c hufftree.h
	gcc -c -g main.c 
huff_creat.o: huff_creat.c hufftree.h
	gcc -c -g huff_creat.c 
clean:
	rm -f *.o compression.tar decompression.souce info.code

# this is makefile
