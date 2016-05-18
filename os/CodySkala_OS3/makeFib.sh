bcc -ansi -c -o fib.o fib.c
as86 lib.asm -o lib_asm.o
ld86 -o fib -d fib.o lib_asm.o
