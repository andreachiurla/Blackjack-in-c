#include <stdbool.h>
#include <unistd.h> // libreria per sleep()
#include <ctype.h>
#include <time.h>
#include <string.h>

#define NOMEGIOCATOREMAXLEN 20
#define NGIOCATORIMAX 6
#define STARTINGMONEY 30.0f
#define SBALLATO (-2)
#define BLACKJACK (-1)

#define DEFAULT_COLOR "\033[0m"

// COLORI
//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

// definizione strutture
typedef struct {
    char nome[NOMEGIOCATOREMAXLEN];     // stringa del nome del giocatore
    float money;           // quanti soldi ha ogni singolo giocatore
    int bet;            // per ogni giocatore, le puntate del turno attuale
    int punteggio;        // per ogni giocatore, il punteggio attuale
    int isAsso;          // se tra le carte del giocatore c'è un asso, per mostrare i due punteggio possibili
    int manyPlayerCards; // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
    bool done;          // serve per determinare se il giocatore non vuole più chiedere carte per la manche
} Giocatore;

typedef struct{
    int nGiocatori;     // numero giocatori
    Giocatore giocatori[NGIOCATORIMAX];     // array di strutture per ogni giocatore
    char dealerSecondCard[3];       // seconda carta del banco che viene nascosta
}Game;

// definizione funzioni
bool checkBlackjackAtFirstManche(Game *game);
void distribuisciCarte(FILE *mazzo, Game *game);
void askBets(Game *game);
int checkPlayersPoints(Game *game);
void printEveryPlayersPoints(Game *game);
void printPlayerPoints(const Game *game, int giocatore);
void updatePlayerPoints(Game *game, int cardValue, int player);
int askPlayersName(Game * game);
bool keepPlaying();
void riempiFileMazzo(FILE *mazzoCarte);
void pescaCarta(FILE *mazzo, char carta[]);
void printCard(char carta[3]);
int cardValueOf(char carta[3]);
void askAndExecuteAction(FILE *mazzo, Game *game);
void actionPrendiCarta(FILE *mazzo, Game *game, int giocatore);
void dealerBlackjack(Game *game);
void dealerPlays(FILE *mazzo, Game *game);
void setMoney(Game *game);
void giveRevenue(Game *game);
void printMoney(Game *game);
bool hasDealerToPlay(Game *game);
void resetPlayersPoints(Game *game);
void checkPointsOf(Game *game, int giocatore);
void areAllCardsDrew(FILE *mazzo);