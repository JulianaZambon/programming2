#ifndef LIB_H
#define LIB_H

struct imagemPGM {
    int largura;
    int altura;
    int maximo; /* valor maximo de cinza */
    int **pixels;
};

/* Função para calcular o LBP */
void lbp(struct imagemPGM *img, int **lbp);

/* Função para calcular a distância Euclidiana */
double distancias_euclidianas(double *v1, double *v2, int n);

/* Função para ignorar comentários */
void ignora_comentarios(FILE *arquivo);

/* Função para ler a imagem */
struct imagemPGM *ler_imagem(const char *nome_arquivo);

/* Função para liberar a imagem */
void liberar_imagem(struct imagemPGM *img);

/* Função para escrever uma imagem PGM */
void escrever_imagem(const char *nome_arquivo, struct imagemPGM *img, const char *tipo);

#endif


