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
    int bet[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};       // puntate del turno attuale
    int areSameCards[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};       // per ogni giocatore, se c'è 0 ha due carte diverse, se c'è 1 due uguali
    int manyPlayerCards[NGIOCATORIMAX] = {2, 2, 2, 2, 2, 2};
    int isAsso[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};     // se tra le carte del giocatore c'è un asso, per mostrare i due punteggio possibili
    int punteggio[NGIOCATORIMAX] = {0, 0, 0, 0, 0, 0};
    char nome[NGIOCATORIMAX][NOMEGIOCATOREMAXLEN];        // array di stringhe - nomi dei nome
*/

    FILE *mazzoCarte;                       // puntatore al file in cui vengono memorizzate le carte uscite.
    bool playing = true;                    // valore booleano usato per determinare se il gioco continua o no

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


    strcpy(game.giocatori[0].nome, "Banco");
    game.giocatori[0].manyPlayerCards = 2;

    // chiedo il numero e i nomi dei giocatori
    game.nGiocatori = askPlayersName(&game);
    printPlayersName(&game);

    // ciclo essenziale del gioco, ogni iterazione è una manche
    while(playing){
        //askBets(bet, nome, nGiocatori);

        distribuisciCarte(mazzoCarte, &game);
        printf("\n");
        printPoints(&game);
        checkBlackjackAtFirstManche(&game);

        printf("\n");

        askAndExecuteAction(mazzoCarte, &game);
        if(checkPoints(&game) == 1) {
            printf("Banco ha sbancato");
            break;
        }

        printf("\n");
        printPoints(&game);
        printf("\n");

        playing = keepPlaying();
    }

    fclose(mazzoCarte);

    return 0;
}

