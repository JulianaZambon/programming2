# Programação II (CI1002) - Trabalho Prático 01: Estratégias de Comparação de Imagens

**Autora:** Juliana Zambon (jz22), GRR20224168

Este repositório contém o código desenvolvido para o Trabalho Prático 01 da disciplina **CI1002 - Programação II**,
oferecida pelo Departamento de Informática da Universidade Federal do Paraná (UFPR).

## Descrição

O objetivo deste trabalho é implementar um programa em C capaz de comparar uma imagem de teste com um conjunto de
imagens de referência e identificar a imagem mais similar utilizando a **Distância Euclidiana**.
Além disso, o programa gera uma imagem baseada na técnica **Local Binary Pattern (LBP)**.

## Funcionalidades

- **Comparação de Imagens:** O programa realiza a comparação entre a imagem de teste e todas as imagens da base de referência, identificando e retornando a imagem mais similar com base na menor distância Euclidiana. Para isso, utilizamos o histograma LBP (Local Binary Pattern), que é uma representação eficiente para descrever a textura de uma imagem. Como os histogramas podem ser representados por vetores, a comparação entre duas imagens se dá através do cálculo da distância entre esses vetores, sendo a distância Euclidiana uma das abordagens mais utilizadas pela sua simplicidade e precisão.

- **Geração de Imagem LBP:** Além da comparação, o programa também gera a imagem LBP correspondente à imagem de teste. A transformação LBP é aplicada pixel a pixel, destacando padrões locais de textura, o que facilita a análise e a comparação entre diferentes imagens.


## Estrutura

O código-fonte está estruturado em arquivos `.c` e `.h` que agrupam as funcionalidades, sendo `lbp.c` o arquivo principal de aplicação, `lib.c` o arquivo de implementação, e `lib.h` o arquivo de cabeçalho.

## Arquivo Cabeçalho (lib.h)

O arquivo `lib.h` contém as estruturas e protótipos de funções necessárias para o processamento de imagens no formato PGM e para o cálculo do histograma LBP. Ele define as funções responsáveis por ler, manipular e comparar as imagens, bem como gerar o vetor LBP e realizar as operações relacionadas à distância Euclidiana.

### Estruturas

- **`struct imagemPGM`**: Armazena as informações básicas de uma imagem PGM, incluindo largura, altura, valor máximo de cinza e a matriz de pixels.
  
  ```c
  struct imagemPGM {
      int largura;
      int altura;
      int maximo;
      unsigned char **pixels;
      char tipo[4];
  };

- **`struct LBPHistograma`**: Armazena o histograma LBP, que contém a frequência de padrões LBP e o tamanho total da imagem.

    ```c
    struct LBPHistograma {
      int histograma[256];
      double tamanho;
    };


### Funções Principais
Manipulação de Imagens PGM

- **`struct imagemPGM *alocar_imagem()`**: Aloca memória para uma estrutura de imagem PGM.
- **`void alocar_pixels(struct imagemPGM *img)`**: Aloca a matriz de pixels de acordo com a largura e altura da imagem.
- **`void ignorar_comentarios(FILE *arquivo)`**: Ignora as linhas de comentário (começam com #) ao ler imagens PGM.
- **`struct imagemPGM *ler_imagem(FILE *arquivo, struct imagemPGM *img)`**: Lê uma imagem PGM (formatos P2 ou P5) de um arquivo.
- **`void liberar_imagem(struct imagemPGM *img)`**: Libera a memória alocada para uma imagem PGM.

Geração e Manipulação de LBP

- **`void gerar_lbp(struct imagemPGM *img, struct imagemPGM *nova)`**: Gera uma imagem LBP a partir da imagem original, removendo as bordas durante a convolução.
- **`void gerar_histograma(struct imagemPGM *nova, struct LBPHistograma *lbp_hist, char *arquivo_entrada)`**: Gera o histograma LBP de uma imagem e normaliza o resultado.
- **`double distancia_euclidiana(struct LBPHistograma *hist1, struct LBPHistograma *hist2)`**: Calcula a distância euclidiana entre dois histogramas LBP.

Comparação de Imagens

- **`void encontrar_imagem_similar(char *diretorio, struct LBPHistograma *histograma_teste, double *distancia, char menor_distancia[256])`**: Compara uma imagem de teste com todas as imagens da base de referência, exibindo a imagem mais similar e a distância euclidiana.
- **`void ler_diretorio(char *nome_diretorio)`**: Lê o conteúdo de um diretório e retorna uma lista de arquivos de imagem PGM.

Formato de Imagens Suportado

A biblioteca trabalha com imagens PGM nos formatos P2 (ASCII) e P5 (binário). A função **`gerar_imagem_saida`** permite gerar a imagem de saída no mesmo formato da imagem de entrada.
