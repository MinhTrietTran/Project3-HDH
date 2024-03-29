# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort testchar teststring testint testhelp testascii testsort scheduler ping pong testab main voinuoc sinhvien testfile



start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult



testFunc.o: testFunc.c
	$(CC) $(CFLAGS) -c testFunc.c
testFunc: testFunc.o start.o
	$(LD) $(LDFLAGS) start.o testFunc.o -o testFunc.coff
	../bin/coff2noff testFunc.coff testFunc

createfile.o: createfile.c
	$(CC) $(CFLAGS) -c createfile.c
createfile: createfile.o start.o
	$(LD) $(LDFLAGS) start.o createfile.o -o createfile.coff
	../bin/coff2noff createfile.coff createfile

testchar.o: testchar.c
	$(CC) $(CFLAGS) -c testchar.c
testchar: testchar.o start.o
	$(LD) $(LDFLAGS) start.o testchar.o -o testchar.coff
	../bin/coff2noff testchar.coff testchar

teststring.o: teststring.c
	$(CC) $(CFLAGS) -c teststring.c
teststring: teststring.o start.o
	$(LD) $(LDFLAGS) start.o teststring.o -o teststring.coff
	../bin/coff2noff teststring.coff teststring

testint.o: testint.c
	$(CC) $(CFLAGS) -c testint.c
testint: testint.o start.o
	$(LD) $(LDFLAGS) start.o testint.o -o testint.coff
	../bin/coff2noff testint.coff testint

testhelp.o: testhelp.c
	$(CC) $(CFLAGS) -c testhelp.c
testhelp: testhelp.o start.o
	$(LD) $(LDFLAGS) start.o testhelp.o -o testhelp.coff
	../bin/coff2noff testhelp.coff testhelp

testascii.o: testascii.c
	$(CC) $(CFLAGS) -c testascii.c
testascii: testascii.o start.o
	$(LD) $(LDFLAGS) start.o testascii.o -o testascii.coff
	../bin/coff2noff testascii.coff testascii

testsort.o: testsort.c
	$(CC) $(CFLAGS) -c testsort.c
testsort: testsort.o start.o
	$(LD) $(LDFLAGS) start.o testsort.o -o testsort.coff
	../bin/coff2noff testsort.coff testsort

ping.o: ping.c
	$(CC) $(CFLAGS) -c ping.c
ping: ping.o start.o
	$(LD) $(LDFLAGS) start.o ping.o -o ping.coff
	../bin/coff2noff ping.coff ping

pong.o: pong.c
	$(CC) $(CFLAGS) -c pong.c
pong: pong.o start.o
	$(LD) $(LDFLAGS) start.o pong.o -o pong.coff
	../bin/coff2noff pong.coff pong


scheduler.o: scheduler.c
	$(CC) $(CFLAGS) -c scheduler.c
scheduler: scheduler.o start.o
	$(LD) $(LDFLAGS) start.o scheduler.o -o scheduler.coff
	../bin/coff2noff scheduler.coff scheduler

tesab.o: testab.c
	$(CC) $(CFLAGS) -c testab.c
testab: testab.o start.o
	$(LD) $(LDFLAGS) start.o testab.o -o testab.coff
	../bin/coff2noff testab.coff testab

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
main: main.o start.o
	$(LD) $(LDFLAGS) start.o main.o -o main.coff
	../bin/coff2noff main.coff main

sinhvien.o: sinhvien.c
	$(CC) $(CFLAGS) -c sinhvien.c
sinhvien: sinhvien.o start.o
	$(LD) $(LDFLAGS) start.o sinhvien.o -o sinhvien.coff
	../bin/coff2noff sinhvien.coff sinhvien

voinuoc.o: voinuoc.c
	$(CC) $(CFLAGS) -c voinuoc.c
voinuoc: voinuoc.o start.o
	$(LD) $(LDFLAGS) start.o voinuoc.o -o voinuoc.coff
	../bin/coff2noff voinuoc.coff voinuoc




testfile.o: testfile.c
	$(CC) $(CFLAGS) -c testfile.c
testfile: testfile.o start.o
	$(LD) $(LDFLAGS) start.o testfile.o -o testfile.coff
	../bin/coff2noff testfile.coff testfile
