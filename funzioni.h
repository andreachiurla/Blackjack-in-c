#include <stdbool.h>
#include <unistd.h> // libreria per sleep()
#include <ctype.h>

#define NOMEGIOCATOREMAXLEN 20
#define NGIOCATORIMAX 6
#define MONEY 30

// definizione strutture
typedef struct {
    char nome[NOMEGIOCATOREMAXLEN];
    int money;           // quanti soldi ha ogni singolo giocatore
    int bet;            // per ogni giocatore, le puntate del turno attuale
    int punteggio;        // per ogni giocatore, il punteggio attuale
    int isAsso;          // se tra le carte del giocatore c'è un asso, per mostrare i due punteggio possibili
    int manyPlayerCards; // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
} Giocatore;

typedef struct{
    int nGiocatori;
    Giocatore giocatori[NGIOCATORIMAX];
    int bancoSecondCardValue;
}Game;

// definizione funzioni
void checkBlackjackAtFirstManche(Game *game);
void distribuisciCarte(FILE *mazzo, Game *game);
void askBets(Game *game);
int checkPoints(Game *game);
void printPoints(Game *game);
void updatePlayerPoints(Game *game, int cardValue, int player);
int askPlayersName(Game * game);
void printPlayersName(Game *game);
bool keepPlaying();
void riempiFileMazzo(FILE *mazzoCarte);
void pescaCarta(FILE *mazzo, char carta[]);
void printCard(char carta[2]);
int cardValueOf(char carta[2]);
int askAndExecuteAction(FILE *mazzo, Game *game);
void actionPrendiCarta(FILE *mazzo, Game *game, int giocatore);
void actionStai(Game *game);
void bancoBlackjackAtFirstManche(Game *game);
void bancoPlays(Game *game);