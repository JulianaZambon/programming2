#ifndef LIB_H
#define LIB_H

struct imagemPGM {
    int largura;
    int altura;
    int maximo; /* valor máximo de cinza */
    int **pixels;
};

/* Função para calcular o LBP */
void lbp(struct imagemPGM *img, int **lbp);

/* Função para extrair um vetor de características que sumarize o conteúdo da imagem */
void extrair_vetor_caracteristicas(struct imagemPGM *img, double *vetor);

/* Função para calcular a distância Euclidiana */
double distancias_euclidianas(double *v1, double *v2, int n);

/* Função para gerar a imagem LBP */
void gerar_imagem_lbp(struct imagemPGM *img, int **lbp);

/* Função para gravar o vetor LBP em um arquivo binário */
void gravar_vetor_lbp(char *nome_arquivo, int **lbp, int largura, int altura);

/* Função para carregar o vetor LBP de um arquivo binário */
void carregar_vetor_lbp(char *nome_arquivo, double *vetor_lbp, int largura, int altura);

/* Função para normalizar o histograma da imagem */
/* Esta função será usada para normalizar o histograma da imagem
dividindo os pixels pelo total, corrigindo assim diferenças de escala*/
void normalizar_histograma(struct imagemPGM *img, double *histograma_normalizado);

/* Função para ignorar comentários em arquivos PGM */
/* Ignora linhas de comentários iniciadas com # nos arquivos PGM */
void ignora_comentarios(FILE *arquivo);

/* Função para ler uma imagem PGM (suporta P2 e P5) */
struct imagemPGM *ler_imagem(const char *nome_arquivo);

/* Função para liberar a memória da imagem */
void liberar_imagem(struct imagemPGM *img);

/* Função para escrever uma imagem PGM (mantém o formato original: P2 ou P5) */
/* As funções de leitura e escrita de imagens garantem que o formato da imagem
de saída será o mesmo da imagem de entrada, seja P2 ou P5.*/
void escrever_imagem(const char *nome_arquivo, struct imagemPGM *img, const char *tipo);

/* Função para descartar bordas da imagem após o processo de convolução */
struct imagemPGM *descartar_bordas(struct imagemPGM *img);

#endif


