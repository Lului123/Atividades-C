# Catálogo de Livros — Ordenação em C

Programa em C que mantém um catálogo de 10 livros e o ordena de quatro formas diferentes, usando o algoritmo **Bubble Sort** implementado manualmente (sem `qsort`).

## Link pra testar
[site](https://atividades-c.onrender.com/)

## Visão geral

| Opção | Critério | Função |
|:-----:|----------|--------|
| 1 | Nome, A → Z | `ordenar_por_nome_crescente` |
| 2 | Nome, Z → A | `ordenar_por_nome_decrescente` |
| 3 | Preço, menor → maior | `ordenar_por_preco_crescente` |
| 4 | Preço, maior → menor | `ordenar_por_preco_decrescente` |

A função `exibir` imprime o catálogo (posição, nome e preço de cada livro).

## Estrutura de dados

```c
#define TAM 10

typedef struct LIVRO{
    char nome[100];
    float preco;
}LIVRO;
```

O catálogo é um vetor de `TAM` structs `LIVRO`, inicializado com 10 títulos fixos. Todas as funções de ordenação recebem um ponteiro para o primeiro elemento do vetor (`LIVRO *catalogo`) e, por isso, **ordenam o vetor no próprio lugar**: não existe cópia nem vetor auxiliar.

## O algoritmo: Bubble Sort

A ideia é percorrer o vetor comparando pares de elementos vizinhos e trocando-os quando estão fora de ordem. A cada passada completa, o maior elemento (segundo o critério) "flutua" até o final da parte ainda não ordenada.

```c
void ordenar_por_preco_crescente(LIVRO* listaLivros){
    LIVRO aux;
    for(int iCont = 0; iCont < TAM - 1; iCont++){
        for(int jCont = 0; jCont < TAM - iCont - 1; jCont++){
            if(listaLivros[jCont].preco > listaLivros[jCont + 1].preco){
                aux = listaLivros[jCont];
                listaLivros[jCont] = listaLivros[jCont + 1];
                listaLivros[jCont + 1] = aux;
            }
        }
    }
}
```

### Como funcionam os laços

- **Laço externo** (`TAM - 1` passadas): após a passada `j`, os `j + 1` maiores elementos já estão em suas posições finais, no fim do vetor.
- **Laço interno** (`TAM - 1 - j` comparações): como o final do vetor já está ordenado, cada passada compara um par a menos que a anterior. Isso evita comparações inúteis e impede o acesso fora do vetor (`i + 1` nunca passa de `TAM - 1`).
- **Troca**: usa uma variável temporária do tipo `LIVRO`. Como é uma atribuição de struct, ela copia o nome inteiro (os 100 caracteres) e o preço de uma só vez.

### Exemplo: primeira passada (preço crescente)

Vetor inicial dos preços:

```
59.90  24.90  39.90  19.90  34.90  49.90  67.67  55.00  29.90  42.80
```

Na primeira passada (9 comparações), o 59.90 avança trocando com cada vizinho menor até encontrar o 67.67. Então o 67.67 assume o papel de "bolha" e vai até o final:

```
24.90  39.90  19.90  34.90  49.90  59.90  55.00  29.90  42.80  67.67
                                                                ^^^^^
                                                  maior já na posição final
```

As passadas seguintes repetem o processo sobre as 9, 8, 7... primeiras posições, até o vetor ficar ordenado.

## Ordenação por nome com `strcmp`

As duas funções de nome seguem exatamente o mesmo esquema, mas trocam o operador numérico por `strcmp` (da `<string.h>`):

```c
if(strcmp(catalogo[i].nome, catalogo[i+1].nome) > 0){   // crescente
if(strcmp(catalogo[i].nome, catalogo[i+1].nome) < 0){   // decrescente
```

`strcmp(a, b)` retorna:

- um valor **positivo** se `a` vem depois de `b`;
- um valor **negativo** se `a` vem antes de `b`;
- **zero** se as strings são iguais.

A comparação é **lexicográfica, byte a byte, pela tabela ASCII**. Consequências práticas:

- dígitos vêm antes de letras, então `"1984"` aparece primeiro na ordem crescente;
- letras maiúsculas vêm antes das minúsculas;
- acentos e `ç` não seguem a ordem alfabética do português, e é por isso que os títulos do catálogo foram escritos sem acentos (`"Vilao"`, `"Preco"`).

## Crescente × decrescente

A única diferença entre as versões crescente e decrescente é o **sentido da comparação** (`>` ou `<`, positivo ou negativo no `strcmp`). A estrutura dos laços e a troca são idênticas.

## Propriedades do algoritmo

| Propriedade | Valor |
|-------------|-------|
| Comparações | Sempre `TAM·(TAM−1)/2` = **45** para 10 livros (não há saída antecipada) |
| Trocas | De 0 (já ordenado) até 45 (ordem inversa) |
| Complexidade de tempo | O(n²) |
| Memória extra | O(1), só uma variável temporária |
| Estável | Sim: a comparação é estrita (`>` / `<`), então elementos iguais nunca trocam de lugar |

## Comportamento acumulativo

Como as funções alteram o vetor original, **ordenar duas vezes parte do resultado anterior**. Por exemplo, ordenar por nome e depois por preço crescente deixa o catálogo ordenado por preço; livros com o mesmo preço mantêm a ordem alfabética obtida antes, graças à estabilidade do algoritmo.

## Possíveis melhorias

- **Parar cedo**: usar uma flag que indica se houve troca na passada e encerrar quando nenhuma ocorrer, o que deixa o melhor caso em O(n).
- **Evitar a repetição de código**: as quatro funções diferem só na condição de troca. Uma única função recebendo um ponteiro para função comparadora (como faz o `qsort`) resolveria isso.
- **Ordenação por nome mais amigável**: tratar maiúsculas/minúsculas e acentos, usando `strcasecmp` ou `strcoll` com a localidade configurada.
- **Trocar ponteiros em vez de structs**: ordenar um vetor de ponteiros evita copiar 104 bytes a cada troca.