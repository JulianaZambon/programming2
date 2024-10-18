# Flags de compilação
CFLAGS = -c -Wall -Wextra  -g
LDFLAGS = -lm
ENTREGA = lbp-entrega

# Compilador
CC = gcc

# Arquivos-objeto
objects = lbp.o lib.o

# Alvo principal
all: LBP

# Regra para gerar o executável LBP
LBP: $(objects)
	$(CC) -o LBP $(objects) $(LDFLAGS)

# Regras para compilar cada arquivo objeto
lbp.o: lbp.c lib.h
	$(CC) -c $(CFLAGS) lbp.c

lib.o: lib.c lib.h
	$(CC) -c $(CFLAGS) lib.c

# Regra para gerar o arquivo TGZ para entrega
#entrega: clean
#	mkdir $(ENTREGA)
#	cp *.c *.h Makefile $(ENTREGA)
#	chmod 600 $(ENTREGA)
#	tar czvf $(ENTREGA).tgz $(ENTREGA)
#	rm -rf $(ENTREGA)
#	@echo "Arquivo $(ENTREGA).tgz criado para entrega"

# Regra de limpeza
clean:
	rm -f $(objects) LBP
