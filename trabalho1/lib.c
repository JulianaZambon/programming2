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

    img->largura = largura;
    img->altura = altura;
    img->pixels = alocar_pixels(largura, altura);

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

    histograma->tamanho = 0;

    return histograma;
}

/* Aloca memória para os pixels da imagem */
unsigned char **alocar_pixels(int largura, int altura)
{
    unsigned char **pixels = (unsigned char **)malloc(altura * sizeof(unsigned char *));

    if (pixels == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para os pixels\n");
        exit(EXIT_FAILURE);
    }

    int i;
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
void ignorar_comentarios(FILE *arquivo)
{
    char c = fgetc(arquivo);
    while (c == '#')
    {
        while (fgetc(arquivo) != '\n')
            ;
        c = fgetc(arquivo);
    }
    fseek(arquivo, -1, SEEK_CUR);
}

/* Preenche a matriz de pixels para o formato P2 */
/* deve usar a função ignorar_comentarios */
struct imagemPGM *preencher_pixels_P2(FILE *arquivo, struct imagemPGM *img)
{
    ignorar_comentarios(arquivo);

    int i, j;
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            fscanf(arquivo, "%hhu", &img->pixels[i][j]);
        }
    }

    return img;
}

/* Preenche a matriz de pixels para o formato P5 */
struct imagemPGM *preencher_pixels_P5(FILE *arquivo, struct imagemPGM *img)
{
    ignorar_comentarios(arquivo);

    int i, j;
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            img->pixels[i][j] = fgetc(arquivo);
        }
    }

    return img;
}

/* Função para ler a imagem PGM */
struct imagemPGM *ler_imagem(FILE *arquivo, struct imagemPGM *img, char *tipo)
{
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
void nova_imagem_inicializa(struct imagemPGM *img, struct imagemPGM *nova)
{
    nova->largura = img->largura - 2;
    nova->altura = img->altura - 2;
    nova->pixels = alocar_pixels(nova->largura, nova->altura);
}

/* Calcula o LBP para um pixel específico da imagem */
/* Remover Bordas Durante a Convolução: garantir que as bordas da imagem original não sejam processadas.
Assegurando que o cálculo comece após as bordas (por exemplo, começando de 1 até altura - 1 e largura - 1).*/
void calcula_lbp(struct imagemPGM *img, struct imagemPGM *nova, int i, int j)
{
    unsigned char lbp = 0;
    unsigned char centro = img->pixels[i][j];

    /* Máscara de vizinhança 3x3 */
    unsigned char vizinhanca[8] = {img->pixels[i - 1][j - 1], img->pixels[i - 1][j], img->pixels[i - 1][j + 1],
                                   img->pixels[i][j + 1], img->pixels[i + 1][j + 1], img->pixels[i + 1][j],
                                   img->pixels[i + 1][j - 1], img->pixels[i][j - 1]};

    /* Cálculo do LBP */
    int k;
    for (k = 0; k < 8; k++)
    {
        if (vizinhanca[k] >= centro)
        {
            lbp |= (1 << k);
        }
    }

    nova->pixels[i - 1][j - 1] = lbp;
}

/* Gera a imagem LBP a partir da imagem original */
void gerar_lbp(struct imagemPGM *img, struct imagemPGM *nova)
{
    /* aloca memória para a nova imagem */
    nova->largura = img->largura - 2;
    nova->altura = img->altura - 2;
    nova->maximo = img->maximo;                                /* Preserva o valor máximo de cinza */
    nova->pixels = alocar_pixels(nova->largura, nova->altura); /* Aloca os pixels */

    /* Calcula o LBP para cada pixel da imagem original */
    int i, j;
    for (i = 1; i < img->altura - 1; i++)
    {
        for (j = 1; j < img->largura - 1; j++)
        {
            calcula_lbp(img, nova, i, j);
        }
    }
}

/*--------------------------------------------------------------------*/
/* Funções de saída */

/* Gera a imagem de saída no formato PGM */
/* Formato de Saída: capaz de determinar o formato da imagem de entrada
e gerar a saída correspondente no mesmo formato (P2 ou P5)*/
void gerar_imagem_saida(struct imagemPGM *nova, FILE *arquivo_saida)
{
    fprintf(arquivo_saida, "P2\n");
    fprintf(arquivo_saida, "%d %d\n", nova->largura, nova->altura);
    fprintf(arquivo_saida, "%d\n", nova->maximo);

    int i, j;
    for (i = 0; i < nova->altura; i++)
    {
        for (j = 0; j < nova->largura; j++)
        {
            fprintf(arquivo_saida, "%hhu ", nova->pixels[i][j]);
        }
        fprintf(arquivo_saida, "\n");
    }
}

/* Funções para histogramas */

/* Define o histograma LBP a partir da imagem */
/* Normalização do Histograma: garantindo que a contagem de pixels de cada tom seja dividida
pelo total de pixels na imagem.*/
void definir_histograma(char *arquivo_entrada, struct imagemPGM *img, struct LBPHistograma *histograma)
{
    FILE *arquivo = fopen(arquivo_entrada, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char tipo[3];
    fscanf(arquivo, "%s", tipo);

    int largura, altura;
    fscanf(arquivo, "%d %d", &largura, &altura);

    int maximo;
    fscanf(arquivo, "%d", &maximo);

    img = ler_imagem(arquivo, img, tipo);

    int i, j;
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            histograma->histograma[img->pixels[i][j]]++;
        }
    }

    for (i = 0; i < 256; i++)
    {
        histograma->histograma[i] /= img->largura * img->altura;
    }

    fclose(arquivo);
}

/* Calcula a distância euclidiana entre histogramas LBP */
void distancia_euclidiana_histLBP(struct LBPHistograma *aux, struct LBPHistograma *lbp_origem, struct LBPHistograma *lbp_comparar)
{
    int i;
    double distancia = 0;
    for (i = 0; i < 256; i++)
    {
        distancia += pow(lbp_origem->histograma[i] - lbp_comparar->histograma[i], 2);
    }

    aux->tamanho = sqrt(distancia);
}

/* Lê a estrutura LBP a partir de um arquivo */
struct LBPHistograma *ler_estrutura_lbp(FILE *arquivo_lbp, struct LBPHistograma *histograma)
{
    int i;
    for (i = 0; i < 256; i++)
    {
        fscanf(arquivo_lbp, "%d", &histograma->histograma[i]);
    }

    fscanf(arquivo_lbp, "%lf", &histograma->tamanho);

    return histograma;
}

/* Calcula a distância euclidiana entre histogramas de um diretório */
void distancia_euclidiana_dir(char *nome_diretorio, struct LBPHistograma *histograma, double *distancia, char menor_distancia[256])
{
    DIR *diretorio = opendir(nome_diretorio);
    if (diretorio == NULL)
    {
        fprintf(stderr, "Erro ao abrir o diretório\n");
        exit(EXIT_FAILURE);
    }

    struct dirent *arquivo;
    double menor = 1000000;
    while ((arquivo = readdir(diretorio)) != NULL)
    {
        char arquivo_entrada[256];
        snprintf(arquivo_entrada, sizeof(arquivo_entrada), "%s/%s", nome_diretorio, arquivo->d_name);

        struct stat statbuf;
        if (stat(arquivo_entrada, &statbuf) == 0 && S_ISREG(statbuf.st_mode))
        {
            if (strstr(arquivo->d_name, ".lbp") != NULL)
            {
                FILE *arquivo_lbp = fopen(arquivo_entrada, "rb");
                if (arquivo_lbp == NULL)
                {
                    fprintf(stderr, "Erro ao abrir o arquivo %s\n", arquivo_entrada);
                    continue;
                }

                struct LBPHistograma *aux = alocar_histograma();
                if (ler_estrutura_lbp(arquivo_lbp, aux) == NULL)
                {
                    fprintf(stderr, "Erro ao ler a estrutura LBP de %s\n", arquivo_entrada);
                    fclose(arquivo_lbp);
                    free(aux);
                    continue;
                }

                double dist = 0.0;
                distancia_euclidiana_histLBP(histograma, aux, &dist);

                if (dist < menor)
                {
                    menor = dist;
                    strcpy(menor_distancia, arquivo_entrada);
                }

                fclose(arquivo_lbp);
                free(aux);
            }
        }
    }

    *distancia = menor;
    closedir(diretorio);
}

/*--------------------------------------------------------------------*/
/* Funções de leitura de diretório */

/* Lê todos os arquivos de um diretório */
void ler_diretorio(char *nome_diretorio)
{
    DIR *diretorio = opendir(nome_diretorio);
    if (diretorio == NULL)
    {
        fprintf(stderr, "Erro ao abrir o diretório\n");
        exit(EXIT_FAILURE);
    }

    struct dirent *arquivo;
    while ((arquivo = readdir(diretorio)) != NULL)
    {
        char arquivo_entrada[256];
        snprintf(arquivo_entrada, sizeof(arquivo_entrada), "%s/%s", nome_diretorio, arquivo->d_name);
        struct stat statbuf;
        if (stat(arquivo_entrada, &statbuf) == 0 && S_ISREG(statbuf.st_mode))
        {

            converter_lbp(arquivo_entrada);
        }
    }
    closedir(diretorio);
}

/* Converte a imagem LBP a partir de um arquivo de entrada */
void converter_lbp(char arquivo_entrada[256])
{
    FILE *arquivo = fopen(arquivo_entrada, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", arquivo_entrada);
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
    fclose(arquivo);

    /* Inicializa a nova imagem */
    struct imagemPGM *nova = alocar_imagem(largura - 2, altura - 2);

    /* Gera a imagem LBP */
    gerar_lbp(img, nova);

    /* Gera o nome do arquivo de saída */
    char nome_arquivo_saida[256];
    snprintf(nome_arquivo_saida, sizeof(nome_arquivo_saida), "lbp_%s", arquivo_entrada);
    FILE *arquivo_saida = fopen(nome_arquivo_saida, "w");
    if (arquivo_saida == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de saída: %s\n", nome_arquivo_saida);
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
    liberar_memoria(img);
    liberar_memoria(nova);
}

/* Função para liberar a memória alocada para a imagem */
void liberar_memoria(struct imagemPGM *img)
{
    int i;
    for (i = 0; i < img->altura; i++)
    {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}
