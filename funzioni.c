#include <stdio.h>
#include "funzioni.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>   // utilizzo di rand() e srand()

/*
 * 1. playersName
 * 2. richiede quanti giocatori saranno presenti nella partita e poi i rispettivi nomi, controllando che non ne vengano inseriti di uguali
 * 3. riceve la matrice (array di stringhe) da modificare con i nomi dei giocatori
 * 4. restituisce il numero dei partecipanti
 */
int playersName(char giocatori[NGIOCATORIMAX][DIM]){
    int nGiocatori;
    int temp = 0;
    char trash;
    char strIn[30];

    // chiede il numero di giocatori
    while(1){
        printf("Quanti giocatori? ");

        // uso la fgets per evitare problemi e bug con la scanf
        fgets(strIn, 30, stdin);

        if(strlen(strIn) == 2){
            nGiocatori = atoi(strIn);
            if(nGiocatori > 0 && nGiocatori < 7){
                break;
            }else{
            printf("Per favore, inserire un valore valido.\n");
            }
        }else{
            printf("Per favore, inserire un valore valido.\n");
        }
    }

    // se parteciperà un solo giocatore non vengono effettuati controlli
    if(nGiocatori == 1){
        printf("Inserire il nome del giocatore: ");
        fgets(giocatori[0], DIM, stdin);
    }
    else if(nGiocatori > 1){
        // nel caso ci sia più di un giocatore entra nel ciclo che chiede il nome e controlla che non sia uguale a uno già inserito
        printf("\nInserire il nome di ciascun giocatore.\n");
        while(temp < nGiocatori){
            printf("Giocatore %d: ", temp + 1);
            fgets(giocatori[temp], DIM, stdin);
            printf("Immesso: %s\n", giocatori[temp]);

            // se è stato appena chiesto il nome al primo giocatore non vengono effettuati controlli
            if(temp == 0) {
                temp++;
            }else{
                for(int i = 0; i < temp; ++i) {
                    if(strcasecmp(giocatori[i], giocatori[temp]) == 0) {    // utilizzo strcasecmp perché non è case-sensitive -> paolo e PaoLo li considera uguali
                        printf("Giocatore %d, inserisci un nome diverso dal giocatore %d.\n", temp + 1, i + 1);
                        temp--;
                        break;
                    }
                }

                temp++;
            }
        }
    }

    // sostituisce il '\n' nelle stringhe con '\0'
    for (int i = 0; i < nGiocatori; ++i) {
        giocatori[i][strcspn(giocatori[i], "\n")] = '\0';
    }

    return nGiocatori;
}

/*
 * 1. keepPlaying
 * 2. chiede se l'utente desidera terminare il programma o continuare a giocare
 * 3. no parametri
 * 4. true se il gioco continua, false in caso contrario
 */
bool keepPlaying(){
    char askPlaying;

    printf("Vuoi continuare a giocare? (S/N) ");
    scanf("%c", &askPlaying);
    scanf("%c", &askPlaying);

    if(askPlaying == 'S' || askPlaying == 's'){
        return true;
    }else{
        return false;
    }
}

/*
 * 1. askBets
 * 2. chiede a ogni giocatore di fare la propria puntata, salvandola nell'array dedicato
 * 3. riceve l'array con le bet, i nomi dei giocatori e il numero dei giocatori
 * 4. void
 */
void askBets(Game *game){
    // cambia il procedimento in base al numero dei giocatori
    if(game->nGiocatori == 1){
        while(1){
            printf("%s, fai la tua puntata: ", game->giocatori[0]);
            scanf("%d", &game->bets[0]);
            if (game->bets[0] < 0 || game->bets[0] > 5) {
                printf("Inserisci un valore compreso tra 1 e 5.\n");
            }else{
                break;
            }
        }
    }else{
        printf("Fate la vostra puntata.\n");
        for(int i = 0; i < game->nGiocatori;){
            printf("%s: ", game->giocatori[i]);
            scanf("%d", &game->bets[i]);

            if(game->bets[i] < 0 || game->bets[i] > 5){
                printf("Inserisci un valore compreso tra 1 e 5.\n");
            }else{
                ++i;
            }
        }
    }

}

/*
 * 1. riempiFileMazzo
 * 2. formatta il file dove verranno salvate le carte uscite
 *    il file è così composto:
 *    nRiga valoreCarta-semeCarta
 * 3. riceve il puntatore a file
 * 4. void
 */
void riempiFileMazzo(FILE *mazzo){
    int j = 1;  // valore che si resetta ogni cambio di seme (ogni 13 carte)
    int k = 1;  // valore che si resetta al cambio del mazzo (dopo 54 carte)
    for (int i = 1; i <= 104; ++i, ++j) {

        // cambio di seme
        if(j == 14){
            j = 1;
            k++;
        }
        // cambio di mazzo
        if(k == 5) k = 1;

        // stampa del numero della riga
        if(i < 10){
            fprintf(mazzo, "%d   ", i);
        }else if(i <= 99){
            fprintf(mazzo, "%d  ", i);
        }else{
            fprintf(mazzo, "%d ", i);
        }

        // stampa del valore della carta
        if(j == 1) fprintf(mazzo, "A");
        else if(j >= 2 && j < 10) fprintf(mazzo, "%d", j);
        else if(j == 10) fprintf(mazzo, "D");
        else if(j == 11) fprintf(mazzo, "J");
        else if(j == 12) fprintf(mazzo, "Q");
        else if(j == 13) fprintf(mazzo, "K");

        // stampa del seme
        if(k == 1) fprintf(mazzo, "Q");
        if(k == 2) fprintf(mazzo, "F");
        if(k == 3) fprintf(mazzo, "C");
        if(k == 4) fprintf(mazzo, "P");

        fprintf(mazzo, "  ");

        fprintf(mazzo, "\n");
    }
}

/*
 * 1. pescaCarta
 * 2. sceglie un numero random tra 1 e 104, il numero estratto corrisponde a una carta, se questa carta non è già uscita è il risultato che rimane nella stringa "carta"
 *    due funzioni di questo tipo eseguite di seguito non funzionano. usare sleep().
 * 3. riceve una stringa e il puntatore al file del mazzo
 * 4. void
 */
void pescaCarta(FILE *mazzo, char carta[]){
    int numRandom;
    char strFile[DIM] = {'a'};
    int numFile;
    char segnale;
    int correctCard = 1;

    srand(time(NULL));

    // porto il puntatore all'inizio del file
    fseek(mazzo, 0, SEEK_SET);

    numRandom = rand() % 103 + 1;   // trovare numero casuale tra 1 e 104

    for (int i = 0; !feof(mazzo); ++i) {

        if (i != 0) fseek(mazzo, +6, SEEK_CUR);  // sposto il puntatore fino al valore della riga successiva

        // legge il valore della riga e lo trasforma in intero
        fgets(strFile, 4, mazzo);
        numFile = atoi(strFile);

        // se ha trovato la riga
        if (numFile == numRandom) {
            // controllo della presenza del segnale
            fseek(mazzo, +4, SEEK_CUR);
            segnale = fgetc(mazzo);
            if (segnale != '$') break;  // se non c'è il segnale interrompe
            else{
                numRandom = rand() % 103 + 1;
                i = 0;
                fseek(mazzo, 0, SEEK_SET);
            }
            break;
        }

    }

    // associo alla stringa "carta" i due caratteri indicativi della carta pescata. (4P -> 4 di picche)
    fseek(mazzo, numRandom*9-5, SEEK_SET);
    fgets(carta, 3, mazzo);

    // aggiungo un carattere che indica che quella carta è già stata pescata
    fseek(mazzo, numRandom*9-2, SEEK_SET);
    fprintf(mazzo, "$");
}

/*
 * 1. distribuisciCarte
 * 2.
 * 3.
 * 4.
 */
void distribuisciCarte(FILE *mazzo, Game *game){
    char carta[2];  // stringa contenente i due caratteri che indicano la carta
    int nGiocatoriRiserva = game->nGiocatori;

    for(int giro = 1; giro <= 2; giro++) {
        for(int giocatore = 0; giocatore <= game->nGiocatori; giocatore++) {
            // pesco la carta e la memorizzo nella stringa "carta"
            pescaCarta(mazzo, carta);
//            game->nGiocatori = nGiocatoriRiserva;
            // stampo la carta uscita
            if (giocatore != 0) {
                printf("%s: ", game->giocatori[giocatore - 1]);
            } else {
                printf("Banco: ");
            }
            printCard(carta);
            printf("\n");
            // la memorizzo nell'array
            updatePlayerPoints(game, cardValueOf(carta), giocatore);
            if(carta[0] == 'A') game->isAsso[giocatore] = 1;         //  NON VAAAAAA

            sleep(1);   // per non far estrarre le stesse carte
        }
    }
}

/*
 * 1. printCard
 * 2. stampa il valore e il seme della carta
 * 3. riceve i due caratteri della carta (valore e seme)
 * 4. void
 */
void printCard(char carta[2]){
    // stampa valore
    if(carta[0] == 'D') printf("10 di ");
    else printf("%c di ", carta[0]);

    // stampa seme
    if(carta[1] == 'Q') printf("quadri");
    else if(carta[1] == 'F') printf("fiori");
    else if(carta[1] == 'C') printf("cuori");
    else if(carta[1] == 'P') printf("picche");
    else printf("Seme sbagliato: %c", carta[1]);
}

/*
 * 1. cardValueOf
 * 2.
 * 3.
 * 4.
 */
int cardValueOf(char carta[2]){
    if(carta[0] == 'J' || carta[0] == 'Q' || carta[0] == 'K' || carta[0] == 'D') return 10;
    else if(carta[0] == 'A') return 11;
    else return atoi(&carta[0]);
}

/*
 * 1. updatePlayerPoints
 * 2. aggiorna le variabili che indicano la situazione della manche del giocatore
 * 3. riceve l'array con i punteggi dei giocatori, il numero del giocatore da aggiornare, il valore della carta e l'array che indica se ha due carte uguali (per dividere)
 * 4. void
 */
void updatePlayerPoints(Game *game, int cardValue, int player) {
    // se il valore delle due carte è uguali, il giocatore potrà dividere
    if(game->punteggi[player] == cardValue) game->areSameCards[player] = 1;

    // aggiorno il punteggio attuale del giocatore
    if (game->punteggi[player] == 0) game->punteggi[player] = cardValue;
    else game->punteggi[player] += cardValue;

    // incremento la quantità di carte che ha il giocatore
    game->manyPlayerCards[player]++;
}

/*
 * 1. isBlackjack
 * 2. controlla tutti i giocatori e vede se hanno fatto blackjack, controllando se ha due carte e il suo punteggio equivale a 21
 * 3. riceve l'array che indica quante carte hanno i giocatori, l'array con i punteggi e quanti giocatori ci sono
 * 4. void
 */
void isBlackjack(Game *game){
    // per ogni giocatore, se hanno ottenuto un punteggio uguale a 21, con solo due carte hanno fatto sicuramente blackjack
    for(int i = 0; i < game->nGiocatori; ++i) {
        if(game->punteggi[i] == 21 && game->manyPlayerCards[i] == 2){
            game->punteggi[i] = -1;
        }
    }
}

/*
 *
 */
void printPoints(Game *game){
    printf("Banco: ");
    if(game->punteggi[0] == -1){
        printf("BLACKJACK!");
    }else if(game->isAsso[0] == 0){
        printf("%d", game->punteggi[0]);
    }else {
        printf("%d / %d", game->punteggi[0], game->punteggi[0] + 10);
    }

    printf("\n");

    for (int i = 1; i < game->nGiocatori + 1; ++i) {
        printf("%s: ", game->giocatori[i - 1]);
        if(game->punteggi[i] == -1){
            printf("BLACKJACK!");
        }else if(game->isAsso[i] == 0){
            printf("%d", game->punteggi[i]);
        }else{
            printf("%d / %d", game->punteggi[i], game->punteggi[i] + 10);
        }
    }
}

/*
 *
 */
void askAndExecuteAction(FILE *mazzo, Game *game){
    char dummy;
    char answer;
    char correctedAnswer;

    char carta[2];
    for (int i = 1; i <= game->nGiocatori; ++i) {
        printf("%s, hai %d punti\n", game->giocatori[i - 1], game->punteggi[i]);
        if(game->punteggi[i] < 21){
            if(game->areSameCards[i] == 0){
                printf("Stai, Prendi carta o Raddoppi? (S/P/R): ");
            }else{
                printf("Stai, Prendi carta, Raddoppi o Dividi? (S/P/R/D)");
            }
            scanf("%c%c", &answer, &dummy);
            correctedAnswer = toupper(answer);
            if(game->areSameCards[i] == 0 && correctedAnswer == 'D') {
                correctedAnswer = ' ';
            }
            switch(correctedAnswer){
                case 'P':
                    actionPrendiCarta(game);
                    break;
                case 'R':
                    actionRaddoppia(game);
                    break;
                case 'D':
                    actionDividi(game);
                    break;
                case 'S':
                    actionStai(game);
                    break;
                default:
                    printf("Risposta errata: %c\n", answer);
            }
        }
    }
}

/*
 * 1. checkPoints
 * 2. controlla tutti i punteggi dei giocatori a quelli che hanno sballato viene impostato il punteggio a -2 e il giocatore verrà riconosciuto come fuori dalla manche.
 * 3. riceve l'array con i punteggi e il numero dei giocatori
 * 4. se il banco ha sballato (punteggio > 21) restituisce 1 (manche finita), altrimenti 0
 */
int checkPoints(Game *game){
    if(game->punteggi[0] > 21) return 1;
    for (int i = 1; i <= game->nGiocatori; ++i) {
        if(game->punteggi[i] > 21) game->punteggi[i] = -2;
    }
    return 0;
}

/*
 *
 */
void actionStai(Game *game){
    char carta[2];

    printCard(carta);

    printf("\n");

    // updatePlayerPoints(game, cardValueOf(carta), );
}

/*
 *
 */
void actionDividi(Game *game){
    char carta[2];

    printCard(carta);

    printf("\n");

    //updatePlayerPoints(punteggi, player, cardValueOf(carta), areSameCards, manyPlayerCards);
}

/*
 *
 */
void actionPrendiCarta(Game *game){
    char carta[2];

    printCard(carta);

    printf("\n");

    //updatePlayerPoints(punteggi, player, cardValueOf(carta), areSameCards, manyPlayerCards);
}

/*
 *
 */
void actionRaddoppia(Game *game){
    char carta[2];

    printCard(carta);

    printf("\n");

    // !! RADDOPPIA LA PUNTATA !!

    //updatePlayerPoints(punteggi, player, cardValueOf(carta), areSameCards, manyPlayerCards);
}