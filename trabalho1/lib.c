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
struct imagemPGM *alocar_imagem()
{
    struct imagemPGM *img = (struct imagemPGM *)malloc(sizeof(struct imagemPGM));
    if (!img)
    {
        fprintf(stderr, "Erro ao alocar memória para a imagem\n");
        exit(EXIT_FAILURE);
    }

    img->largura = 0;
    img->altura = 0;
    img->maximo = 0;
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
void alocar_pixels(struct imagemPGM *img)
{
    /* aloca memória para cada linha */
    img->pixels = (unsigned char **)malloc(img->altura * sizeof(unsigned char *));

    if (img->pixels == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para os pixels\n");
        exit(EXIT_FAILURE);
    }

    int i, j;
    /* aloca memória para cada coluna da imagem */
    for (i = 0; i < img->altura; i++)
    {
        img->pixels[i] = (unsigned char *)malloc(img->largura * sizeof(unsigned char));
        if (img->pixels[i] == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para os pixels\n");
            exit(EXIT_FAILURE);
        }
        for (j = 0; j < img->largura; j++)
        {
            img->pixels[i][j] = 0;
        }
    }
}

/*--------------------------------------------------------------------*/
/* Funções para manipulação de pixels */

/* Ignorar Linhas de Comentário: garantir que os comentários
(linhas que começam com #) sejam ignorados ao ler arquivos P2 e P5*/
void ignorar_comentarios(FILE *arquivo)
{
    char linha[256]; /* Buffer para armazenar uma linha temporariamente */

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        /* Verifica se a linha começa com '#' */
        if (linha[0] != '#')
        {
            /* Se não for uma linha de comentário, volta o ponteiro ao início da linha */
            fseek(arquivo, -strlen(linha + 1), SEEK_CUR);
            break;
        }
    }
}

/* Preenche a matriz de pixels para o formato P2 */
/* usar a função ignorar_comentarios */
struct imagemPGM *preencher_pixels_P2(FILE *arquivo, struct imagemPGM *img)
{
    ignorar_comentarios(arquivo);

    int i, j, aux;
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            fscanf(arquivo, "%d", &aux); /* lê um inteiro */
            img->pixels[i][j] = aux;
        }
    }
    return img;
}

/* Preenche a matriz de pixels para o formato P5 */
/* ler caractere */
struct imagemPGM *preencher_pixels_P5(FILE *arquivo, struct imagemPGM *img)
{
    ignorar_comentarios(arquivo);

    unsigned char aux;
    int i, j;

    for (i = 0; i < (img->altura); i++)
    {
        for (j = 0; j < (img->largura); j++)
        {
            fscanf(arquivo, "%c", &aux); /* lê um caractere */
            img->pixels[i][j] = aux;
        }
    }

    return img;
}

/* Função para ler a imagem PGM */
struct imagemPGM *ler_imagem(FILE *arquivo, struct imagemPGM *img)
{
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de entrada\n");
        exit(0);
    }

    /*  ler o cabeçalho ignorando os comentarios */
    /*  ler a linha toda e verificar se encontra algum " # "
        se encontrar dá um continue pra próxima linha*/

    int estado = 0;
    while (estado < 3)
    {
        char buffer[256];
        fgets(buffer, 256, arquivo);
        /* Ignorar comentários (linhas que começam com '#') */
        if (buffer[0] == '#')
        {
            continue;
        }

        if (estado == 0)
        {
            /* Ler o tipo da imagem (P2 ou P5) */
            sscanf(buffer, "%s", img->tipo);
            estado++;
        }
        else if (estado == 1)
        {
            /* Ler a largura e a altura da imagem */
            sscanf(buffer, "%d %d", &(img->largura), &(img->altura));
            estado++;
        }
        else if (estado == 2)
        {
            /* Ler o valor máximo de cinza */
            sscanf(buffer, "%d", &(img->maximo));
            estado++;
        }
    }

    alocar_pixels(img);
    if (img->pixels == NULL)
    {
        fprintf(stderr, "Erro ao alocar pixels\n");
        exit(EXIT_FAILURE);
    }

    getc(arquivo);

    if (strstr(img->tipo, "P2") != 0)
    {
        img = preencher_pixels_P2(arquivo, img);
    }
    else if (strstr(img->tipo, "P5") != 0)
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
void inicializar_nova_imagem(struct imagemPGM *nova_imagem, struct imagemPGM *imagemPGM)
{
    /* Verifica se a nova imagem foi alocada corretamente */
    if (nova_imagem == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a nova imagem\n");
        exit(EXIT_FAILURE);
    }

    /* Copia as informações básicas da imagem original */
    strcpy(nova_imagem->tipo, imagemPGM->tipo);
    nova_imagem->largura = imagemPGM->largura;
    nova_imagem->altura = imagemPGM->altura;
    nova_imagem->maximo = imagemPGM->maximo;

    /* Aloca memória para os pixels da nova imagem */
    alocar_pixels(nova_imagem);
    if (nova_imagem->pixels == NULL)
    {
        fprintf(stderr, "Erro ao alocar pixels\n");
        exit(EXIT_FAILURE);
    }
}

/* Calcula o LBP para um pixel específico da imagem */
/* Remover Bordas Durante a Convolução: garantir que as bordas da imagem original não sejam processadas.
Assegurando que o cálculo comece após as bordas (por exemplo, começando de 1 até altura - 1 e largura - 1).*/
void calcula_lbp(struct imagemPGM *img, struct imagemPGM *nova_imagem, int i, int j)
{
    int mult = 1; /* Inicializa o multiplicador binário */
    int sum = 0;  /* Acumulador para armazenar a soma final (valor LBP) */

    /* Reseta o multiplicador binário */
    mult = 1;

    /* Aplicar a máscara com valores 2^n para os vizinhos */
    for (int lin = -1; lin <= 1; lin++)
    {
        for (int col = -1; col <= 1; col++)
        {
            /* Ignorar o pixel central */
            if (lin == 0 && col == 0)
            {
                continue; /* O pixel central não é comparado */
            }

            /* Comparar o pixel vizinho com o pixel central */
            if (img->pixels[i + lin][j + col] >= img->pixels[i][j])
            {
                /* Adiciona o peso binário correspondente (2^n) */
                sum += mult;
            }

            mult *= 2; /* Aumenta o peso binário para o próximo vizinho */
        }
    }

    /* Armazenar o valor LBP no pixel correspondente na nova imagem */
    nova_imagem->pixels[i][j] = sum;
}

/* Gera a imagem LBP a partir da imagem original */
void gerar_lbp(struct imagemPGM *img, struct imagemPGM *nova)
{
    /* Verifica se a imagem foi alocada corretamente */
    if (img == NULL || nova == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a imagem\n");
        exit(EXIT_FAILURE);
    }

    /* Inicializa a nova imagem */
    inicializar_nova_imagem(nova, img);

    /* Calcula o LBP para cada pixel da imagem */
    for (int i = 1; i < img->altura - 1; i++)
    {
        for (int j = 1; j < img->largura - 1; j++)
        {
            calcula_lbp(img, nova, i, j);
        }
    }
}

/*--------------------------------------------------------------------*/
/* Funções de entrada e saída */

/* Gera a imagem de saída no formato PGM */
/* Formato de Saída: capaz de determinar o formato da imagem de entrada
e gerar a saída correspondente no mesmo formato (P2 ou P5)*/
void gerar_imagem_saida(struct imagemPGM *nova, FILE *arquivo_saida)
{
    if (nova == NULL)
    {
        fprintf(stderr, "Erro: a imagem de saída não foi alocada corretamente.\n");
        exit(EXIT_FAILURE);
    }

    /* Escreve o cabeçalho da imagem */
    fprintf(arquivo_saida, "%s\n", nova->tipo);
    fprintf(arquivo_saida, "%d %d\n", nova->largura, nova->altura);
    fprintf(arquivo_saida, "%d\n", nova->maximo);

    /* Escreve os pixels da imagem */
    for (int i = 0; i < nova->altura; i++)
    {
        for (int j = 0; j < nova->largura; j++)
        {
            fprintf(arquivo_saida, "%d ", nova->pixels[i][j]);
        }
        fprintf(arquivo_saida, "\n");
    }

    fclose(arquivo_saida);
}

/*--------------------------------------------------------------------*/
/* Funções para histogramas */

/* Gera o histograma LBP de uma imagem */
/* Normalização do Histograma: garantindo que a contagem de pixels de cada tom seja dividida
pelo total de pixels na imagem.*/
/* Gera o histograma LBP de uma imagem com normalização */
void gerar_histograma(struct imagemPGM *nova, struct LBPHistograma *lbp_hist, char *arquivo_entrada)
{
    /* Verifica se a imagem e o histograma foram alocados corretamente */
    if (nova == NULL || lbp_hist == NULL)
    {
        fprintf(stderr, "Erro: memória não alocada para a imagem ou histograma\n");
        exit(EXIT_FAILURE);
    }

    /* Popula o histograma */
    for (int i = 0; i < nova->altura; i++)
    {
        for (int j = 0; j < nova->largura; j++)
        {
            lbp_hist->histograma[nova->pixels[i][j]]++;
        }
    }

    /* Normaliza o histograma */
    // double total_pixels = (double)(nova->altura * nova->largura);
    // for (int i = 0; i < 256; i++)
    // {
    //     lbp_hist->histograma[i] /= total_pixels;  // Normalização
    // }

    /* Gera o nome do arquivo de saída do histograma */
    char nome_arquivo[128] = {0}; /* inicializa com zero */
    strcpy(nome_arquivo, arquivo_entrada);
    strcat(nome_arquivo, ".lbp");

    /* Abre o arquivo de saída do histograma */
    FILE *arquivo_histograma = fopen(nome_arquivo, "w");
    if (arquivo_histograma == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de saída do histograma\n");
        exit(EXIT_FAILURE);
    }

    /* Escreve o histograma no arquivo */
    for (int i = 0; i < 256; i++)
    {
        fprintf(arquivo_histograma, "%d\n", lbp_hist->histograma[i]);
    }

    fclose(arquivo_histograma);
}

/* Calcula a distância euclidiana entre dois histogramas LBP */
double distancia_euclidiana(struct LBPHistograma *hist1, struct LBPHistograma *hist2)
{
    if (hist1 == NULL || hist2 == NULL)
    {
        fprintf(stderr, "Erro: um dos histogramas não foi alocado corretamente.\n");
        exit(EXIT_FAILURE);
    }

    double distancia = 0.0;

    for (int i = 0; i < 256; i++)
    {
        /* Calcula o quadrado da diferença entre os valores dos histogramas */
        int diferenca = hist1->histograma[i] - hist2->histograma[i];
        distancia += diferenca * diferenca;
    }
    return sqrt(distancia);
}

/* Lê um histograma LBP a partir de um arquivo de texto */
struct LBPHistograma *ler_histLBP(FILE *arquivo, struct LBPHistograma *histograma)
{
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de histograma\n");
        exit(EXIT_FAILURE);
    }

    /* Lê os valores do histograma do arquivo de texto */
    for (int h = 0; h < 256; h++)
    {
        if (fscanf(arquivo, "%d", &(histograma->histograma[h])) != 1)
        {
            fprintf(stderr, "Erro ao ler o valor do histograma no índice %d.\n", h);
            return NULL;
        }
    }

    return histograma;
}

/* Exibir a imagem mais similar e a distância */
/* printf("Imagem mais similar: %s %.6f\n", img_similar, distancia); */
void encontrar_imagem_similar(char *diretorio, struct LBPHistograma *histograma_teste, double *distancia, char menor_distancia[256])
{
    DIR *base;
    FILE *arquivo_hist;
    struct dirent *diretorio_base;
    char caminho[256];

    strcpy(caminho, diretorio);
    base = opendir(diretorio);

    if (base == NULL)
    {
        fprintf(stderr, "Não foi possível abrir o diretório.\n");
        exit(EXIT_FAILURE);
    }

    struct LBPHistograma *histograma_base = alocar_histograma();
    struct LBPHistograma *histograma_comparacao = alocar_histograma();
    double menor_distancia_atual = 1e12; /* Inicializa a menor distância com a distância atual */
    double distancia_atual = 0.0;

    while ((diretorio_base = readdir(base)) != NULL)
    {
        /* Ignora entradas '.' e '..' e verifica a extensão do arquivo */
        if (diretorio_base->d_name[0] == '.' || strstr(diretorio_base->d_name, ".lbp") == NULL)
            continue;

        /* Monta o caminho completo para o arquivo */
        if (snprintf(caminho, sizeof(caminho), "%s/%s", diretorio, diretorio_base->d_name) >= (int)sizeof(caminho))
        {
            fprintf(stderr, "Erro: caminho truncado\n");
            exit(EXIT_FAILURE);
        }

        arquivo_hist = fopen(caminho, "r");
        if (arquivo_hist == NULL)
        {
            fprintf(stderr, "Não foi possível abrir o arquivo de histograma.\n");
            exit(EXIT_FAILURE);
        }

        /* Lê o histograma do arquivo */
        ler_histLBP(arquivo_hist, histograma_base);

        distancia_atual = distancia_euclidiana(histograma_teste, histograma_base);

        /* Verifica se a nova distância é menor que a atual */
        if (distancia_atual < menor_distancia_atual)
        {
            menor_distancia_atual = distancia_atual;
            strcpy(menor_distancia, diretorio_base->d_name);
        }

        fclose(arquivo_hist);
    }

    closedir(base);

    /* Remove a extensão .lbp do nome da imagem mais similar */
    char *ext = strstr(menor_distancia, ".lbp");
    if (ext != NULL)
    {
        *ext = '\0';
    }

    *distancia = menor_distancia_atual;

    /* Exibe a imagem mais similar e a distância */
    printf("Imagem mais similar: %s %.6f\n", menor_distancia, *distancia);

    free(histograma_comparacao);
    free(histograma_base);
}

/*--------------------------------------------------------------------*/
/* Funções de leitura de diretório */

/* Lê o conteúdo de um diretório e executa uma operação em arquivos de imagem */
void ler_diretorio(char *nome_diretorio)
{
    /* Verifica se nome_diretorio não é nulo */
    if (nome_diretorio == NULL)
    {
        fprintf(stderr, "Erro: nome_diretorio é nulo.\n");
        exit(EXIT_FAILURE);
    }

    DIR *diretorio_base;
    struct dirent *arquivo;
    char caminho[256];

    /* Copia o nome do diretório para a variável caminho */
    strncpy(caminho, nome_diretorio, sizeof(caminho) - 1);
    caminho[sizeof(caminho) - 1] = '\0'; /* Garante que a string esteja terminada */

    /* Abre o diretório */
    diretorio_base = opendir(nome_diretorio);
    if (diretorio_base == NULL)
    {
        fprintf(stderr, "Não foi possível abrir o diretório: %s\n", nome_diretorio);
        exit(EXIT_FAILURE);
    }

    /* Lê a primeira entrada do diretório */
    arquivo = readdir(diretorio_base);
    if (arquivo == NULL)
    {
        fprintf(stderr, "Não foi possível ler o diretório: %s\n", nome_diretorio);
        closedir(diretorio_base);
        exit(EXIT_FAILURE);
    }

    /* Loop para ler as entradas do diretório */
    while (arquivo)
    {
        /* Verifica se a entrada é um arquivo .lbp */
        if (arquivo->d_name[0] != '.' && strstr(arquivo->d_name, ".lbp") == NULL)
        {
            /* Monta o caminho completo do arquivo */
            snprintf(caminho + strlen(nome_diretorio), sizeof(caminho) - strlen(nome_diretorio), "/%s", arquivo->d_name);
            converter_lbp(caminho);

            /* Reinicializa o caminho para o diretório */
            strncpy(caminho, nome_diretorio, sizeof(caminho) - 1);
            caminho[sizeof(caminho) - 1] = '\0'; /* Garante que a string esteja terminada */
        }

        /* Lê a próxima entrada */
        arquivo = readdir(diretorio_base);
    }

    closedir(diretorio_base);
}

/* Converte uma imagem PGM para o formato LBP */
void converter_lbp(char arquivo_entrada[256])
{
    FILE *arquivo = NULL;
    arquivo = fopen(arquivo_entrada, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", arquivo_entrada);
        exit(EXIT_FAILURE);
    }

    /* Aloca estrutura para a imagem de entrada */
    struct imagemPGM *img_entrada = alocar_imagem();
    if (img_entrada == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a imagem de entrada.\n");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    /* Lê a imagem do arquivo */
    img_entrada = ler_imagem(arquivo, img_entrada);
    if (img_entrada == NULL)
    {
        fprintf(stderr, "Erro ao ler a imagem do arquivo");
        liberar_imagem(img_entrada);
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    /* Aloca estrutura para a nova imagem */
    struct imagemPGM *nova_imagem = alocar_imagem();
    if (nova_imagem == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a nova imagem.\n");
        liberar_imagem(img_entrada);
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    /* Inicializa a nova imagem e gera LBP */
    inicializar_nova_imagem(nova_imagem, img_entrada);
    gerar_lbp(img_entrada, nova_imagem);

    /* Aloca estrutura para LBP */
    struct LBPHistograma *lbp = alocar_histograma();
    if (lbp == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o histograma LBP.\n");
        liberar_imagem(img_entrada);
        liberar_imagem(nova_imagem);
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    /* Gera o histograma LBP */
    gerar_histograma(nova_imagem, lbp, arquivo_entrada);

    liberar_imagem(img_entrada);
    liberar_imagem(nova_imagem);
    free(lbp);
    fclose(arquivo);
}

/*--------------------------------------------------------------------*/
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
