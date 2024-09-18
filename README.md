# Programação 02
Uso de técnicas avançadas para desenvolvimento de software. Práticas de projetos de desenvolvimento de programas de média e alta complexidade.

## Trabalho 01
Estratégias de Comparação de Imagens

O que deve ser implementado:
Você deve escrever um programa em C, chamado lbp que implemente as seguintes funcionalidades:
### 1) Comparar uma imagem de teste com todas as imagens da base de referência
O cálculo do LBP é a parte mais custosa do processo. Em função disso, você deverá realizar esse cálculo uma única vez para todas as imagens da base de referência. O vetor LBP deve ser armazenado em um arquivo binário que deve ter o mesmo nome da imagem, mas com extensão .lbp. Por exemplo, para a imagem img1.tif, o arquivo deverá ser img1.lbp.
Toda vez que o programa for executado, você deverá procurar pelo arquivo binário para calcular a distância com a imagem de teste. Caso ele não exista, você deve ler a
imagem, calcular o LBP e gravar o vetor LBP no arquivo binário.

### 2) Gerar a imagem LBP 
Nesse caso, o programa recebe como entrar imagem img1.tif e gerar a imagem LPB com o nome informado pela opção -o. NENHUMA saída é esperada no terminal; caso algum erro ocorra, basta não criar a imagem de saída.
