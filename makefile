
CC = gcc
CFLAGS = -O3 -g -Wall


all: tp1

tp1: tp1.o TAD_fila.o TAD_operacao.o TAD_serializacao.o TAD_arvoreB+.o
	$(CC) $(CFLAGS) tp1.o TAD_fila.o TAD_operacao.o TAD_serializacao.o TAD_arvoreB+.o -o tp1.exe

tp1.o: tp1.c TAD_fila.h TAD_operacao.h TAD_serializacao.h TAD_arvoreB+.h
	$(CC) $(CFLAGS) -c tp1.c


TAD_fila.o: TAD_fila.h TAD_fila.c
	$(CC) $(CFLAGS) -c TAD_fila.c 
TAD_operacao.o: TAD_operacao.h TAD_operacao.c
	$(CC) $(CFLAGS) -c TAD_operacao.c 
TAD_serializacao.o: TAD_serializacao.h TAD_serializacao.c
	$(CC) $(CFLAGS) -c TAD_serializacao.c 
TAD_arvoreB+.o: TAD_arvoreB+.h TAD_arvoreB+.c
	$(CC) $(CFLAGS) -c TAD_arvoreB+.c 

clean:
	rm -f tp1.exe tp1.o TAD_fila.o TAD_operacao.o TAD_serializacao.o TAD_arvoreB+.o

