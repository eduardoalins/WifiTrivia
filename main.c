#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define TAMANHO 1000

typedef struct node {
  char nome[TAMANHO];
  char acontecimento[TAMANHO];
  int ID;
  int ano;
  struct node *prox;
  struct node *ant;
  int errado;
} carta;

int x = 34, y = 12;
int incX = 1, incY = 1;
// Tela (X: 3 - 72 Y: 1 - 48)
// ➣ ♥

void inserirLista(carta **head, char nome[TAMANHO], int ano, int cont,
                  char acontecimento[TAMANHO], int cor);
void printListaTela(carta *head);
//void ordenarLista(carta **head);
int calcularTamanhoLista(carta *head);
void limparLista(carta **head);
void apagarListaTela(carta *head);
void colocarID(carta **head);
void printSeta(int qtdTermos, int x, int y);
void apagarSeta(int x, int y);
carta *escolherCarta(carta *head);
carta *conversaoLinha(char *linha);
//void trocarCelula(carta **atualC, carta **proxC);
int jogo(carta *head);
void menu();
void comoJogar();
void atualizarHead(carta **head);
void printNovaCarta(carta carta);
void printVida(int quantidade);
void bubblesort(carta **head);
void trocarCelula2(carta **atualC, carta **proxC);
void verificarErro(carta **head, int ano);
int contarVidas(carta *head);

int main() {
  screenSetColor(YELLOW, BLACK);
  srand(time(NULL));
  carta *head = NULL;
  menu();

  carta *primeiraCarta = escolherCarta(head);
  inserirLista(&head, primeiraCarta->nome, primeiraCarta->ano, 0,
               primeiraCarta->acontecimento, 0);

  while (1) {
    timerInit(0);
    screenGotoxy(4, 19);
    if (keyhit()) {
      char key = readch();
      if (key == '1') {
        int jogoValidacao = 1;
        while (jogoValidacao == 1) {
          jogoValidacao = jogo(head);
        }
        if(jogoValidacao == 0){
          screenDestroy();
          printf("Game Over");
        }
      } else if (key == '2') {
        comoJogar();

      } else if (key == '3') {
        // mostra o ranking
      } else if (key == '4') {
        // mostra os créditos
      } else if (key == '5') {
        break;
      } else {
        printf("");
      }
    }
  }

  screenDestroy();

  return 0;
}

void inserirLista(carta **head, char nome[TAMANHO], int ano, int cont,
                  char acontecimento[TAMANHO],
                  int cor) { // 0 coloca antes da head e
                             // 1 antes do primeiro termo
  carta *novo = (carta *)malloc(sizeof(carta));
  if (novo != NULL) {
    strcpy(novo->nome, nome);
    novo->ano = ano;
    strcpy(novo->acontecimento, acontecimento);
    novo->ant = NULL;
    novo->prox = NULL;
    novo->errado = cor;

    if (*head == NULL) {
      *head = novo;
    } else {
      carta *n = *head;
      int i = 0;
      for (i = 0; i < cont; i++) {
        n = n->prox;
      }
      if (n == (*head)) {
        novo->prox = n;
        n->ant = novo;
        (*head) = novo;
      } else if (n == NULL) {
        n = *head;
        while (n->prox != NULL) {
          n = n->prox;
        }
        n->prox = novo;
        novo->ant = n;
      } else {
        novo->ant = n->ant;
        n->ant->prox = novo;
        novo->prox = n;
        n->ant = novo;
      }
    }
  }
}

carta *escolherCarta(carta *head) {
  carta *novo = malloc(sizeof(carta));
  FILE *file = fopen("bibliotecaWiki.txt", "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
  }

  int qtdLinhas = 0;
  char caractere;
  while ((caractere = fgetc(file)) != EOF) {
    if (caractere == '\n') {
      qtdLinhas++;
    }
  }

  srand(time(NULL));
  int linhaEscolhida = rand() % qtdLinhas;
  int linhaAtual = 0;
  char linha[TAMANHO];

  rewind(file);

  while (fgets(linha, TAMANHO, file)) {
    if (linhaAtual == linhaEscolhida) {
      novo = conversaoLinha(linha);
      break;
    }
    linhaAtual++;
  }

  fclose(file);
  return novo;
}

carta *conversaoLinha(char *linha) {
  carta *novo = malloc(sizeof(carta));

  char nome[TAMANHO];
  int i = 0, j = 0;

  while (linha[i] != '/' && linha[i] != '\0') {
    nome[j++] = linha[i++];
  }
  nome[j] = '\0';
  strcpy(novo->nome, nome);

  i++;
  char ano[TAMANHO];
  j = 0;

  while (linha[i] != '/' && linha[i] != '\0') {
    ano[j++] = linha[i++];
  }
  ano[j] = '\0';
  novo->ano = atoi(ano);

  i++;
  strcpy(novo->acontecimento, &linha[i]);

  return novo;
}

void printListaTela(carta *head) {
  screenInit(1);
  screenSetColor(YELLOW, BLACK);

  int posicaoX = 4;
  int posicaoY = 5;

  screenGotoxy(posicaoX, posicaoY);

  if (head != NULL) {
    while (head != NULL) {
      if (head->errado == 1) {
        screenSetColor(RED, BLACK);
        printf("%s --> %d", head->nome, head->ano);
        screenSetColor(YELLOW, BLACK);
      } else {
        screenSetColor(YELLOW, BLACK);
        printf("%s --> %d", head->nome, head->ano);
      }
      posicaoY += 1;
      screenGotoxy(posicaoX, posicaoY);
      if (head->prox != NULL) {
        printf("|");
        posicaoY += 1;
      }

      screenGotoxy(posicaoX, posicaoY);
      head = head->prox;
    }
  } else if (head == NULL) {
    printf("Error: head == NULL");
  }
}

void ordenarLista(carta **head) {
  int n = 1, len = calcularTamanhoLista(*head);
  carta *atual = *head;
  for(int i = 0; i < len; i++){
    atual = *head;
    while(atual->prox != NULL){
      if(atual->ano > atual->prox->ano){
        int atualAno = (atual)->ano;
        int proxAno = (atual->prox)->ano;
        char atualNome[TAMANHO];
        char proxNome[TAMANHO];
        int atualCor = atual->errado;
        int proxCor = atual->prox->errado;
        strcpy(atualNome, (atual)->nome);
        strcpy(proxNome, (atual->prox)->nome);
        (atual)->ano = proxAno;
        strcpy((atual)->nome, proxNome);
        (atual->prox)->ano = atualAno;
        strcpy((atual->prox)->nome, atualNome);
        atual->errado = proxCor;
        atual->prox->errado = atualCor;
      }
      atual = atual->prox;
    }
  }
}


int calcularTamanhoLista(carta *head) {
  int tamanho = 0;
  while (head != NULL) {
    tamanho++;
    head = head->prox;
  }
  return tamanho;
}

void colocarID(carta **head) {
  carta *n = *head;
  int cont = 0;
  while (n != NULL) {
    n->ID = cont;
    cont++;
    n = n->prox;
  }
}

void limparLista(carta **head) {
  carta *n = *head;
  while (n != NULL) {
    n = n->prox;
    free(n);
  }
}

void printSeta(int qtdTermos, int x, int y) {
  screenSetColor(YELLOW, BLACK);
  if (qtdTermos != 0) {
    screenGotoxy(x, y);
    printf("➣");
  }
}

void apagarSeta(int x, int y) {
  screenGotoxy(x, y);
  printf(" ");
}

void menu() {
  screenInit(1);

  screenSetColor(YELLOW, BLACK);
  screenGotoxy(20, 6);
  printf("Bem Vindo ao WikiTrivia!");

  screenSetColor(WHITE, BLACK);
  screenGotoxy(20, 10);
  printf(" 1 - Iniciar Jogo");

  screenGotoxy(20, 14);
  printf(" 2 - Como Jogar");

  screenGotoxy(20, 18);
  printf(" 3 - Ranking");

  screenGotoxy(20, 22);
  printf(" 4 - Créditos");

  screenGotoxy(20, 26);
  printf(" 5 - Sair");

  screenUpdate();
}
void comoJogar() {

  screenInit(1);
  screenSetColor(WHITE, BLACK);
  screenGotoxy(16, 5);
  printf("O jogo iniciará com uma carta na linha do tempo");
  screenGotoxy(23, 6);
  printf("e uma carta na mão do jogador.");
  screenGotoxy(13, 9);
  printf("O seu objetivo é colocar a carta em sua mão na ordem");
  screenGotoxy(9, 10);
  printf("crescente na mesa, baseando-se no ano disponível nas cartas.");
  screenGotoxy(9, 13);
  printf("A cada carta colocada no lugar errado você perde uma vida.\n");
  screenGotoxy(23, 14);
  printf("Você tem um total de três vidas.");
  screenGotoxy(7, 17);
  printf("Para selecionar onde colocar a carta, use o W e S do seu teclado.");
  screenGotoxy(22, 18);
  printf("Use o ENTER para confirmar a seleção!");
  screenGotoxy(35, 20);
  printf("Boa sorte!");
}

int jogo(carta *head) {

  int posicaoSetaX = 3;
  int posicaoSetaY = 6;
  carta *novaCarta = escolherCarta(head);

  screenUpdate();
  atualizarHead(&head);
  ordenarLista(&head);
  printListaTela(head);
  printNovaCarta(*novaCarta);

  int vidas = contarVidas(head);
  if (vidas <= 0){
    return 0;
  }else{
    printVida(vidas);
  }
  
  printSeta(3, posicaoSetaX, posicaoSetaY);

  int limite = calcularTamanhoLista(head), contLimite = 0;
  int posicaoAdicionar = 1;

  while (1) {
    screenUpdate();
    if (keyhit()) {
      char anda = readch();
      if (anda == 's') {
        contLimite++;
        posicaoAdicionar++;
        if (contLimite == limite) {
          contLimite--;
          posicaoAdicionar--;
        } else if (contLimite == -1) {
          contLimite++;
          posicaoAdicionar++;
        } else {
          apagarSeta(posicaoSetaX, posicaoSetaY);
          posicaoSetaY += 2;
          printSeta(3, posicaoSetaX, posicaoSetaY);
          screenUpdate();
        }
      } else if (anda == 'w') {
        contLimite--;
        posicaoAdicionar--;
        if (contLimite == -2) {
          contLimite++;
          posicaoAdicionar++;
        } else if (contLimite == limite) {
          contLimite--;
          posicaoAdicionar--;
        } else {
          apagarSeta(posicaoSetaX, posicaoSetaY);
          posicaoSetaY -= 2;
          printSeta(3, posicaoSetaX, posicaoSetaY);
          screenUpdate();
        }
      } else if (anda == 13) { // enter
        inserirLista(&head, novaCarta->nome, novaCarta->ano, posicaoAdicionar,
                     novaCarta->acontecimento, 0);
        verificarErro(&head, novaCarta->ano);
        return 1;
      }
    }
  }
  return 0;
}

void atualizarHead(carta **head) {
  if (*head != NULL) {
    while ((*head)->ant != NULL) {
      (*head) = (*head)->ant;
    }
  }
}

void printNovaCarta(carta carta) {
  screenGotoxy(48, 5);
  printf("%s", carta.nome);
  screenGotoxy(47, 6);
  printf("%s", carta.acontecimento);
}

void printVida(int quantidade) {
  int posicaoX = 38;
  screenGotoxy(posicaoX, 2);
  for (int i = 0; i < quantidade; i++) {
    printf("♥");
    posicaoX -= 2;
    screenGotoxy(posicaoX, 2);
  }
}

void verificarErro(carta **head, int ano) {
    carta *n = *head;
    while (n != NULL && n->ano != ano) {
        n = n->prox;
    }
    if (n != NULL && n->prox != NULL && n->ano > n->prox->ano) {
        n->errado = 1;
    }
    else if (n != NULL && n->ant != NULL && n->ano < n->ant->ano) {
        n->errado = 1;
    }
}

int contarVidas(carta *head){
  int cont = 3;
  while(head != NULL){
    if(head->errado == 1){
      cont--;
    }
    head = head->prox;
  }
  return cont;
}
