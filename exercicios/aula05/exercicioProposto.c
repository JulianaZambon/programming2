#include <stdio.h>

/*
O arquivo file.bin contem um trecho de um poema. Cada caracter do texto está codificado em 5 bytes,
sendo 4 bytes para a posição do catacter no texto e 1 byte para o caracter. Por exemplo, a sequencia
de bytes com o conteúdo 64n indica que o caracter 'n' deve ser colocado na posição 64 da string de texto.
Escreva um programa em C que decodifique a mensagem contida no aquivo file.bin e a imprima na tela. 
*/

int main() {

    FILE *file = fopen("file.bin", "rb");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END); 
    long size = ftell(file); 
    fseek(file, 0, SEEK_SET); 

    char *text = (char *) malloc(size); // acho que tem que ser string?
    fread(text, 1, size, file); 

    for (int i = 0; i < size; i += 5) {
        int pos = 0;
        for (int j = 0; j < 4; j++) {
            pos = pos * 10 + text[i + j] - '0';
        }
        printf("%c", text[i + 4]);
    }

    fclose(file);
    free(text);

    return 0;
}