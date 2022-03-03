out.bin: 2022-03-03.c
	gcc -Werror -o out.bin 2022-03-03.c

run: out.bin
	./out.bin
