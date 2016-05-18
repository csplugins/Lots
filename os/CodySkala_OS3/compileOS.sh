#Assemble bootload.asm and creates output file bootload
nasm bootload.asm

#This creates a blank disk image
dd if=/dev/zero of=floppya.img bs=512 count=2880

#This will put bootload into sector zero
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

#This will place the disk map into sector one
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc

#This will put the disk directory into sector two
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

#Compiles kernel.c appropriately
bcc -ansi -c -o kernel.o kernel.c

#Assembles kernel.asm correctly
as86 kernel.asm -o kernel_asm.o

#This will link kernel.o with kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o

#This will put kernel into sector three
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

#This assembles lib.asm making lib.asm_o (used later on)
as86 lib.asm -o lib_asm.o


#This will compile Shell.c, cal.c, fib.c, and kitty.c
bcc -ansi -c -o Shell.o Shell.c
bcc -ansi -c -o cal.o cal.c
bcc -ansi -c -o fib.o fib.c
bcc -ansi -c -o kitty.o kitty.c

#This will load and assemble Shell.o, cal.o, fib.o, and kitty.o
ld86 -o Shell -d Shell.o lib_asm.o
ld86 -o cal -d cal.o lib_asm.o
ld86 -o fib -d fib.o lib_asm.o
ld86 -o kitty -d kitty.o lib_asm.o

#The following loads multiple files onto the disk. Some executable, some text files
./loadFile Shell
./loadFile Help
./loadFile bigMSG
./loadFile cal
./loadFile fib
./loadFile kitty
./loadFile msg
./loadFile test1
./loadFile test2
