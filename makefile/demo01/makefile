CC = gcc

# make .out
template: template.o
	$(CC) -no-pie -o template template.o

# make .o
template.o: template.c
	$(CC) -c -o template.o template.c

clean:
	rm template.o template
