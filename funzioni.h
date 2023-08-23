#include <stdbool.h>
#include <unistd.h> // libreria per sleep()
#include <ctype.h>

#define DIM 20
#define NGIOCATORIMAX 6
#define MONEY 30

typedef struct{
    int nGiocatori;
    char giocatori[NGIOCATORIMAX][DIM];
    int money[NGIOCATORIMAX];           // quanti soldi ha ogni singolo giocatore
    int bets[NGIOCATORIMAX];            // per ogni giocatore, le puntate del turno attuale
    int punteggi[NGIOCATORIMAX];        // per ogni giocatore, il punteggio attuale
    int isAsso[NGIOCATORIMAX];          // se tra le carte del giocatore c'è un asso, per mostrare i due punteggi possibili
    int manyPlayerCards[NGIOCATORIMAX]; // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
    int areSameCards[NGIOCATORIMAX];    // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
}Game;

void isBlackjack(Game *game);
void distribuisciCarte(FILE *mazzo, Game *game);
void askBets(Game *game);
int checkPoints(Game *game);
void printPoints(Game *game);
void updatePlayerPoints(Game *game, int cardValue, int player);
int playersName(char giocatori[6][DIM]);
bool keepPlaying();
void riempiFileMazzo(FILE *mazzo);
void pescaCarta(FILE *mazzo, char carta[]);
void printCard(char carta[2]);
int cardValueOf(char carta[2]);
void askAndExecuteAction(FILE *mazzo, Game *game);
void actionPrendiCarta(Game *game);
void actionRaddoppia(Game *game);
void actionDividi(Game *game);
void actionStai(Game *game);