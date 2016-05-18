#Assemble bootload.asm and creates output file bootload
nasm bootload.asm

#This creates a blank disk image
dd if=/dev/zero of=floppya.img bs=512 count=2880

#This will put bootload into sector zero
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

#Compiles kernel.c appropriately
bcc -ansi -c -o kernel.o kernel.c

#Assembles kernel.asm correctly
as86 kernel.asm -o kernel_asm.o

#This will link kernel.o with kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o

#This will put kernel into sector three
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
