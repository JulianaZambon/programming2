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
    char *arquivo_entrada = NULL;
    char *diretorio = NULL;
    char *arquivo_saida = NULL;
    FILE *arquivo = NULL;

    while ((opt = getopt(argc, argv, "d:i:o:")) != -1)
    {
        switch (opt)
        {
        case 'd':
            diretorio = strdup(optarg);
            break;
        case 'i':
            arquivo_entrada = strdup(optarg);
            break;
        case 'o':
            arquivo_saida = strdup(optarg);
            break;
        default:
            fprintf(stderr, "Uso: %s -d <diretório> -i <arquivo_entrada> -o <arquivo_saida>\n", argv[0]);
            exit(1);
        }
    }

    /* Lê o arquivo de entrada */
    arquivo = fopen(arquivo_entrada, "r"); /* r = read */
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada (main) \n");
        free(arquivo_entrada);
        exit(1);
    }

    /* Aloca memória para a imagem */
    struct imagemPGM *img = alocar_imagem();

    /* Lê a imagem */
    img = ler_imagem(arquivo, img);
    if (img == NULL)
    {
        fprintf(stderr, "Erro ao ler a imagem\n");
        free(arquivo_entrada);
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    /* Aloca memória para a nova imagem */
    struct imagemPGM *nova_img = alocar_imagem();

    /* Inicializa a nova imagem */
    inicializar_nova_imagem(nova_img, img);

    gerar_lbp(img, nova_img);

    /* Abre o arquivo de saída */
    /* gerar a imagem LPB com o nome informado pela opção -o */
    FILE *arquivo_saida_ptr = fopen(arquivo_saida, "wb");

    if (arquivo_saida_ptr == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de saída\n");
        free(arquivo_entrada);
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    /* Gera a imagem LBP */
    gerar_imagem_saida(nova_img, arquivo_saida_ptr);

    /* Gera o histograma LBP */
    struct LBPHistograma *histograma = alocar_histograma();
    gerar_histograma(nova_img, histograma, arquivo_entrada);
    fclose(arquivo);

    /* Encontra a imagem mais similar */
    double distancia = 1e12;
    char menor_distancia[256];
    ler_diretorio(diretorio);

    /* Exibir a imagem mais similar e a distância */
    encontrar_imagem_similar(diretorio, histograma, &distancia, menor_distancia);

    /* Libera a memória alocada */
    free(diretorio);
    free(arquivo_entrada);
    free(arquivo_saida);
    liberar_imagem(img);
    liberar_imagem(nova_img);
    free(histograma);

    return 0;
}