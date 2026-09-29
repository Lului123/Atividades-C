#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 10

typedef struct LIVRO{
    char nome[100];
    float preco;
}LIVRO;

int ordenar_por_nome(LIVRO *catalogo){
    LIVRO temp;

    for(int j = 0; j < TAM-1; j++){
        for(int i = 0; i < TAM-1-j; i++){
            if(strcmp(catalogo[i].nome, catalogo[i+1].nome) > 0){
                temp = catalogo[i];
                catalogo[i] = catalogo[i+1];
                catalogo[i+1] = temp;
            }
        }
    }
}

int main(){
// Criando e inicializando o catálogo com 10 livros
LIVRO catalogo[TAM] = {
    {"O Senhor dos Aneis", 59.90},
    {"Jogos Vorazes", 24.90},
    {"1984", 39.90},
    {"O Pequeno Principe", 19.90},
    {"O Alquimista", 34.90},
    {"Harry Potter e a Pedra Filosofal", 49.90},
    {"Os Alunos que Ordenavam Livros", 67.67},
    {"Vilao", 55.00},
    {"Orgulho e Preconceito", 29.90},
    {"Duna", 42.80}
};

ordenar_por_nome(catalogo);

for(int i = 0; i < TAM; i++){
    printf("\nlivro - %d ",i+1);
    printf("\nNome: %s",catalogo[i].nome);
    printf("\nPreço: %.2f\n\n",catalogo[i].preco);
}


}