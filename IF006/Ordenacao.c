#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 10

typedef struct LIVRO{
    char nome[100];
    float preco;
}LIVRO;

void ordenar_por_nome_crescente(LIVRO *catalogo){
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

void ordenar_por_nome_decrescente(LIVRO *catalogo){
    LIVRO temp;

    for(int j = 0; j < TAM-1; j++){
        for(int i = 0; i < TAM-1-j; i++){
            if(strcmp(catalogo[i].nome, catalogo[i+1].nome) < 0){
                temp = catalogo[i];
                catalogo[i] = catalogo[i+1];
                catalogo[i+1] = temp;
            }
        }
    }
}

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

void ordenar_por_preco_decrescente(LIVRO* listaLivros){
    LIVRO aux;
    for(int iCont = 0; iCont < TAM - 1; iCont++){
        for(int jCont = 0; jCont < TAM - iCont - 1; jCont++){
            if(listaLivros[jCont].preco < listaLivros[jCont + 1].preco){
            aux = listaLivros[jCont];
            listaLivros[jCont] = listaLivros[jCont + 1];
            listaLivros[jCont + 1] = aux;
            }
        }
    }
}

void exibir(LIVRO *catalogo){
    for(int i = 0; i < TAM; i++){
        printf("\nlivro - %d ",i+1);
        printf("\nNome: %s",catalogo[i].nome);
        printf("\nPreço: %.2f\n\n",catalogo[i].preco);
    }
}

void menu(){
    printf("\n/// MENU ///\n");
    printf("\n1 - Ordenar por nome (crescente)");
    printf("\n2 - Ordenar por nome (decrescente)");
    printf("\n3 - Ordenar por preço (crescente)");
    printf("\n4 - Ordenar por preço (decrescente)");
    printf("\n0- Encerrar programa\n");
    
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

    int opcao;
    do
        {int invalido;

        do{
            invalido = 0;

            menu();
            printf("\nDigite uma opcao: ");

            scanf("%d",&opcao);

            if(opcao < 0 || opcao > 4)
                invalido = 1;
            
        }while(invalido);

        switch (opcao){
            case 1:
                printf("\n/// ORDENACAO POR NOME CRESCENTE ///\n");
                ordenar_por_nome_crescente(catalogo);
                exibir(catalogo);
                break;

            case 2:
                printf("\n/// ORDENACAO POR NOME DECRESCENTE ///\n");
                ordenar_por_nome_decrescente(catalogo);
                exibir(catalogo);
                break;

            case 3:
                printf("\n/// ORDENACAO POR PREÇO CRESCENTE ///\n");
                ordenar_por_preco_crescente(catalogo);
                exibir(catalogo);
                break;

            case 4:
                printf("\n/// ORDENACAO POR PREÇO DECRESCENTE ///\n");
                ordenar_por_preco_decrescente(catalogo);
                exibir(catalogo);
                break;
            case 0: 
                printf("\n/// PROGRAMA ENCERRADO ///\n");
                return 0;
        }
    }while(opcao != 0);
}