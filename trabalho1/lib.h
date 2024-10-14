/* Autora: Juliana Zambon (jz22), GRR20224168 */

#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <stdlib.h>

/* Estruturas para armazenar dados da imagem e do histograma LBP */

/* Estrutura para armazenar a imagem PGM */
struct imagemPGM
{
    int largura;            /* Largura da imagem em pixels */
    int altura;             /* Altura da imagem em pixels */
    int maximo;             /* Valor máximo de cinza (branco) */
    unsigned char **pixels; /* Matriz de pixels (valores de 0 a 255) */
};

/* Estrutura para armazenar o histograma LBP */
struct LBPHistograma
{
    int histograma[256]; /* Frequência de cada padrão LBP */
    double tamanho;      /* Total de padrões LBP computados */
};

/*--------------------------------------------------------------------*/
/* Funções para alocação de memória */

/* Aloca memória para a imagem PGM */
struct imagemPGM *alocar_imagem(int largura, int altura);

/* Aloca memória para o histograma LBP */
struct LBPHistograma *alocar_histograma();

/* Aloca memória para os pixels da imagem */
unsigned char **alocar_pixels(int largura, int altura);

/*--------------------------------------------------------------------*/
/* Funções para manipulação de pixels */

/* Ignorar Linhas de Comentário: garantir que os comentários
(linhas que começam com #) sejam ignorados ao ler arquivos P2 e P5*/
void ignorar_comentarios(FILE *arquivo);

/* Preenche a matriz de pixels para o formato P2 */
struct imagemPGM *preencher_pixels_P2(FILE *arquivo, struct imagemPGM *img);

/* Preenche a matriz de pixels para o formato P5 */
struct imagemPGM *preencher_pixels_P5(FILE *arquivo, struct imagemPGM *img);

/* Função para ler a imagem PGM */
struct imagemPGM *ler_imagem(FILE *arquivo, struct imagemPGM *img, char *tipo);

/*--------------------------------------------------------------------*/
/* Funções para processamento LBP */

/* Inicializa uma nova imagem para armazenar o resultado do LBP */
void nova_imagem_inicializa(struct imagemPGM *img, struct imagemPGM *nova);

/* Calcula o LBP para um pixel específico da imagem */
/* Remover Bordas Durante a Convolução: garantir que as bordas da imagem original não sejam processadas.
Assegurando que o cálculo comece após as bordas (por exemplo, começando de 1 até altura - 1 e largura - 1).*/
void calcula_lbp(struct imagemPGM *img, struct imagemPGM *nova, int i, int j);

/* Gera a imagem LBP a partir da imagem original */
void gerar_lbp(struct imagemPGM *img, struct imagemPGM *nova);

/*--------------------------------------------------------------------*/
/* Funções de saída */

/* Gera a imagem de saída no formato PGM */
/* Formato de Saída: capaz de determinar o formato da imagem de entrada
e gerar a saída correspondente no mesmo formato (P2 ou P5)*/
void gerar_imagem_saida(struct imagemPGM *nova, FILE *arquivo_saida);

/* Funções para histogramas */

/* Define o histograma LBP a partir da imagem */
/* Normalização do Histograma: garantindo que a contagem de pixels de cada tom seja dividida
pelo total de pixels na imagem.*/
void definir_histograma(char *arquivo_entrada, struct imagemPGM *img, struct LBPHistograma *histograma);

/* Calcula a distância euclidiana entre histogramas LBP */
void distancia_euclidiana_histLBP(struct LBPHistograma *aux, struct LBPHistograma *lbp_origem, struct LBPHistograma *lbp_comparar);

/* Lê a estrutura LBP a partir de um arquivo */
struct LBPHistograma *ler_estrutura_lbp(FILE *arquivo_lbp, struct LBPHistograma *histograma);

/* Calcula a distância euclidiana entre histogramas de um diretório */
void distancia_euclidiana_dir(char *nome_diretorio, struct LBPHistograma *histograma, double *distancia, char menor_distancia[256]);

/*--------------------------------------------------------------------*/
/* Funções de leitura de diretório */

/* Lê todos os arquivos de um diretório */
void ler_diretorio(char *nome_diretorio);

/* Converte a imagem LBP a partir de um arquivo de entrada */
void converter_lbp(char arquivo_entrada[256]);

/* Função para liberar a memória alocada para a imagem */
void liberar_memoria(struct imagemPGM *img);

#endif /* LIB_H */

