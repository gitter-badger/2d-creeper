cproto -DLINUX -DCPROTO -e -p -v -o proto.h *.c
gcc -DLINUX -std=c11 *.c -static -o linux_2d_creeper

