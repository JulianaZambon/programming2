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
            exit(EXIT_FAILURE);
        }
    }

    if (diretorio == NULL || arquivo_entrada == NULL || arquivo_saida == NULL)
    {
        fprintf(stderr, "Uso: %s -d <diretório> -i <arquivo_entrada> -o <arquivo_saida>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Lê o arquivo de entrada */
    FILE *arquivo = fopen(arquivo_entrada, "r"); /* r = read */
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
        exit(EXIT_FAILURE);
    }

    /* Lê o cabeçalho da imagem 
    char tipo[2];
    fscanf(arquivo, "%s", tipo);

    /* Lê a largura e a altura da imagem 
    int largura, altura;
    fscanf(arquivo, "%d %d", &largura, &altura);

    /* Lê o valor máximo de cinza 
    int maximo;
    fscanf(arquivo, "%d", &maximo);

    /* Aloca memória para a imagem */
    struct imagemPGM *img = alocar_imagem();

    /* Lê a imagem */
    img = ler_imagem(arquivo, img, arquivo_entrada);

    /* Comparar uma imagem de teste com todas as imagens da base de referência */
    /*  A saída deve ser EXATAMENTE a seguinte
        Imagem mais similar: <img mais similar> <distância>
    */

    /* Exibir a imagem mais similar e a distância */
    /* printf("Imagem mais similar: %s %.6f\n", img_similar, distancia); */

    /*
        em que <img mais similar> é a imagem com a menor distância Euclidiana encontrada
        no diretório ./base (informar apenas o nome do arquivo com sua extensão) e
        <distância> é o valor da distância associada a esta imagem, um número com
        EXATAMENTE 6 casas decimais. Nome e valor devem ser separados APENAS por
        um espaço.
    */

    /* Gerar a imagem LBP */
    /*
     Exemplo:
     ./lbp -i img1.tif -o img_out.tif
     Nesse caso, o programa recebe como entrar imagem img1.tif e gerar a imagem LPB
     com o nome informado pela opção -o. NENHUMA saída é esperada no terminal; caso
     algum erro ocorra, basta não criar a imagem de saída.
    */
}

