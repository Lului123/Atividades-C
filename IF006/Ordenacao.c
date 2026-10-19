#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Camada de comunicacao com a interface (HTTP local) ---- */
#ifdef _WIN32
    #include <winsock2.h>
    typedef SOCKET sock_t;
    #define FECHAR closesocket
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    typedef int sock_t;
    #define FECHAR close
#endif

#define PORTA 8080
#define TAM 10

typedef struct LIVRO{
    char nome[100];
    float preco;
}LIVRO;

/* ================= LOGICA ORIGINAL (sem alteracoes) ================= */

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
        printf("\nPreco: %.2f\n\n",catalogo[i].preco);
    }
}

/* ================= COMUNICACAO COM A INTERFACE ================= */

/* Converte o catalogo em JSON: [{"nome":"...","preco":59.90}, ...] */
static int catalogo_json(LIVRO *catalogo, char *out, size_t max){
    int n = 0;
    n += snprintf(out + n, max - n, "[");
    for(int i = 0; i < TAM; i++){
        n += snprintf(out + n, max - n, "%s{\"nome\":\"%s\",\"preco\":%.2f}",
                      i ? "," : "", catalogo[i].nome, catalogo[i].preco);
    }
    n += snprintf(out + n, max - n, "]");
    return n;
}

static void responder(sock_t c, const char *status, const char *tipo,
                      const char *corpo, int tam){
    char cab[256];
    int n = snprintf(cab, sizeof cab,
        "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n"
        "Cache-Control: no-store\r\nConnection: close\r\n\r\n", status, tipo, tam);
    send(c, cab, n, 0);
    if(tam > 0) send(c, corpo, tam, 0);
}

static void enviar_interface(sock_t c){
    FILE *f = fopen("interface.html", "rb");
    if(!f){
        const char *msg = "interface.html nao encontrado (deixe na mesma pasta do executavel)";
        responder(c, "404 Not Found", "text/plain; charset=utf-8", msg, (int)strlen(msg));
        return;
    }
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);
    char *buf = malloc(tam);
    fread(buf, 1, tam, f);
    fclose(f);
    responder(c, "200 OK", "text/html; charset=utf-8", buf, (int)tam);
    free(buf);
}

int main(){
    // Criando e inicializando o catálogo com 10 livros
    LIVRO original[TAM] = {
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
    LIVRO catalogo[TAM];
    memcpy(catalogo, original, sizeof catalogo);

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    sock_t srv = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof opt);

    struct sockaddr_in end;
    memset(&end, 0, sizeof end);
    end.sin_family = AF_INET;
    end.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   /* somente esta maquina */
    end.sin_port = htons(PORTA);

    if(bind(srv, (struct sockaddr*)&end, sizeof end) < 0 || listen(srv, 5) < 0){
        printf("Erro ao abrir a porta %d (ja esta em uso?)\n", PORTA);
        return 1;
    }
    printf("\n/// SERVIDOR ATIVO ///\nAbra no navegador: http://localhost:%d\n", PORTA);
    printf("Ctrl+C para encerrar.\n");

    char req[2048], json[4096];
    while(1){
        sock_t c = accept(srv, NULL, NULL);
        if(c < 0) continue;
        int r = recv(c, req, sizeof req - 1, 0);
        if(r <= 0){ FECHAR(c); continue; }
        req[r] = '\0';

        char metodo[8], rota[256];
        if(sscanf(req, "%7s %255s", metodo, rota) != 2){ FECHAR(c); continue; }

        if(strncmp(rota, "/api/catalogo", 13) == 0){
            int n = catalogo_json(catalogo, json, sizeof json);
            responder(c, "200 OK", "application/json", json, n);

        } else if(strncmp(rota, "/api/ordenar?op=", 16) == 0){
            int opcao = atoi(rota + 16);   /* mesmas opcoes do menu original */
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
                    printf("\n/// ORDENACAO POR PRECO CRESCENTE ///\n");
                    ordenar_por_preco_crescente(catalogo);
                    exibir(catalogo);
                    break;
                case 4:
                    printf("\n/// ORDENACAO POR PRECO DECRESCENTE ///\n");
                    ordenar_por_preco_decrescente(catalogo);
                    exibir(catalogo);
                    break;
                default:
                    break;   /* opcao invalida: devolve o catalogo como esta */
            }
            int n = catalogo_json(catalogo, json, sizeof json);
            responder(c, "200 OK", "application/json", json, n);

        } else if(strncmp(rota, "/api/reset", 10) == 0){
            memcpy(catalogo, original, sizeof catalogo);
            int n = catalogo_json(catalogo, json, sizeof json);
            responder(c, "200 OK", "application/json", json, n);

        } else if(strcmp(rota, "/") == 0 || strncmp(rota, "/index", 6) == 0){
            enviar_interface(c);

        } else {
            responder(c, "404 Not Found", "text/plain", "nao encontrado", 14);
        }
        FECHAR(c);
    }
    return 0;
}