/* Autora: Juliana Zambon (jz22), GRR20224168 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "lib.h"

/* formato de imagem PGM (Portable Grey Map)*/
/* existe o formato P2 (ASCII) e o formato P5 (binário) */

/*--------------------------------------------------------------------*/
/* Funções para alocação de memória */

/* Aloca memória para a imagem PGM */
struct imagemPGM *alocar_imagem(int largura, int altura)
{
    struct imagemPGM *img = (struct imagemPGM *)malloc(sizeof(struct imagemPGM));
    if (img == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a imagem\n");
        exit(EXIT_FAILURE);
    }

    img->largura = 0;
    img->altura = 0;
    img->pixels = NULL; /* ponteiro inicializa nulo */
    img->tipo[0] = 'P';
    img->tipo[1] = '0';

    return img;
}

/* Aloca memória para o histograma LBP */
struct LBPHistograma *alocar_histograma()
{
    struct LBPHistograma *histograma = (struct LBPHistograma *)malloc(sizeof(struct LBPHistograma));
    if (histograma == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o histograma\n");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < 256; i++)
    {
        histograma->histograma[i] = 0;
    }

    histograma->tamanho = 0.0; /* eh um double */

    return histograma;
}

/* Aloca memória para os pixels da imagem */
unsigned char **alocar_pixels(int largura, int altura)
{
    /* aloca memória para cada linha */
    unsigned char **pixels = (unsigned char **)malloc(altura * sizeof(unsigned char *));

    if (pixels == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para os pixels\n");
        exit(EXIT_FAILURE);
    }

    int i;
    /* aloca memória para cada coluna da imagem */
    for (i = 0; i < altura; i++)
    {
        pixels[i] = (unsigned char *)malloc(largura * sizeof(unsigned char));
        if (pixels[i] == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para os pixels\n");
            exit(EXIT_FAILURE);
        }
    }

    return pixels;
}

/*--------------------------------------------------------------------*/
/* Funções para manipulação de pixels */

/* Ignorar Linhas de Comentário: garantir que os comentários
(linhas que começam com #) sejam ignorados ao ler arquivos P2 e P5*/
// void ignorar_comentarios(FILE *arquivo)
// {
//     char c = fgetc(arquivo);
//     while (c == '#')
//     {
//         while (fgetc(arquivo) != '\n')
//             ;
//         c = fgetc(arquivo);
//     }
//     /* volta um caractere */
//     fseek(arquivo, -1, SEEK_CUR);
// }

/* Preenche a matriz de pixels para o formato P2 */
/* deve usar a função ignorar_comentarios */
struct imagemPGM *preencher_pixels_P2(FILE *arquivo, struct imagemPGM *img)
{
    // ignorar_comentarios(arquivo);

    int i, j;
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            fscanf(arquivo, "%d", &img->pixels[i][j]);
        }
    }

    return img;
}

/* Preenche a matriz de pixels para o formato P5 */
/* ler caractere */
struct imagemPGM *preencher_pixels_P5(FILE *arquivo, struct imagemPGM *img)
{
    // ignorar_comentarios(arquivo);

    int i, j;
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            fscanf(arquivo, "%c", &img->pixels[i][j]);
        }
    }

    return img;
}

/* Função para ler a imagem PGM */
struct imagemPGM *ler_imagem(FILE *arquivo, struct imagemPGM *img, char *tipo)
{
    /* ler o cabeçalho ignorando os comentarios */
    // ignorar_comentarios();


    /* ler o cabeçalho da imagem */
    /* são 3 linhas */
    fscanf (arquivo, "%s", img->tipo);
    fscanf (arquivo, "%d %d", img->largura, img->altura);
    fscanf (arquivo, "%d", img->maximo);

    /* para testar */
    printf("%d\n", img->maximo);

    if (strcmp(tipo, "P2") == 0)
    {
        img = preencher_pixels_P2(arquivo, img);
    }
    else if (strcmp(tipo, "P5") == 0)
    {
        img = preencher_pixels_P5(arquivo, img);
    }
    else
    {
        fprintf(stderr, "Formato de imagem inválido\n");
        exit(EXIT_FAILURE);
    }

    return img;
}

/*--------------------------------------------------------------------*/
/* Funções para processamento LBP */

/* Inicializa uma nova imagem para armazenar o resultado do LBP */
void inicializar_nova_imagem(struct imagemPGM *nova_img, int largura, int altura);

/* Calcula o LBP para um pixel específico da imagem */
/* Remover Bordas Durante a Convolução: garantir que as bordas da imagem original não sejam processadas.
Assegurando que o cálculo comece após as bordas (por exemplo, começando de 1 até altura - 1 e largura - 1).*/
void calcula_lbp(struct imagemPGM *img, struct imagemPGM *nova, int i, int j);

/* Gera a imagem LBP a partir da imagem original */
void gerar_lbp(struct imagemPGM *img, struct imagemPGM *nova);

/*--------------------------------------------------------------------*/
/* Funções de entrada e saída */

/* Gera a imagem de saída no formato PGM */
/* Formato de Saída: capaz de determinar o formato da imagem de entrada
e gerar a saída correspondente no mesmo formato (P2 ou P5)*/
void gerar_imagem_saida(struct imagemPGM *nova, FILE *arquivo_saida);

/*--------------------------------------------------------------------*/
/* Funções para histogramas */

/* Gera o histograma LBP de uma imagem */
/* Normalização do Histograma: garantindo que a contagem de pixels de cada tom seja dividida
pelo total de pixels na imagem.*/
void gerar_histograma(struct imagemPGM *nova, struct LBPHistograma *histograma);

/* Calcula a distância euclidiana entre dois histogramas LBP */
double distancia_euclidiana_hist(struct LBPHistograma *hist1, struct LBPHistograma *hist2);

/* Lê um histograma LBP a partir de um arquivo binário */
struct LBPHistograma *ler_histograma_bin(FILE *arquivo, struct LBPHistograma *histograma);

/* Escreve um histograma LBP em um arquivo binário */
void escrever_histograma_bin(FILE *arquivo, struct LBPHistograma *histograma);

/*--------------------------------------------------------------------*/
/* Funções de leitura de diretório */

/* Lê o conteúdo de um diretório e retorna uma lista de arquivos de imagem */
void ler_diretorio(char *nome_diretorio, char lista_arquivos[][256], int *num_arquivos);

/* Compara uma imagem de teste com todas as imagens LBP da base de referência */
void comparar_imagens(char *diretorio, struct LBPHistograma *histograma_teste, char *img_mais_similar, double *menor_distancia);

/*--------------------------------------------------------------------*/
/* Funções utilitárias */

/* Libera a memória alocada para uma imagem PGM */
void liberar_imagem(struct imagemPGM *img)
{
    int i;
    for (i = 0; i < img->altura; i++)
    {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}

/* Libera a memória alocada para um histograma LBP */
void liberar_histograma(struct LBPHistograma *histograma)
{
    free(histograma);
}
