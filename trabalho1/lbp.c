#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <getopt.h>
#include "lib.h"

int main(int argc, char *argv[])
{
    char *input_file = NULL;          /* Arquivo de entrada */
    char *reference_dir = NULL;       /* Diretório de referência */
    char *output_file = NULL;         /* Arquivo de saída */
    int largura, altura;              /* Dimensões da imagem */
    unsigned char **lbp_input = NULL; /* Matriz LBP da imagem de entrada */
    double menor_distancia = -1.0;    /* Variável para armazenar a menor distância */
    char *imagem_mais_similar = NULL; /* Para armazenar a imagem mais similar */
    char **imagens_ref = NULL;        /* Array para armazenar nomes de imagens de referência */
    int contagem_ref = 0;             /* Contagem de imagens de referência */
    int i, j, k;                      /* Variáveis de controle para loops */

    /* Análise dos argumentos da linha de comando */
    int opt;
    /* Parsear argumentos de linha de comando (use getopt) */
    while ((opt = getopt(argc, argv, "d:i:o:")) != -1)
    {
        switch (opt)
        {
        case 'd':
            reference_dir = optarg;
            break;
        case 'i':
            input_file = optarg;
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            fprintf(stderr, "Forma de uso: ./LBP -d ./base -i input_file.pgm -o output_file.pgm\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Verificar se a imagem de entrada foi fornecida */
    if (input_file == NULL)
    {
        fprintf(stderr, "Erro: imagem de teste não especificada.\n");
        return EXIT_FAILURE;
    }

    /* Ler a imagem de entrada */
    struct imagemPGM *img = ler_imagem(input_file);
    if (img == NULL)
    {
        fprintf(stderr, "Erro: não foi possível ler a imagem %s\n", input_file);
        return EXIT_FAILURE;
    }

    largura = img->largura;
    altura = img->altura;

    /* Alocar memória para a matriz LBP da imagem de entrada */
    lbp_input = (unsigned char **)malloc(altura * sizeof(unsigned char *));
    for (i = 0; i < altura; i++)
    {
        lbp_input[i] = (unsigned char *)malloc(largura * sizeof(unsigned char));
    }

    /* Calcular o LBP da imagem de teste */
    lbp(img, lbp_input);

    /* Listar imagens no diretório de referência */
    listar_imagens(reference_dir, &imagens_ref, &contagem_ref);

    for (i = 0; i < contagem_ref; i++)
    {
        char caminho_lbp[256];
        sprintf(caminho_lbp, "%s/%s.lbp", reference_dir, imagens_ref[i]); 

        /* Verificar se o arquivo .lbp existe */
        FILE *arquivo_lbp = fopen(caminho_lbp, "rb");
        double *vetor_lbp_ref = (double *)malloc(img->largura * img->altura * sizeof(double));

        if (arquivo_lbp)
        {
            /* Carregar vetor LBP do arquivo binário */
            carregar_vetor_lbp(caminho_lbp, vetor_lbp_ref, img->largura * img->altura);
            fclose(arquivo_lbp);
        }
        else
        {
            /* Se não existir, calcular LBP da imagem de referência e salvar o vetor LBP */
            char caminho_imagem[256];
            sprintf(caminho_imagem, "%s/%s", reference_dir, imagens_ref[i]); 
            struct imagemPGM *img_ref = ler_imagem(caminho_imagem);
            if (!img_ref)
            {
                fprintf(stderr, "Erro ao ler imagem de referência %s.\n", imagens_ref[i]);
                continue;
            }

            unsigned char **lbp_ref = (unsigned char **)malloc(img_ref->altura * sizeof(unsigned char *));
            for (j = 0; j < img_ref->altura; j++)
            {
                lbp_ref[j] = (unsigned char *)malloc(img_ref->largura * sizeof(unsigned char));
            }

            lbp(img_ref, lbp_ref); /* Calcular LBP da imagem de referência */
            extrair_vetor_caracteristicas(img_ref, vetor_lbp_ref);
            gravar_vetor_lbp(caminho_lbp, vetor_lbp_ref, img_ref->largura * img_ref->altura);

            /* Liberar memória da imagem de referência */
            liberar_imagem(img_ref);
            for (j = 0; j < img_ref->altura; j++)
            {
                free(lbp_ref[j]);
            }
            free(lbp_ref);
        }

        /* Calcular distância Euclidiana */
        double *lbp_input_double = (double *)malloc(img->largura * img->altura * sizeof(double));
        for (j = 0; j < altura; j++)
        {
            for (k = 0; k < largura; k++)
            {
                lbp_input_double[j * largura + k] = lbp_input[j][k]; /* Converter para double */
            }
        }

        double distancia = distancias_euclidianas(vetor_lbp_ref, lbp_input_double, img->largura * img->altura);

        if (menor_distancia < 0 || distancia < menor_distancia)
        {
            menor_distancia = distancia;
            imagem_mais_similar = imagens_ref[i];
        }

        free(lbp_input_double);
        free(vetor_lbp_ref);
    }

    /* Resultado da comparação */
    if (imagem_mais_similar)
    {
        printf("Imagem mais similar: %s %.6f\n", imagem_mais_similar, menor_distancia);
    }

    /* Gerar a imagem LBP de saída*/
    if (output_file)
    {
        gerar_imagem_lbp(img, (int **)lbp_input, output_file);
    }

    /* Liberar memória */
    liberar_imagem(img);
    for (i = 0; i < altura; i++)
    {
        free(lbp_input[i]);
    }
    free(lbp_input);

    for (i = 0; i < contagem_ref; i++)
    {
        free(imagens_ref[i]);
    }
    free(imagens_ref);

    return 0;
}
