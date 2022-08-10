cc = gcc

# make .out
template: template.o
	$(cc) -no-pie -o template template.o

# make .o
template.o: template.c
	$(cc) -c -o template.o template.c

clean:
	rm template.o template
