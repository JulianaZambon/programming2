# Nome do executável em maiúsculas
EXEC = LBP

# Compilador e flags
CC = gcc
CFLAGS = -Wall -O2

# Arquivos de origem e objetos
SRCS = lbp.c lib.c
OBJS = $(SRCS:.c=.o)

# Regras principais
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar os arquivos .c
%.o: %.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJS) $(EXEC)

# Regra de limpeza completa
cleanall: clean
	rm -f $(EXEC)

