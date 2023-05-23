cc=gcc

outfile=daemon

daemon:
	@${cc} daemon.c -o ${outfile}

loader:
	@${cc} loader.c -o loader

check:
	@${cc} check_config.c -o check

test:
	@${cc} test.c -o test

clean:
	@-rm -rf ${outfile} *.log loader ./check
