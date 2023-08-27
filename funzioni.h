#include <stdbool.h>
#include <unistd.h> // libreria per sleep()
#include <ctype.h>

#define NOMEGIOCATOREMAXLEN 20
#define NGIOCATORIMAX 6
#define STARTINGMONEY 30

// definizione strutture
typedef struct {
    char nome[NOMEGIOCATOREMAXLEN];
    float money;           // quanti soldi ha ogni singolo giocatore
    int bet;            // per ogni giocatore, le puntate del turno attuale
    int punteggio;        // per ogni giocatore, il punteggio attuale
    int isAsso;          // se tra le carte del giocatore c'è un asso, per mostrare i due punteggio possibili
    int manyPlayerCards; // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
    bool done;          // serve per determinare se il giocatore non vuole più chiedere carte per la manche
} Giocatore;

typedef struct{
    int nGiocatori;
    Giocatore giocatori[NGIOCATORIMAX];
    char dealerSecondCard[3];
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
void dealerBlackjackAtFirstManche(Game *game);
void dealerPlays(FILE *mazzo, Game *game);
void setMoney(Game *game);
void giveRevenue(Game *game);