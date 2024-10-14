/* neste trabalho será adotado o
formato de imagem PGM (Portable Grey Map)*/

/* existe o formato P2 (ASCII) e o formato P5 (binário).
seu programa deve aceitar ambos.*/

#include <stdio.h>  
#include <stdlib.h>
#include <math.h>    
#include <string.h>
#include "lib.h"


/*
gera um mapa LBP para a imagem, onde cada pixel é
substituído por um valor de 8 bits (entre 0 e 255)
representando a relação entre o pixel central e
seus vizinhos.
*/

/* Função para calcular o LBP */
void lbp(struct imagemPGM *img, unsigned char **lbp)
{
    int i, j;
    int **pixels = img->pixels;
    int largura = img->largura;
    int altura = img->altura;

    /* Calcula o valor LBP para cada pixel, exceto as bordas */
    for (i = 1; i < altura - 1; i++)
    {
        for (j = 1; j < largura - 1; j++)
        {
            int pixel_central = pixels[i][j];
            unsigned char valor = 0;

            /* Calcula o valor LBP baseado nos vizinhos */
            valor |= (pixels[i - 1][j - 1] > pixel_central) << 7;
            valor |= (pixels[i - 1][j] > pixel_central) << 6;
            valor |= (pixels[i - 1][j + 1] > pixel_central) << 5;
            valor |= (pixels[i][j + 1] > pixel_central) << 4;
            valor |= (pixels[i + 1][j + 1] > pixel_central) << 3;
            valor |= (pixels[i + 1][j] > pixel_central) << 2;
            valor |= (pixels[i + 1][j - 1] > pixel_central) << 1;
            valor |= (pixels[i][j - 1] > pixel_central) << 0;

            lbp[i][j] = valor;
        }
    }
}

/* Função para extrair um vetor de características que sumarize o conteúdo da imagem */
void extrair_vetor_caracteristicas(struct imagemPGM *img, double *vetor)
{
    /* Verifica se a imagem é válida */
    if (img == NULL || img->pixels == NULL) {
        fprintf(stderr, "Erro: imagem inválida.\n");
        return;
    }

    int i, j;
    int **pixels = img->pixels;
    int largura = img->largura;
    int altura = img->altura;
    int maximo = img->maximo;  /* Valor máximo de cinza na imagem */
    int histograma[256] = {0};
    int total = 0;

    /* Verifica se o valor máximo é maior que 255 (escala PGM não standard) */
    if (maximo > 255) {
        fprintf(stderr, "Erro: valor máximo da imagem maior que 255.\n");
        return;
    }

    /* Calcula o histograma e contabiliza pixels, descartando as bordas */
    for (i = 1; i < altura - 1; i++) {
        for (j = 1; j < largura - 1; j++) {
            histograma[pixels[i][j]]++;  /* Incrementa a contagem para o pixel correspondente */
            total++;  /* Contador total de pixels processados */
        }
    }

    /* Se não houver pixels válidos, evita divisão por zero */
    if (total == 0) {
        fprintf(stderr, "Erro: imagem sem pixels processáveis.\n");
        return;
    }

    /* Normaliza o histograma e preenche o vetor de características */
    for (i = 0; i < 256; i++) {
        vetor[i] = (double)histograma[i] / total;
    }
}

/* Função para calcular a distância Euclidiana */
double distancias_euclidianas(double *v1, double *v2, int n)
{
    double soma = 0.0;
    int i;

    for (i = 0; i < n; i++) {
        soma += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }

    return sqrt(soma);
}

/* Função para gerar a imagem LBP com o nome de saída especificado */
void gerar_imagem_lbp(struct imagemPGM *img, int **lbp, const char *nome_arquivo_saida)
{
    /* Verifica se os parâmetros são válidos */
    if (img == NULL || lbp == NULL || nome_arquivo_saida == NULL || strlen(nome_arquivo_saida) == 0) {
        return; /* Não cria a imagem de saída se algum parâmetro for inválido */
    }

    int i, j;
    int largura = img->largura;
    int altura = img->altura;

    /* Cria uma nova estrutura para a imagem LBP */
    struct imagemPGM *img_lbp = (struct imagemPGM *)malloc(sizeof(struct imagemPGM));
    if (img_lbp == NULL) {
        return; /* Falha na alocação, não cria a imagem de saída */
    }

    img_lbp->largura = largura;
    img_lbp->altura = altura;
    img_lbp->maximo = 255;  /* Valor máximo de cinza (LBP usa 8 bits por pixel) */

    /* Aloca memória para os ponteiros de linha da imagem LBP */
    img_lbp->pixels = (int **)malloc(altura * sizeof(int *));
    if (img_lbp->pixels == NULL) {
        free(img_lbp); /* Libera a memória alocada para img_lbp */
        return; /* Falha na alocação de memória */
    }

    /* Aloca memória para cada linha da imagem LBP */
    for (i = 0; i < altura; i++) {
        img_lbp->pixels[i] = (int *)malloc(largura * sizeof(int));
        if (img_lbp->pixels[i] == NULL) {
            /* Se falhar em alocar para alguma linha, libera a memória já alocada */
            for (j = 0; j < i; j++) {
                free(img_lbp->pixels[j]);
            }
            free(img_lbp->pixels);
            free(img_lbp);
            return; /* Falha na alocação de memória */
        }
    }

    /* Preenche a imagem LBP com os valores gerados */
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++) {
            img_lbp->pixels[i][j] = lbp[i][j];
        }
    }

    /* O tipo da imagem LBP é sempre P2, pois os valores são de 0 a 255 */
    const char *tipo = "P2";

    /* Escreve a imagem LBP com o nome especificado na opção -o */
    escrever_imagem(nome_arquivo_saida, img_lbp, tipo);

    /* Libera a memória da imagem LBP */
    liberar_imagem(img_lbp);
}


/* Função para gravar o vetor LBP em um arquivo binário */
void gravar_vetor_lbp(const char *nome_arquivo, double *vetor_lbp, int tamanho)
{
    /* Validação de parâmetros */
    if (vetor_lbp == NULL || tamanho <= 0) {
        return;  /* Não faz nada se o vetor é nulo ou tamanho é inválido */
    }

    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (arquivo == NULL) {
        return;  /* Não faz nada se não for possível abrir o arquivo */
    }

    /* Grava o vetor LBP no arquivo binário */
    size_t elementos_escritos = fwrite(vetor_lbp, sizeof(double), tamanho, arquivo);

    /* Se a quantidade de elementos escritos não for igual ao tamanho esperado */
    if (elementos_escritos != tamanho) {
        /* Aqui você pode optar por retornar ou tomar outra ação */
        fclose(arquivo);
        return;  
    }

    /* Fecha o arquivo */
    fclose(arquivo);
}


/* Função para carregar o vetor LBP de um arquivo binário */
void carregar_vetor_lbp(const char *nome_arquivo, double *vetor_lbp, int tamanho)
{
    /* Validação de parâmetros */
    if (vetor_lbp == NULL || tamanho <= 0) {
        return;  /* Não faz nada se o vetor é nulo ou tamanho é inválido */
    }

    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        return;  /* Não faz nada se não for possível abrir o arquivo */
    }

    /* Lê o vetor LBP do arquivo binário */
    size_t elementos_lidos = fread(vetor_lbp, sizeof(double), tamanho, arquivo);

    /* Se a quantidade de elementos lidos não for igual ao tamanho esperado */
    if (elementos_lidos != tamanho) {
        fclose(arquivo);
        return;  /* Não faz nada em caso de erro na leitura */
    }

    /* Fecha o arquivo */
    fclose(arquivo);
}

/* Função para normalizar o histograma da imagem */
/* Esta função será usada para normalizar o histograma da imagem
   dividindo os pixels pelo total, corrigindo assim diferenças de escala */
void normalizar_histograma(struct imagemPGM *img, double *histograma_normalizado)
{
    if (img == NULL || histograma_normalizado == NULL) {
        /* Se a imagem ou o vetor de histograma for inválido, não faz nada */
        return;
    }

    int i, j;
    int **pixels = img->pixels;
    int largura = img->largura;
    int altura = img->altura;
    int histograma[256] = {0};  /* Inicializa o histograma com zeros */
    int total = 0;  /* Total de pixels processados */

    /* Calcula o histograma e contabiliza os pixels válidos, ignorando as bordas */
    for (i = 1; i < altura - 1; i++) {
        for (j = 1; j < largura - 1; j++) {
            histograma[pixels[i][j]]++;  /* Incrementa a contagem do valor de pixel */
            total++;
        }
    }

    /* Se não houver pixels válidos, não prossegue com a normalização */
    if (total == 0) {
        return;
    }

    /* Normaliza o histograma dividindo pela contagem total de pixels */
    for (i = 0; i < 256; i++) {
        histograma_normalizado[i] = (double)histograma[i] / total;
    }
}

/* Função para ignorar comentários em arquivos PGM */
/* Ignora linhas de comentários iniciadas com # nos arquivos PGM */
void ignora_comentarios(FILE *arquivo)
{
    int c = fgetc(arquivo);
    while (c == '#') {
        while (c != '\n') {
            c = fgetc(arquivo);
        }
        c = fgetc(arquivo);
    }
    ungetc(c, arquivo);
}

/* Função para ler uma imagem PGM (suporta P2 e P5) */
struct imagemPGM *ler_imagem(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        return NULL;  /* Retorna NULL se não for possível abrir o arquivo */
    }

    /* Lê o cabeçalho da imagem PGM */
    char tipo[3];
    int largura, altura, maximo;

    /* Lê o tipo da imagem */
    fscanf(arquivo, "%2s", tipo);
    ignora_comentarios(arquivo);

    /* Lê largura e altura */
    fscanf(arquivo, "%d", &largura);
    ignora_comentarios(arquivo);
    fscanf(arquivo, "%d", &altura);
    ignora_comentarios(arquivo);
    fscanf(arquivo, "%d", &maximo);
    fgetc(arquivo);  /* Lê o caractere de nova linha */

    /* Verifica se o tipo da imagem é P2 ou P5 */
    if (strcmp(tipo, "P2") != 0 && strcmp(tipo, "P5") != 0) {
        fclose(arquivo);
        return NULL;  /* Retorna NULL se o tipo da imagem não for P2 ou P5 */
    }

    /* Aloca memória para a estrutura da imagem */
    struct imagemPGM *img = (struct imagemPGM *)malloc(sizeof(struct imagemPGM));
    if (img == NULL) {
        fclose(arquivo);
        return NULL;  /* Retorna NULL se falhar na alocação de memória */
    }

    /* Preenche os campos da estrutura da imagem */
    img->largura = largura;
    img->altura = altura;
    img->maximo = maximo;

    /* Aloca memória para os ponteiros de linha da imagem */
    img->pixels = (int **)malloc(altura * sizeof(int *));
    if (img->pixels == NULL) {
        free(img);  /* Libera a memória alocada para a estrutura da imagem */
        fclose(arquivo);
        return NULL;  /* Retorna NULL se falhar na alocação de memória */
    }

    /* Aloca memória para cada linha da imagem */
    int i, j;
    for (i = 0; i < altura; i++) {
        img->pixels[i] = (int *)malloc(largura * sizeof(int));
        if (img->pixels[i] == NULL) {
            /* Se falhar em alocar para alguma linha */
            /* libera a memória já alocada */
            for (j = 0; j < i; j++) {
                free(img->pixels[j]); /* Libera linhas anteriores */
            }
            free(img->pixels);  /* Libera o array de ponteiros */
            free(img);          /* Libera a estrutura da imagem */
            fclose(arquivo);
            return NULL;  /* Retorna NULL se falhar na alocação de memória */
        }
    }

    /* Lê os pixels da imagem */
    if (strcmp(tipo, "P2") == 0) {
        /* Leitura para P2 (ASCII) */
        for (i = 0; i < altura; i++) {
            for (j = 0; j < largura; j++) {
                fscanf(arquivo, "%d", &img->pixels[i][j]);
            }
        }
    } else {
        /* Leitura para P5 (Binary) */
        fread(&img->pixels[0][0], sizeof(int), largura * altura, arquivo);
    }

    /* Fecha o arquivo */
    fclose(arquivo);
    
    return img;  /* Retorna a estrutura da imagem lida */
}

/* Função para liberar a memória da imagem PGM */
void liberar_imagem(struct imagemPGM *img)
{
    if (img == NULL) {
        return;  /* Não faz nada se a imagem for nula */
    }

    int i;
    /* Libera a memória de cada linha da imagem */
    if (img->pixels != NULL) {
        for (i = 0; i < img->altura; i++) {
            free(img->pixels[i]);  /* Libera cada linha */
        }
        free(img->pixels);  /* Libera o array de ponteiros de linha */
    }

    /* Libera a estrutura da imagem */
    free(img);
}

/* Função para escrever uma imagem PGM (mantém o formato original: P2 ou P5) */
/* As funções de leitura e escrita de imagens garantem que o formato da imagem
de saída será o mesmo da imagem de entrada, seja P2 ou P5.*/
void escrever_imagem(const char *nome_arquivo, struct imagemPGM *img, const char *tipo)
{
    /* Verifica se a imagem e o tipo são válidos */
    if (img == NULL || tipo == NULL) {
        return;  /* Não faz nada se a imagem ou tipo forem nulos */
    }

    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (arquivo == NULL) {
        return;  /* Não faz nada se não for possível abrir o arquivo */
    }

    /* Escreve o cabeçalho da imagem PGM */
    fprintf(arquivo, "%s\n", tipo);
    fprintf(arquivo, "%d %d\n", img->largura, img->altura);
    fprintf(arquivo, "%d\n", img->maximo);

    int i, j;

    /* Escreve os pixels da imagem */
    if (strcmp(tipo, "P2") == 0) {
        /* Escrita para P2 (ASCII) */
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < img->largura; j++) {
                fprintf(arquivo, "%d ", img->pixels[i][j]);
            }
            fprintf(arquivo, "\n");
        }
    } else if (strcmp(tipo, "P5") == 0) {
        /* Escrita para P5 (Binary) */
        for (i = 0; i < img->altura; i++) {
            fwrite(img->pixels[i], sizeof(unsigned char), img->largura, arquivo);
        }
    } else {
        fclose(arquivo);  /* Fecha o arquivo e não escreve se o tipo não for reconhecido */
        return;
    }

    /* Fecha o arquivo */
    fclose(arquivo);
}

/* Função para descartar bordas da imagem após o processo de convolução */
struct imagemPGM *descartar_bordas(struct imagemPGM *img)
{
    /* Verifica se a imagem é válida */
    if (img == NULL || img->pixels == NULL) {
        return NULL;  /* Retorna NULL se a imagem for inválida */
    }

    int i, j;
    int largura = img->largura;
    int altura = img->altura;

    /* Cria uma nova estrutura para a imagem sem as bordas */
    struct imagemPGM *img_sem_bordas = (struct imagemPGM *)malloc(sizeof(struct imagemPGM));
    if (img_sem_bordas == NULL) {
        return NULL;  /* Retorna NULL se falhar na alocação de memória */
    }

    img_sem_bordas->largura = largura - 2;  /* Desconta as bordas esquerda e direita */
    img_sem_bordas->altura = altura - 2;  /* Desconta as bordas superior e inferior */
    img_sem_bordas->maximo = img->maximo;  /* Mantém o valor máximo de cinza */

    /* Aloca memória para os ponteiros de linha da imagem sem as bordas */
    img_sem_bordas->pixels = (int **)malloc(img_sem_bordas->altura * sizeof(int *));
    if (img_sem_bordas->pixels == NULL) {
        free(img_sem_bordas);  /* Libera a memória alocada para a nova imagem */
        return NULL;  /* Retorna NULL se falhar na alocação de memória */
    }

    /* Aloca memória para cada linha da imagem sem as bordas */
    for (i = 0; i < img_sem_bordas->altura; i++) {
        img_sem_bordas->pixels[i] = (int *)malloc(img_sem_bordas->largura * sizeof(int));
        if (img_sem_bordas->pixels[i] == NULL) {
            /* Se falhar em alocar para alguma linha, libera a memória já alocada */
            for (j = 0; j < i; j++) {
                free(img_sem_bordas->pixels[j]);  /* Libera linhas anteriores */
            }
            free(img_sem_bordas->pixels);  /* Libera o array de ponteiros */
            free(img_sem_bordas);  /* Libera a estrutura da nova imagem */
            return NULL;  /* Retorna NULL se falhar na alocação de memória */
        }
    }

    /* Copia os pixels da imagem original para a nova imagem, descartando as bordas */
    for (i = 0; i < img_sem_bordas->altura; i++) {
        for (j = 0; j < img_sem_bordas->largura; j++) {
            img_sem_bordas->pixels[i][j] = img->pixels[i + 1][j + 1];  /* Descartando as bordas */
        }
    }

    return img_sem_bordas;  /* Retorna a nova imagem sem bordas */
}

/* Função para listar imagens no diretório (apenas arquivos .pgm) */

void listar_imagens(const char *diretorio, char ***imagens, int *contagem) {
    if (diretorio == NULL || imagens == NULL || contagem == NULL) {
        return;  /* Verifica se os parâmetros são válidos */
    }

    /* Cria o comando para listar arquivos no diretório */
    char comando[256];
    snprintf(comando, sizeof(comando), "ls -p %s | grep -v /", diretorio);

    /* Abre o pipe para executar o comando */
    FILE *fp = popen(comando, "r");
    if (fp == NULL) {
        perror("Erro ao executar o comando ls");
        return;
    }

    /* Conta quantos arquivos existem no diretório */
    char buffer[256];
    int total = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        total++;
    }

    /* Aloca memória para o array de strings */
    *imagens = (char **)malloc(total * sizeof(char *));
    if (*imagens == NULL) {
        pclose(fp);
        return;  /* Falha na alocação de memória */
    }

    /* Reabre o pipe para ler os nomes dos arquivos */
    pclose(fp);
    fp = popen(comando, "r");
    if (fp == NULL) {
        perror("Erro ao reabrir o comando ls");
        free(*imagens);
        return;
    }

    /* Lê os nomes dos arquivos e armazena no array */
    int i = 0;
    int j;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;  /* Remove o caractere de nova linha */
        (*imagens)[i] = (char *)malloc(strlen(buffer) + 1);
        if ((*imagens)[i] == NULL) {
            /* Em caso de falha, libera a memória alocada */
            for (j = 0; j < i; j++) {
                free((*imagens)[j]);
            }
            free(*imagens);
            pclose(fp);
            return;
        }
        strcpy((*imagens)[i], buffer);
        i++;
    }

    /* Fecha o pipe */
    pclose(fp);

    /* Atualiza a contagem de arquivos */
    *contagem = total;
}
