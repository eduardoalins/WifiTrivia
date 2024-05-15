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
} carta;

int x = 34, y = 12;
int incX = 1, incY = 1;
// Tela (X: 3 - 72 Y: 1 - 48)
// ➣

void inserirLista(carta **head, char nome[TAMANHO], int ano, int cont,
                  char acontecimento[TAMANHO]);
void printListaTela(carta *head);
// void ordenarLista(carta **head, int tamanho);
int calcularTamanhoLista(carta *head);
void limparLista(carta **head);
void apagarListaTela(carta *head);
void colocarID(carta **head);
void printSeta(int qtdTermos, int x, int y);
void apagarSeta(int x, int y);
void lerArquivo(carta **head, const char *filename);
// void trocarCelula(carta **atual, carta **prox);
int jogo(carta *head);
void menu();
void comoJogar();
void atualizarHead(carta **head);
void printNovaCarta(carta carta);

int main() {
  carta *head = NULL;
  lerArquivo(&head, "bibliotecaWiki.txt");
  menu();

  while (1) {
    screenGotoxy(4, 19);
    if (keyhit()) {
      char key = readch();
      if (key == '1') {
        inserirLista(&head, "Carta 1", 1990, 0, "seila porra 1");
        inserirLista(&head, "Carta 2", 2000, 1, "seila porra 2");
        inserirLista(&head, "Carta 3", 2010, 0, "seila porra 3");
        int jogoValidacao = jogo(head);
        while (jogoValidacao == 1) {
          jogoValidacao = jogo(head);
        }
      } else if (key == '2') {
        comoJogar();
      } else if (key == '3') {
        // mostra o ranking
      } else if (key == '4') {
        // mostra os créditos
      } else if (key == '5') {
        // end
      } else {
        printf("");
      }
    }
  }

  screenDestroy();

  return 0;
}

void inserirLista(carta **head, char nome[TAMANHO], int ano, int cont,
                  char acontecimento[TAMANHO]) { // 0 coloca antes da head e
                                                 // 1 antes do primeiro termo
  carta *novo = (carta *)malloc(sizeof(carta));
  if (novo != NULL) {
    strcpy(novo->nome, nome);
    novo->ano = ano;
    strcpy(novo->acontecimento, acontecimento);
    novo->ant = NULL;
    novo->prox = NULL;

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

void lerArquivo(carta **head, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }

  char line[TAMANHO];
  while (fgets(line, sizeof(line), file)) {
    char nome[TAMANHO];
    int ano;
    char acontecimento[TAMANHO];

    line[strcspn(line, "\n")] = '\0';

    char *token = strtok(line, " / ");
    if (token != NULL) {
      strcpy(nome, token);
      token = strtok(NULL, " / ");
      if (token != NULL) {
        ano = atoi(token);
        token = strtok(NULL, " / ");
        if (token != NULL) {
          strcpy(acontecimento, token);
        }
      }
    }
  }

  fclose(file);
}

void printListaTela(carta *head) {
  screenInit(1);
  screenSetColor(YELLOW, BLACK);

  int posicaoX = 4;
  int posicaoY = 3;

  screenGotoxy(posicaoX, posicaoY);

  if (head != NULL) {
    while (head != NULL) {
      printf("%s --> %d", head->nome, head->ano);
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

// void ordenarLista(carta **head, int tamanho) {
//   int troca, n;
//   carta *atual;

//   while (troca) {
//     troca = 0;
//     atual = *head;
//     while (atual->prox != NULL) {
//       if (atual->ano > atual->prox->ano) {
//         //        trocarCelula(&atual, &atual->prox);
//         troca = 1;
//       }
//       atual = atual->prox;
//     }
//   }
// }

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
  screenGotoxy(4, 5);
  printf("Bem Vindo ao WikiTrivia!");

  screenSetColor(WHITE, BLACK);
  screenGotoxy(4, 10);
  printf(" 1 - Iniciar Jogo");

  screenGotoxy(4, 12);
  printf(" 2 - Como Jogar");

  screenGotoxy(4, 14);
  printf(" 3 - Ranking");

  screenGotoxy(4, 16);
  printf(" 4 - Créditos");

  screenGotoxy(4, 18);
  printf(" 5 - Sair");

  screenUpdate();
}
void comoJogar() {
  screenDestroy();
  screenInit(1);
  screenGotoxy(15, 5);
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
  printf("Para selecionar onde colocar a carta, use o WASD do seu teclado.");
  screenGotoxy(33, 20);
  printf("Boa sorte!");
}

int jogo(carta *head) {

  int posicaoSetaX = 3;
  int posicaoSetaY = 4;
  carta novaCarta;
  strcpy(novaCarta.nome, "Bibas");
  
  atualizarHead(&head);
  printListaTela(head);
  printNovaCarta(novaCarta);
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
        inserirLista(&head, "Carta X", posicaoAdicionar, posicaoAdicionar,
                     "seila porra 3");
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
  screenGotoxy(54, 3);
  printf("%s", carta.nome);
  screenGotoxy(54, 4);
  printf("%s", carta.acontecimento);
}
