#include <stdio.h>
#include "funzioni.h"

int main() {
    printf("Blackjack by Chiurla Andrea\n\n");

    Game game;
    bool dealerBlackjack;

    FILE *mazzoCarte;                       // puntatore al file in cui vengono memorizzate le carte uscite.
    bool playing = true;                    // valore booleano usato per determinare se il gioco continua o no

    // formatto il file del mazzo aprendolo in scrittura così sovrascrivo
    if((mazzoCarte = fopen("mazzo.txt","w")) == NULL) {
        printf("Errore nell'apertura del file");
        return 1;
    }
    riempiFileMazzo(mazzoCarte);
    fclose(mazzoCarte);

    // apertura del file in modifica con controllo
    if((mazzoCarte = fopen("mazzo.txt","r+")) == NULL) {
        printf("Errore nell'apertura del file");
        return 1;
    }

    char carta[3];
    for (int pesco = 1; pesco <= 120; ++pesco) {
        pescaCarta(mazzoCarte, carta);
    }

    // assegno alla posizione 0 dell'array dei nomi la stringa "Banco"
    strcpy(game.giocatori[0].nome, "Banco");
    game.giocatori[0].manyPlayerCards = 2;

    // chiedo il numero e i nomi dei giocatori
    game.nGiocatori = askPlayersName(&game);
    setMoney(&game);
    //printPlayersName(&game);

    // ciclo essenziale del gioco, ogni iterazione è una manche
    while(playing){
        resetPlayersPoints(&game);

        printf("\n");

        askBets(&game);

        printf("\n");

        distribuisciCarte(mazzoCarte, &game);
        printf("\n");

        dealerBlackjack = checkBlackjackAtFirstManche(&game);   // se il banco ha fatto blackjack non c'è bisogno di far giocare i giocatori
        printf("\n");

        if(dealerBlackjack == false){
            printEveryPlayersPoints(&game);

            printf("\n");

            askAndExecuteAction(mazzoCarte, &game);

            if(checkPlayersPoints(&game) == 1) {
                printf("Il banco ha sballato");
            }
            printf("\n");

            printEveryPlayersPoints(&game);
            printf("\n");

            if(hasDealerToPlay(&game)){
                dealerPlays(mazzoCarte, &game);
            }
            printf("\n");

            checkPointsOf(&game, 0);

            printEveryPlayersPoints(&game);
            printf("\n");
        }

        giveRevenue(&game);

        printMoney(&game);

        printf("\n");

        playing = keepPlaying();
    }

    fclose(mazzoCarte);

    return 0;
}