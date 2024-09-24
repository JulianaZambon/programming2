/* neste trabalho será adotado o
formato de imagem PGM (Portable Grey Map)*/

/* existe o formato P2 (ASCII) e o formato P5 (binário).
seu programa deve aceitar ambos.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

/*
gera um mapa LBP para a imagem, onde cada pixel é
substituído por um valor de 8 bits (entre 0 e 255)
representando a relação entre o pixel central e
seus vizinhos.
*/

/* Função para calcular o LBP */
void lbp(struct imagemPGM *img, int **lbp) {
    int altura = img->altura;
    int largura = img->largura;
    
    /* Inicializar a matriz LBP com zeros */
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            lbp[i][j] = 0;
        }
    }

    /* Ignorar a borda da imagem */
    for (int i = 1; i < altura - 1; i++) {
        for (int j = 1; j < largura - 1; j++) {
            int centro = img->pixels[i][j];
            lbp[i][j] = 
                (img->pixels[i-1][j-1] >= centro ? 1 : 0) << 7 |
                (img->pixels[i-1][j]   >= centro ? 1 : 0) << 6 |
                (img->pixels[i-1][j+1] >= centro ? 1 : 0) << 5 |
                (img->pixels[i][j+1]   >= centro ? 1 : 0) << 4 |
                (img->pixels[i+1][j+1] >= centro ? 1 : 0) << 3 |
                (img->pixels[i+1][j]   >= centro ? 1 : 0) << 2 |
                (img->pixels[i+1][j-1] >= centro ? 1 : 0) << 1 |
                (img->pixels[i][j-1]   >= centro ? 1 : 0);
        }
    }
}

/* Função para calcular a distância Euclidiana */
double distancias_euclidianas(double *v1, double *v2, int n) {
    double soma = 0.0;

    for (int i = 0; i < n; i++) {
        soma += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(soma);
}

/* Função para ignorar comentários */
void ignora_comentarios(FILE *arquivo) {
    int c = fgetc(arquivo);

    while (c == '#') {
        while (fgetc(arquivo) != '\n'); /* Ignora a linha inteira */
        c = fgetc(arquivo);
    }
    ungetc(c, arquivo); /* Retorna o último caractere que não era um comentário */
}

/* Função para ler a imagem PGM (P2 e P5) */
struct imagemPGM *ler_imagem(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb"); /* Usar "rb" para suportar P5 */

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    char tipo[3];
    fscanf(arquivo, "%2s", tipo);
    tipo[2] = '\0';

    /* Verificar se o arquivo é P2 ou P5 */
    if (strcmp(tipo, "P2") != 0 && strcmp(tipo, "P5") != 0) {
        fprintf(stderr, "Formato de imagem inválido\n");
        fclose(arquivo);
        exit(1);
    }

    ignora_comentarios(arquivo);

    int largura, altura, valor_maximo;
    fscanf(arquivo, "%d %d %d", &largura, &altura, &valor_maximo); /* Ler largura, altura e valor máximo */
    fgetc(arquivo); /* Consumir o newline após o valor máximo */

    struct imagemPGM *img = malloc(sizeof(struct imagemPGM));
    img->largura = largura;
    img->altura = altura;
    img->maximo = valor_maximo;
    img->pixels = malloc(altura * sizeof(int *));

    for (int i = 0; i < altura; i++) {
        img->pixels[i] = malloc(largura * sizeof(int));
    }

    if (strcmp(tipo, "P2") == 0) { /* Formato ASCII */
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                fscanf(arquivo, "%d", &img->pixels[i][j]);
            }
        }
    } else if (strcmp(tipo, "P5") == 0) { /* Formato binário */
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                img->pixels[i][j] = fgetc(arquivo);
            }
        }
    }

    fclose(arquivo);
    return img;
}

/* Função para liberar a imagem */
void liberar_imagem(struct imagemPGM *img) {
    for (int i = 0; i < img->altura; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}

/* Função para escrever uma imagem PGM */
void escrever_imagem(const char *nome_arquivo, struct imagemPGM *img, const char *tipo) {
    FILE *arquivo = fopen(nome_arquivo, "wb"); /* Usar "wb" para suportar P5 */
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    fprintf(arquivo, "%s\n", tipo);
    fprintf(arquivo, "%d %d\n", img->largura, img->altura);
    fprintf(arquivo, "%d\n", img->maximo);

    if (strcmp(tipo, "P2") == 0) { /* Formato ASCII */
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                fprintf(arquivo, "%d ", img->pixels[i][j]);
            }
            fprintf(arquivo, "\n");
        }
    } else if (strcmp(tipo, "P5") == 0) { /* Formato binário */
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                fputc(img->pixels[i][j], arquivo);
            }
        }
    }

    fclose(arquivo);
}