#include <stdio.h>
#include "funzioni.h"

#include <stdlib.h>
#include <time.h>

int main() {
    printf("Blackjack by Chiurla Andrea\n\n");

    Game game;

/*
    int nGiocatori;                         // numero di partecipanti
    int money[NGIOCATORIMAX] = {MONEY, MONEY, MONEY, MONEY, MONEY, MONEY};      // quanti soldi ha ogni singolo giocatore
    int bets[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};       // puntate del turno attuale
    int areSameCards[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};       // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
    int manyPlayerCards[NGIOCATORIMAX] = {2, 2, 2, 2, 2, 2};
    int isAsso[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};     // se tra le carte del giocatore c'è un asso, per mostrare i due punteggi possibili
    int punteggi[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};
    char giocatori[NGIOCATORIMAX][DIM];        // array di stringhe - nomi dei giocatori
*/

    FILE *mazzoCarte;                       // puntatore al file in cui vengono memorizzate le carte uscite.
    bool playing = true;                    // valore booleano usato per determinare se il gioco continua o no

    /*
    int nGiocatori;                         // numero di partecipanti
    int money[NGIOCATORIMAX] = {MONEY, MONEY, MONEY, MONEY, MONEY, MONEY};      // quanti soldi ha ogni singolo giocatore
    int bets[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};       // puntate del turno attuale
    int areSameCards[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};       // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
    int manyPlayerCards[NGIOCATORIMAX] = {2, 2, 2, 2, 2, 2};
    int isAsso[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};     // se tra le carte del giocatore c'è un asso, per mostrare i due punteggi possibili
    int punteggi[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};
*/

    // formatto il file del mazzo
    if((mazzoCarte = fopen("mazzo.txt","w")) == NULL) {
        printf("Errore nell'apertura del file");
        return 1;
    }
    riempiFileMazzo(mazzoCarte);
    fclose(mazzoCarte);

    // apertura del file con controllo
    if((mazzoCarte = fopen("mazzo.txt","r+")) == NULL) {
        printf("Errore nell'apertura del file");
        return 1;
    }

    //chiedo il numero e i nomi dei giocatori
    game.nGiocatori = playersName(game.giocatori);

    // ciclo essenziale del gioco, ogni iterazione è una manche
    while(playing){
        //askBets(bets, giocatori, nGiocatori);

        distribuisciCarte(mazzoCarte, &game);
        printf("\npunteggi[0]: %d\n", game.punteggi[0]);
        isBlackjack(&game);

        printf("\n");

        askAndExecuteAction(mazzoCarte, &game);
        checkPoints(&game);

        printf("\n");
        printPoints(&game);
        printf("\n");

        playing = keepPlaying();
    }

    fclose(mazzoCarte);

    return 0;
}
