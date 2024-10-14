/* Autora: Juliana Zambon (jz22), GRR20224168 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
#include "lib.h"

int main(int argc, char *argv[])
{
    int opt;
    char *file_in = NULL;
    char *directory = NULL;
    char *file_out = NULL;

    while ((opt = getopt(argc, argv, "d:i:o:")) != -1)
    {
        switch (opt)
        {
        case 'd':
            directory = strdup(optarg);
            break;
        case 'i':
            file_in = strdup(optarg);
            break;
        case 'o':
            file_out = strdup(optarg);
            break;
        default:
            fprintf(stderr, "Forma de uso: ./lbp -d ./base -i img1.tif\n");
            exit(1);
        }
    }

    if (!file_in || !directory)
    {
        fprintf(stderr, "Forma de uso: ./lbp -d ./base -i img1.tif\n");
        exit(1);
    }

    FILE *arquivo = fopen(file_in, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", file_in);
        exit(EXIT_FAILURE);
    }

    /* Lê o cabeçalho da imagem */
    char tipo[3];
    fscanf(arquivo, "%s", tipo);

    /* Lê a largura e a altura da imagem */
    int largura, altura;
    fscanf(arquivo, "%d %d", &largura, &altura);

    /* Lê o valor máximo de cinza */
    int maximo;
    fscanf(arquivo, "%d", &maximo);

    /* Aloca memória para a imagem */
    struct imagemPGM *img = alocar_imagem(largura, altura);

    /* Lê a imagem */
    img = ler_imagem(arquivo, img, tipo);

    /* Inicializa a nova imagem */
    struct imagemPGM *nova = alocar_imagem(largura - 2, altura - 2);

    /* Gera a imagem LBP */
    gerar_lbp(img, nova);

    /* Gera o nome do arquivo de saída */
    if (!file_out)
    {
        file_out = (char *)malloc(strlen(file_in) + 5);
        snprintf(file_out, strlen(file_in) + 5, "lbp_%s", file_in);
    }

    FILE *arquivo_saida = fopen(file_out, "w");

    if (arquivo_saida == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de saída: %s\n", file_out);
        exit(EXIT_FAILURE);
    }

    /* Escreve o cabeçalho da imagem */
    fprintf(arquivo_saida, "%s\n", tipo);
    fprintf(arquivo_saida, "%d %d\n", nova->largura, nova->altura);
    fprintf(arquivo_saida, "%d\n", maximo);

    /* Escreve a imagem */
    int i, j;
    for (i = 0; i < nova->altura; i++)
    {
        for (j = 0; j < nova->largura; j++)
        {
            fprintf(arquivo_saida, "%hhu ", nova->pixels[i][j]);
        }

        fprintf(arquivo_saida, "\n");
    }

    fclose(arquivo_saida);
    if (directory)
    {
        struct LBPHistograma *histograma = alocar_histograma();
        double distancia;
        char menor_distancia[256];

        distancia_euclidiana_dir(directory, histograma, &distancia, menor_distancia);
    }

    fclose(arquivo);

    return 0;
}
