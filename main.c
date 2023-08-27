#include <stdio.h>
#include "funzioni.h"

#include <time.h>
#include <string.h>

// DA FARE: resettare il file del mazzo quando tutte le carte sono state pescate

int main() {
    printf("Blackjack by Chiurla Andrea\n\n");

    Game game;

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
    setMoney(&game);
    printPlayersName(&game);

    // ciclo essenziale del gioco, ogni iterazione è una manche
    while(playing){
        askBets(&game);

        distribuisciCarte(mazzoCarte, &game);
        printf("\n");

        checkBlackjackAtFirstManche(&game);
        printPoints(&game);

        printf("\n");

        askAndExecuteAction(mazzoCarte, &game);

        if(checkPoints(&game) == 1) {
            printf("Banco ha sbancato");
        }

        printf("\n");
        printPoints(&game);
        printf("\n");

        dealerPlays(mazzoCarte, &game);

        giveRevenue(&game);

        printPoints(&game);
        printMoney(&game);

        playing = keepPlaying();
    }

    fclose(mazzoCarte);

    return 0;
}

