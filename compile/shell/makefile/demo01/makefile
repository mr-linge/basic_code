cc=gcc

source=template.c

# make .o
objectfile: ${source}
	$(cc) -c template.c -o template.o

# make .out
execute: objectfile
	$(cc) -no-pie template.o -o template

clean:
	@-rm -rf template.o template
