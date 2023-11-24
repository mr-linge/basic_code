cc = gcc

p1: pthread_01.c
	$(cc) pthread_01.c -o pthread_01 -lpthread

p2: pthread_02.c
	$(cc) pthread_02.c -o pthread_02 -lpthread

p3: pthread_03.c block_queue.c
	$(cc) pthread_03.c block_queue.c -o pthread_03 -lpthread

clean:
	@- rm -rf pthread_01
	@- rm -rf pthread_02
	@- rm -rf pthread_03
