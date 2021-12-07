exe_path=./*.out
if [ ! -d "$myPath"]
then
	echo ""
else
	rm $exe_path
fi

gcc aes128_block_crypt.c aes128_crypt.c main.c -o ./main.out
#./main.out
./main.out -e -p /home/dio/Repositories/C_study/tmp/crypto/test -k 1234567890abcdef
./main.out -d -p /home/dio/Repositories/C_study/tmp/crypto/test -k 1234567890abcdef

