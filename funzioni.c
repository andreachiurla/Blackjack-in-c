#include <stdio.h>
#include "funzioni.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>   // utilizzo di rand() e srand()

/*
 * 1. askPlayersName
 * 2. richiede quanti nome saranno presenti nella partita e poi i rispettivi nomi, controllando che non ne vengano inseriti di uguali
 * 3. riceve la matrice (array di stringhe) da modificare con i nomi dei nome
 * 4. restituisce il numero dei partecipanti
 */
int askPlayersName(Game * game){
    int nGiocatori;
    int temp = 1;
    char trash;
    char strIn[30];

    // chiede il numero di nome
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
        memset(game->giocatori[1].nome, 0, NOMEGIOCATOREMAXLEN);
        fgets(game->giocatori[1].nome, NOMEGIOCATOREMAXLEN, stdin);
    }else{
        // nel caso ci sia più di un giocatore entra nel ciclo che chiede il nome e controlla che non sia uguale a uno già inserito
        printf("\nInserire il nome di ciascun giocatore.\n");
        while(temp <= nGiocatori){
            printf("Giocatore %d: ", temp);
            memset(game->giocatori[temp].nome, 0, NOMEGIOCATOREMAXLEN);     // imposta tutto a 0
            fgets(game->giocatori[temp].nome, NOMEGIOCATOREMAXLEN, stdin);
            // printf("Immesso: %s\n", game->giocatori[temp].nome);     // DEBUG

            // se è stato appena chiesto il nome al primo giocatore non vengono effettuati controlli
            if(temp == 1) {
                temp++;
            }else{
                for(int i = 1; i < temp; ++i) {
                    if(strcasecmp(game->giocatori[i].nome, game->giocatori[temp].nome) == 0) {    // utilizzo strcasecmp perché non è case-sensitive -> paolo e PaoLo li considera uguali
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
    for (int i = 1; i <= nGiocatori; ++i) {
        game->giocatori[i].nome[strcspn(game->giocatori[i].nome, "\n")] = '\0';
    }

    return nGiocatori;
}

void setMoney(Game *game){
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        game->giocatori[giocatore].money = STARTINGMONEY;
    }
}

void printPlayersName(Game *game) {
    for(int giocatore = 1; giocatore <= game->nGiocatori; giocatore++) {
        printf("Giocatore n. %d: %s\n", giocatore, game->giocatori[giocatore].nome);
    }
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
 * 3. riceve l'array con le bet, i nomi dei nome e il numero dei nome
 * 4. void
 */
void askBets(Game *game){
    // cambia il procedimento in base al numero dei giocatori
    if(game->nGiocatori == 1){
        while(1){
            if(game->giocatori[1].money > 0){
                printf("%s, fai la tua puntata: ", game->giocatori[1].nome);
                scanf("%d", &game->giocatori[1].bet);
                if (game->giocatori[1].bet < 0 || game->giocatori[1].bet > 5) {
                    printf("Inserisci un valore compreso tra 1 e 5.\n");
                }else if (game->giocatori[1].money < game->giocatori[1].bet) {
                    printf("Non hai abbastanza soldi, hai solo %.1f", game->giocatori[1].money);
                }else {
                    game->giocatori[1].money -= (float) game->giocatori[1].bet;
                    break;
                }
            }
        }
    }else{
        printf("Fate la vostra puntata.\n");
        for(int giocatore = 1; giocatore <= game->nGiocatori;){
            if(game->giocatori[giocatore].money > 0) {  // chiede bet solo se gli rimangono dei soldi
                printf("%s: ", game->giocatori[giocatore].nome);
                scanf("%d", &game->giocatori[giocatore].bet);

                if (game->giocatori[giocatore].bet < 0 || game->giocatori[giocatore].bet > 5) {
                    printf("Inserisci un valore compreso tra 1 e 5.\n");
                } else if (game->giocatori[giocatore].money < game->giocatori[giocatore].bet) {
                    printf("%s non hai abbastanza soldi, hai solo %.1f", game->giocatori[giocatore].nome,
                           game->giocatori[giocatore].money);
                } else {
                    game->giocatori[giocatore].money -= (float) game->giocatori[giocatore].bet;
                    ++giocatore;
                }
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
void riempiFileMazzo(FILE *mazzoCarte){
    int seme = 1;  // valore che si resetta ogni cambio di seme (ogni 13 carte)
    int mazzo = 1;  // valore che si resetta al cambio del mazzoCarte (dopo 54 carte)
    for (int i = 1; i <= 104; ++i, ++seme) {

        // cambio di seme
        if(seme == 14){
            seme = 1;
            mazzo++;
        }
        // cambio di mazzoCarte
        if(mazzo == 5) mazzo = 1;

        // stampa del numero della riga
        if(i < 10){
            fprintf(mazzoCarte, "%d   ", i);
        }else if(i <= 99){
            fprintf(mazzoCarte, "%d  ", i);
        }else{
            fprintf(mazzoCarte, "%d ", i);
        }

        // stampa del valore della carta
        if(seme == 1) fprintf(mazzoCarte, "A");
        else if(seme >= 2 && seme < 10) fprintf(mazzoCarte, "%d", seme);
        else if(seme == 10) fprintf(mazzoCarte, "D");
        else if(seme == 11) fprintf(mazzoCarte, "J");
        else if(seme == 12) fprintf(mazzoCarte, "Q");
        else if(seme == 13) fprintf(mazzoCarte, "K");

        // stampa del seme
        if(mazzo == 1) fprintf(mazzoCarte, "Q");
        if(mazzo == 2) fprintf(mazzoCarte, "F");
        if(mazzo == 3) fprintf(mazzoCarte, "C");
        if(mazzo == 4) fprintf(mazzoCarte, "P");

        fprintf(mazzoCarte, "  ");

        fprintf(mazzoCarte, "\n");
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
    char strFile[NOMEGIOCATOREMAXLEN] = {'a'};
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
    char carta[3];  // stringa contenente i due caratteri che indicano la carta

    //printf("Numero giocatori: %d\n", game->nGiocatori);   // debug

    for(int giro = 1; giro <= 2; giro++) {
        for(int giocatore = 0; giocatore <= game->nGiocatori; giocatore++) {
            // se il giocatore non ha puntato non viene considerato
            if(giocatore > 0 && game->giocatori[giocatore].bet == 0){
                printf("Giocatore %s non gioca\n", game->giocatori[giocatore].nome);
                continue;   // ignora tutte le righe successive e ricomincia il ciclo
            }
            //printf("Giocatore %d/%d giro %d\n", giocatore, game->nGiocatori, giro);   // debug

            // pesco la carta e la memorizzo nella stringa "carta"
            pescaCarta(mazzo, carta);

            // stampo la carta uscita se non è la seconda carta del banco
            if(giro == 2 && giocatore == 0){
                strcpy(game->dealerSecondCard, carta);
                printf("*seconda carta del banco nascosta*\n");
            }else{
                printf("%s: ", game->giocatori[giocatore].nome);
                printCard(carta);
                printf("\n");
                // la memorizzo nell'array
                updatePlayerPoints(game, cardValueOf(carta), giocatore);
                if(carta[0] == 'A') game->giocatori[giocatore].isAsso = 1;         //  NON VAAAAAA
            }

            sleep(1);   // per non far estrarre le stesse carte

            printf("\n");
        }
    }
}

/*
 * 1. printCard
 * 2. stampa il valore e il seme della carta
 * 3. riceve i due caratteri della carta (valore e seme)
 * 4. void
 */
void printCard(char carta[3]){
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
 * 3. riceve l'array con i punteggio dei nome, il numero del giocatore da aggiornare, il valore della carta e l'array che indica se ha due carte uguali (per dividere)
 * 4. void
 */
void updatePlayerPoints(Game *game, int cardValue, int player) {
    // aggiorno il punteggio attuale del giocatore
    if (game->giocatori[player].punteggio == 0) game->giocatori[player].punteggio = cardValue;
    else game->giocatori[player].punteggio += cardValue;

    // incremento la quantità di carte che ha il giocatore
    game->giocatori[player].manyPlayerCards++;
    // printf("updatePlayerPoints: punteggio=%d, manyPlayerCards=%d\n", game->giocatori[player].punteggio, game->giocatori[player].manyPlayerCards);
}

/*
 * 1. checkBlackjackAtFirstManche
 * 2. controlla tutti i giocatori e vede se hanno fatto blackjack, controllando se ha due carte e il suo punteggio equivale a 21
 * 3. riceve l'array che indica quante carte hanno i nome, l'array con i punteggio e quanti nome ci sono
 * 4. void
 */
void checkBlackjackAtFirstManche(Game *game){
    // per ogni giocatore, se hanno ottenuto un punteggio uguale a 21, con solo due carte hanno fatto sicuramente blackjack
    for(int giocatore = 0; giocatore < game->nGiocatori; ++giocatore) {
        if(game->giocatori[giocatore].punteggio == 21 && game->giocatori[giocatore].manyPlayerCards == 2){
            game->giocatori[giocatore].punteggio = -1;
        }
    }

    // gestire puntate se il banco ha fatto blackjack
    if(game->giocatori[0].punteggio == -1){
        dealerBlackjackAtFirstManche(game);
    }
}

/*
 *
 */
void dealerBlackjackAtFirstManche(Game *game){
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        if(game->giocatori[giocatore].punteggio == -1){
            printf("%s hai pareggiato.\n", game->giocatori[giocatore].nome);
            game->giocatori[giocatore].money += game->giocatori[giocatore].bet;
        }
    }
}

/*
 *
 */
void printPoints(Game *game){
    for (int giocatore = 0; giocatore <= game->nGiocatori; ++giocatore) {
        printf("%s: ", game->giocatori[giocatore].nome);
        if(game->giocatori[giocatore].punteggio == -1){
            printf("BLACKJACK!");
        }else if(game->giocatori[giocatore].punteggio == -2){
            printf("sballato");
        }else if(game->giocatori[giocatore].isAsso == 0 || game->giocatori[giocatore].punteggio + 10 > 21){
            printf("%d", game->giocatori[giocatore].punteggio);
        }else if(giocatore == 0 && game->giocatori[giocatore].isAsso == 0){
            if(game->giocatori[giocatore].punteggio + 10 >= 17){
                printf("%d", game->giocatori[giocatore].punteggio + 10);
            }else{
                printf("%d", game->giocatori[giocatore].punteggio + 10);
            }
        }else{
            printf("%d / %d", game->giocatori[giocatore].punteggio, game->giocatori[giocatore].punteggio + 10);
        }
        printf("\n");
    }
}

/*
 *
 */
int askAndExecuteAction(FILE *mazzo, Game *game){
    char answer;

    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        if( game->giocatori[giocatore].bet == 0){
            printf("%s non gioca\n", game->giocatori[giocatore].nome);
            continue;   // ignora tutte le righe successive e ricomincia il ciclo
        }
        if(game->giocatori[giocatore].punteggio != -1){
            game->giocatori[giocatore].done = false;
            // finché non sballa o decide di non chiedere più carta continua a chiedere
            while(!game->giocatori[giocatore].done){
                // chiedere mossa al giocatore
                printf("%s, hai %d punti\n", game->giocatori[giocatore].nome, game->giocatori[giocatore].punteggio);
                if(game->giocatori[giocatore].punteggio < 21){
                    printf("Stai o Prendi carta (S/P): ");
                    scanf(" %c", &answer);
                    answer = toupper(answer);

                    // printf("\nANSWER: %c\n", correctedAnswer);   // DEBUG

                    // chiamata funziona relativa alla mossa
                    switch(answer){
                        case 'P':
                            actionPrendiCarta(mazzo, game, giocatore);
                            break;
                        case 'S':
                            game->giocatori[giocatore].done = true;
                            break;
                        default:
                            printf("Mossa non disponibile: %c\n", answer);
                    }
                }
                // sistema i punteggi dei giocatori
                checkPoints(game);
                // finché il punteggio non supera 21 continua a chiedere
                if(game->giocatori[giocatore].punteggio == SBALLATO || game->giocatori[giocatore].punteggio == BLACKJACK || game->giocatori[giocatore].punteggio == 21 || game->giocatori[giocatore].punteggio > 21){
                    game->giocatori[giocatore].done = true;
                }
            }
        }
    }
}

/*
 * 1. checkPoints
 * 2. controlla tutti i punteggio dei giocatori e a quelli che hanno sballato viene impostato il punteggio a -2 e il giocatore verrà riconosciuto come fuori dalla manche.
 * 3. riceve l'array con il punteggio e il numero dei giocatori
 * 4. se il banco ha sballato (punteggio > 21) restituisce 1 (manche finita), altrimenti 0
 */
int checkPoints(Game *game){
    if(game->giocatori[0].punteggio > BLACKJACK) return 1;  // se il banco ha sbancato restituisce 1

    // controlla il punteggio di tutti i giocatori e sistema il valore (-2 se ha sbancato, -1 se ha fatto blackjack)
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        if(game->giocatori[giocatore].punteggio > BLACKJACK) game->giocatori[giocatore].punteggio = SBANCATO;
    }
    return 0;
}

/*
 *
 */
void actionPrendiCarta(FILE *mazzo, Game *game, int giocatore){
    char carta[3];
    int cardValue;

    pescaCarta(mazzo, carta);

    printCard(carta);

    printf("\n");

    // se viene pescato l'asso e il punteggio del giocatore supererebbe 21 (con +11), l'asso vale 1
    if((cardValue = cardValueOf(carta)) == 11 && game->giocatori[giocatore].punteggio + 11 > 21){
        cardValue = 1;
    }

    updatePlayerPoints(game, cardValue, giocatore);

    //printf("%s, hai %d punti\n", game->giocatori[giocatore].nome, game->giocatori[giocatore].punteggio);
}

/*
 *
 */
void dealerPlays(FILE *mazzo, Game *game){
    char carta[3];

    printCard(game->dealerSecondCard);
    updatePlayerPoints(game, cardValueOf(game->dealerSecondCard), 0);

    printf("\n");

    while(game->giocatori[0].punteggio < 17){
        pescaCarta(mazzo, carta);

        printf("Il banco pesca: ");
        printCard(carta);

        updatePlayerPoints(game, cardValueOf(carta), 0);
        printf("\n");
    }
}

void giveRevenue(Game *game){
    for (int giocatore = 1; giocatore < game->nGiocatori; ++giocatore) {
        if( game->giocatori[giocatore].bet == 0){
            continue;   // ignora tutte le righe successive e ricomincia il ciclo
        }
        if(game->giocatori[giocatore].punteggio == BLACKJACK){
            if(game->giocatori[0].punteggio == BLACKJACK){
                game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet;
            }else{
                game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet * 2.5f;
            }
        }else if(game->giocatori[giocatore].punteggio > game->giocatori[0].punteggio){
            game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet * 2.0f;
        }else if(game->giocatori[giocatore].punteggio == game->giocatori[0].punteggio){
            game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet;
        }else if(game->giocatori[giocatore].punteggio != SBANCATO && game->giocatori[giocatore].punteggio == SBANCATO){
            game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet;
        }
    }
}

void printMoney(Game *game){
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        printf("%s hai %.1f soldi\n", game->giocatori[giocatore].nome, game->giocatori[giocatore].money);
    }
}