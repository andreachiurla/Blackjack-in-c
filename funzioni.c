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
    char strIn[30];

    // chiede il numero di nome
    while(1){
        printf(BGRN);
        printf("Quanti giocatori (max 6)? ");
        printf(DEFAULT_COLOR);

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
            printf("Per favore, inserire un valore compreso tra 1 e 6.\n");
        }
    }

    // se parteciperà un solo giocatore non vengono effettuati controlli
    if(nGiocatori == 1){
        printf(BGRN);
        printf("Inserire il nome del giocatore: ");
        printf(DEFAULT_COLOR);

        memset(game->giocatori[1].nome, 0, NOMEGIOCATOREMAXLEN);
        fgets(game->giocatori[1].nome, NOMEGIOCATOREMAXLEN, stdin);
    }else{
        // nel caso ci sia più di un giocatore entra nel ciclo che chiede il nome e controlla che non sia uguale a uno già inserito
        printf(BGRN);
        printf("\nInserire il nome di ciascun giocatore.\n");
        printf(DEFAULT_COLOR);

        while(temp <= nGiocatori){
            printf("Giocatore %d: ", temp);
            memset(game->giocatori[temp].nome, 0, NOMEGIOCATOREMAXLEN);     // imposta tutto a 0
            fgets(game->giocatori[temp].nome, NOMEGIOCATOREMAXLEN, stdin);
            // printf("Immesso: %s\n", game->giocatori[temp].nome);     // DEBUG

            if(!strcmp(game->giocatori[temp].nome, "\n")){   // se non ha inserito niente non è valido
                temp--;
                printf("Non è stato inserito niente, per favore inserire un nome\n");
            }

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

/*
 * 1. setMoney
 * 2. imposta tutte le variabili dei soldi posseduti da ogni giocatore alla costante STARTINGMONEY
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void setMoney(Game *game){
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        game->giocatori[giocatore].money = STARTINGMONEY;
    }
}

/*
 * 1. resetPlayerPoints
 * 2. azzera tutti i valori necessari per effettuare un'altra manche
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void resetPlayersPoints(Game *game) {
    for (int giocatore = 0; giocatore <= game->nGiocatori; ++giocatore) {
        game->giocatori[giocatore].punteggio = 0;
        game->giocatori[giocatore].bet = 0;
        game->giocatori[giocatore].done = false;
        game->giocatori[giocatore].manyPlayerCards = 0;
        game->giocatori[giocatore].isAsso = false;
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
                // chiede la puntata finché non viene inserito un valore valido
                printf(BGRN);
                printf("%s, fai la tua puntata (min 1 max 5, 0 per non giocare): ", game->giocatori[1].nome);
                printf(DEFAULT_COLOR);

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
        printf(BGRN);
        printf("Fate la vostra puntata. (min 1 max 5, 0 per non giocare)\n");
        printf(DEFAULT_COLOR);

        for(int giocatore = 1; giocatore <= game->nGiocatori;){
            if(game->giocatori[giocatore].money > 0) {  // chiede bet solo se gli rimangono dei soldi
                printf("%s: ", game->giocatori[giocatore].nome);
                scanf("%d", &game->giocatori[giocatore].bet);

                // controllo valore inserito
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

    srand(time(NULL));

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

        fprintf(mazzoCarte, " -");

        fprintf(mazzoCarte, "\n");
    }
}

/*
 * 1. areAllCardsDrew
 * 2. controlla se tutte le carte del mazzo sono state pescate, nel caso, toglie da tutte le carte il segno che indica che sono state pescate
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void areAllCardsDrew(FILE *mazzo){
    char sign;
    int cardsDrew = 0;

    // ciclo che controlla se sono state pescate tutte
    // printf("Mazzo: ");
    fseek(mazzo, 7, SEEK_SET);
    for (int riga = 1; riga <= 104; ++riga) {
        sign = fgetc(mazzo);
        // printf("%d%c", riga%10, sign);
        if(sign == '$'){
            cardsDrew++;
        }
        fseek(mazzo, +8, SEEK_CUR);
    }
    // printf("\n");

    // ciclo che se sono state pescate tutte toglie il simbolo
    if(cardsDrew == 104){
        printf("MISCHIANDO CARTE..\n");
        fseek(mazzo, 7, SEEK_SET);
        for (int riga = 0; riga <= 104; ++riga) {
            fprintf(mazzo, "-");
            fseek(mazzo, +8, SEEK_CUR);
        }
    }

}

/*
 * 1. pescaCarta
 * 2. ciclo che trova la carta corrispondente a un numero casuale finché non ne trova una non ancora pescata
 * 3. riceve una stringa e il puntatore al file del mazzo
 * 4. void
 */
void pescaCarta(FILE *mazzo, char carta[]) {
    int numRandom;
    int numRiga;
    char cartaLetta[4];
    char segno;
    bool found = false;

    areAllCardsDrew(mazzo); // prima di pescare controllo se tutte le carte sono già state pescate (drew)

    // porto il puntatore all'inizio del file
    fseek(mazzo, 0, SEEK_SET);

    while (!found) {
        numRandom = rand() % 104 + 1;   // trovare numero casuale tra 1 e 104

        fseek(mazzo, (numRandom - 1) * 9, SEEK_SET);
        fscanf(mazzo, "%d %s %c", &numRiga, cartaLetta, &segno);

        // se ha trovato la riga
        if (numRiga == numRandom) {
            // verifico della presenza del segnale
            if (segno != '$') {
                found = true;
            }

        }

        // associo alla stringa "carta" i due caratteri indicativi della carta pescata. (4P -> 4 di picche)
        fseek(mazzo, numRandom * 9 - 5, SEEK_SET);
        fgets(carta, 3, mazzo);

        // aggiungo un carattere che indica che quella carta è già stata pescata
        fseek(mazzo, numRandom * 9 - 2, SEEK_SET);
        fprintf(mazzo, "$");
    }
}

/*
 * 1. distribuisciCarte
 * 2. prima distribuzione di carte per la manche, da a ogni giocatore due carte pescate casualmente e nasconde la seconda del banco
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void distribuisciCarte(FILE *mazzo, Game *game){
    char carta[3];  // stringa contenente i due caratteri che indicano la carta

    //printf("Numero giocatori: %d\n", game->nGiocatori);   // debug

    printf(BGRN);
    printf("Distribuzione carte:\n");
    printf(DEFAULT_COLOR);

    for(int giro = 1; giro <= 2; giro++) {
        for(int giocatore = 0; giocatore <= game->nGiocatori; giocatore++) {
            // se il giocatore non ha puntato non viene considerato
            if(giocatore > 0 && game->giocatori[giocatore].bet == 0){
                printf("%s non gioca\n", game->giocatori[giocatore].nome);
                continue;   // ignora tutte le righe successive e ricomincia il ciclo
            }
            //printf("Giocatore %d/%d giro %d\n", giocatore, game->nGiocatori, giro);   // debug

            // pesco la carta e la memorizzo nella stringa "carta"
            pescaCarta(mazzo, carta);

            // stampo la carta uscita se non è la seconda carta del banco
            if(giro == 2 && giocatore == 0){
                strcpy(game->dealerSecondCard, carta);
                printf("*seconda carta del banco nascosta*");
            }else{
                printf("%s:\t\t", game->giocatori[giocatore].nome);
                printCard(carta);
                //printf("\n");
                // la memorizzo nell'array
                updatePlayerPoints(game, cardValueOf(carta), giocatore);
                if(carta[0] == 'A') game->giocatori[giocatore].isAsso = 1;
            }

            sleep(1);   // per non far estrarre le stesse carte

            printf("\n");
        }
    }
}

/*
 * 1. printCard
 * 2. stampa il valore e il seme della carta
 * 3. riceve la carta da stampare
 * 4. void
 */
void printCard(char carta[3]){
    printf(YEL);

    // stampa valore
    if(carta[0] == 'D') printf("10 di ");
    else printf("%c di ", carta[0]);

    // stampa seme
    if(carta[1] == 'Q') printf("quadri");
    else if(carta[1] == 'F') printf("fiori");
    else if(carta[1] == 'C') printf("cuori");
    else if(carta[1] == 'P') printf("picche");
    else printf("Seme sbagliato: %c", carta[1]);

    printf(DEFAULT_COLOR);
}

/*
 * 1. cardValueOf
 * 2. restituisce il valore della carta come parametro
 * 3. riceve una carta
 * 4. restituisce il valore della carta ricevuta
 */
int cardValueOf(char carta[3]){
    if(carta[0] == 'J' || carta[0] == 'Q' || carta[0] == 'K' || carta[0] == 'D') return 10;
    else if(carta[0] == 'A') return 1;
    else return atoi(&carta[0]);
}

/*
 * 1. updatePlayerPoints
 * 2. aggiorna il punteggio del giocatore che viene indicato con la carta come parametro
 * 3. riceve la struttura principale del gioco, il valore della carta e il giocatore da aggiornare
 * 4. void
 */
void updatePlayerPoints(Game *game, int cardValue, int player) {
    // aggiorno il punteggio attuale del giocatore
    if (game->giocatori[player].punteggio == 0) game->giocatori[player].punteggio = cardValue;
    else game->giocatori[player].punteggio += cardValue;

    // incremento la quantità di carte che ha il giocatore
    game->giocatori[player].manyPlayerCards++;
    // printf("updatePlayerPoints: punteggio=%d, manyPlayerCards=%d\n", game->giocatori[player].punteggio, game->giocatori[player].manyPlayerCards);
    checkPlayersPoints(game);
}

/*
 * 1. checkBlackjackAtFirstManche
 * 2. controlla tutti i giocatori e vede se hanno fatto blackjack, controllando se ha due carte e il suo punteggio equivale a 21
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
bool checkBlackjackAtFirstManche(Game *game){
    // i giocatori che hanno ottenuto un punteggio uguale a 21 alla prima distribuzione di carte hanno fatto blackjack
    for(int giocatore = 0; giocatore <= game->nGiocatori; ++giocatore) {
        if(game->giocatori[giocatore].punteggio == 11 && game->giocatori[giocatore].isAsso == true){
            game->giocatori[giocatore].punteggio = BLACKJACK;
        }
    }

    // gestire puntate se il banco ha fatto blackjack
    if(cardValueOf(game->dealerSecondCard) == 1 && game->giocatori[0].punteggio + 11 == 21){
        dealerBlackjack(game);
        return true;
    }else if(cardValueOf(game->dealerSecondCard) == 10 && game->giocatori[0].punteggio == 1){
        dealerBlackjack(game);
        return true;
    }
    return false;
}

/*
 * 1. dealerBlackjack
 * 2. verifica se il banco ha fatto blackjack e nel caso gestisce le puntate
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void dealerBlackjack(Game *game){
    printf(MAG);
    printf("Il banco ha fatto blackjack\n");
    printf(DEFAULT_COLOR);
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        if(game->giocatori[giocatore].punteggio == -1){
            printf("%s hai pareggiato.\n", game->giocatori[giocatore].nome);
            game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet;
        }
    }
}

/*
 * 1. printEveryPlayersPoints
 * 2. stampa il punteggio di ogni giocatore
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void printEveryPlayersPoints(Game *game){
    for (int giocatore = 0; giocatore <= game->nGiocatori; ++giocatore){
        if(game->giocatori[giocatore].bet != 0 || giocatore == 0){
            printf("%s: ", game->giocatori[giocatore].nome);
            printPlayerPoints(game, giocatore);
            printf("\n");
        }
    }
}

/*
 * 1. printPlayerPoints
 * 2. stampa il punteggio del giocatore indicato
 * 3. riceve la struttura principale del gioco e il numero del giocatore di cui stampare i punti
 * 4. void
 */
void printPlayerPoints(const Game *game, int giocatore) {
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
    }else if(game->giocatori[giocatore].manyPlayerCards == 1){
        printf("%d + carta nascosta", game->giocatori[giocatore].punteggio);
    }else{
        printf("%d / %d", game->giocatori[giocatore].punteggio, game->giocatori[giocatore].punteggio + 10);
    }
}

/*
 * 1. askAndExecuteAction
 * 2. chiede a ogni giocatore che mossa fare, se stare o chiedere carta ed esegue di conseguenza le funzioni dedicate
 * 3. riceve il file del mazzo di carte e la struttura principale del gioco
 * 4. void
 */
void askAndExecuteAction(FILE *mazzo, Game *game){
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
                // stampa punti
                printf("%s hai ", game->giocatori[giocatore].nome);
                printPlayerPoints(game, giocatore);
                printf(" punti\n");

                // chiedere mossa al giocatore
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
                            printf("\n");
                            break;
                        default:
                            printf("Mossa non disponibile: %c\n", answer);
                    }
                }
                // sistema i punteggi dei giocatori
                checkPlayersPoints(game);
                // finché il punteggio non supera 21 continua a chiedere
                if(game->giocatori[giocatore].punteggio == SBALLATO || game->giocatori[giocatore].punteggio == BLACKJACK || game->giocatori[giocatore].punteggio == 21 || game->giocatori[giocatore].punteggio > 21){
                    game->giocatori[giocatore].done = true;
                    if(game->giocatori[giocatore].punteggio == 21) printf("%s: 21, ottimo!", game->giocatori[giocatore].nome);
                    printf("\n\n");
                }
            }
        }
        sleep(1);
    }
}

/*
 * 1. checkPlayersPoints
 * 2. controlla tutti i punteggio dei giocatori e a quelli che hanno sballato viene impostato il punteggio a -2 e il giocatore verrà riconosciuto come fuori dalla manche.
 * 3. riceve la struttura principale del gioco
 * 4. se il banco ha sballato (punteggio > 21) restituisce 1 (manche finita), altrimenti 0
 */
int checkPlayersPoints(Game *game){
    if(game->giocatori[0].punteggio > 21) return 1;  // se il banco ha sballato restituisce 1

    // controlla il punteggio di tutti i giocatori e imposta -2 se ha sballato
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        if(game->giocatori[giocatore].punteggio > 21){
            printf("%s hai %d punti e hai sballato.", game->giocatori[giocatore].nome, game->giocatori[giocatore].punteggio);
            game->giocatori[giocatore].punteggio = SBALLATO;
        }
    }
    return 0;
}

/*
 * 1. checkPointsOf
 * 2. controlla il punteggio del giocatore indicato e modifica il punteggio (blackjack e sballato)
 * 3. riceve la struttura principale del gioco e il numero del giocatore da controllare
 * 4. void
 */
void checkPointsOf(Game *game, int giocatore) {
    if (game->giocatori[giocatore].punteggio > 21) {
        game->giocatori[giocatore].punteggio = SBALLATO;
    } else if (game->giocatori[giocatore].punteggio == 21 && game->giocatori[giocatore].manyPlayerCards == 2){
        game->giocatori[giocatore].punteggio = BLACKJACK;
    }
}

/*
 * 1. actionPrendiCarta
 * 2. se un giocatore decide di prendere carta viene chiamata questa funzione che pesca carta e fa altri controlli
 * 3. riceve il puntatore al mazzo, la struttura principale del gioco e il numero del giocatore che vuole pescare
 * 4. void
 */
void actionPrendiCarta(FILE *mazzo, Game *game, int giocatore){
    char carta[3];
    int cardValue;

    pescaCarta(mazzo, carta);

    printf("Carta pescata: ");
    printCard(carta);

    printf("\n");

    // se viene pescato l'asso e il punteggio del giocatore supererebbe 21 (con +11), l'asso vale 1
    if((cardValue = cardValueOf(carta)) == 11 && game->giocatori[giocatore].punteggio + 11 > 21){
        cardValue = 1;
    }

    updatePlayerPoints(game, cardValue, giocatore);
}

/*
 * 1. hasDealerToPlay
 * 2. se tutti hanno sballato o hanno fatto blackjack, non c'è bisogno che il banco giochi, questa funzione stabilisce se deve essere chiamata la funzione del banco che gioca
 * 3. riceve la struttura principale del gioco
 * 4. restituisce un booleano. false se tutti hanno sballato e true in caso contrario
 */
bool hasDealerToPlay(Game *game) {
    int manyDone = 0;

    // ciclo che controlla il punteggio di tutti i giocatori e conta i giocatori che hanno sballato e quelli che hanno fatto blackjack
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        if(game->giocatori[giocatore].punteggio == SBALLATO || game->giocatori[giocatore].punteggio == BLACKJACK) manyDone++;
    }

    if(manyDone == game->nGiocatori) return false;
    else return true;
}

/*
 * 1. dealerPlays
 * 2. dopo che hanno giocato tutti i giocatori, gioca il banco e pesca finché il suo punteggio è inferiore a 17
 * 3. riceve il puntatore al file del mazzo e la struttura principale del gioco
 * 4. void
 */
void dealerPlays(FILE *mazzo, Game *game){
    char carta[3];
    int dealerSecondCardValue;

    // stampa la carta del banco e aggiorna il punteggio del banco
    printf("Seconda carta del banco: ");
    printCard(game->dealerSecondCard);
    if((dealerSecondCardValue = cardValueOf(game->dealerSecondCard)) == 1){     // se è un asso vale 11 e non 1. blackjack
        dealerSecondCardValue = 11;
    }
    updatePlayerPoints(game, dealerSecondCardValue, 0);

    sleep(1);

    printf("\n");

    // ciclo che pesca una carta finché il punteggio del banco non è maggiore o uguale a 17
    while(game->giocatori[0].punteggio < 17){
        // stampa punteggio
        printf("\nBanco: ");
        printPlayerPoints(game, 0);
        printf(" punti");

        // pesca carta
        printf("\nIl banco pesca: ");
        sleep(1);   // suspense
        pescaCarta(mazzo, carta);
        printCard(carta);

        // aggiorno punteggio
        updatePlayerPoints(game, cardValueOf(carta), 0);
        printf("\n");
    }
}

/*
 * 1. giveRevenue
 * 2. restituisce le puntate in base al risultato della manche e stampa il risultato di ogni giocatore
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void giveRevenue(Game *game){
    // ciclo che controlla ogni giocatore e restituisce la puntata in base al risultato
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        if( game->giocatori[giocatore].bet == 0){
            continue;   // ignora tutte le righe successive e ricomincia il ciclo
        }
        if(game->giocatori[giocatore].punteggio == BLACKJACK){      // se giocatore ha fatto blackjack
            if(game->giocatori[0].punteggio == BLACKJACK){      // se anche il banco ha fatto blackjack hanno pareggiato
                game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet;
                printf("%s: Blackjack pareggiato\n", game->giocatori[giocatore].nome);
            }else{  // altrimenti ha vinto x2.5
                game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet * 2.5f;
                printf("%s: Blackjack, hai vinto %f soldi\n", game->giocatori[giocatore].nome, (float)game->giocatori[giocatore].bet * 2.5f);
            }
        }else if(game->giocatori[giocatore].punteggio == SBALLATO){     // se giocatore ha sballato
            // non restituisce la puntata
        }else if(game->giocatori[0].punteggio == BLACKJACK){
            printf(RED);
            printf("%s: perso\n", game->giocatori[giocatore].nome);
            printf(DEFAULT_COLOR);
        }else if(game->giocatori[giocatore].punteggio > game->giocatori[0].punteggio){  // punteggio giocatore > punteggio banco
            game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet * 2.0f;
            printf(GRN);
            printf("%s: vinto %.1f soldi\n", game->giocatori[giocatore].nome, (float)game->giocatori[giocatore].bet * 2.0f);
            printf(DEFAULT_COLOR);
        }else if(game->giocatori[giocatore].punteggio == game->giocatori[0].punteggio){     // pareggio tra banco e giocatore
            game->giocatori[giocatore].money += (float)game->giocatori[giocatore].bet;
            printf("%s: pareggio\n", game->giocatori[giocatore].nome);
        }else{  // punteggio giocatore < punteggio banco
            printf(RED);
            printf("%s: perso\n", game->giocatori[giocatore].nome);
            printf(DEFAULT_COLOR);
        }
    }

    printf("\n");
}

/*
 * 1. printMoney
 * 2. stampa i soldi di ogni giocatore
 * 3. riceve la struttura principale del gioco
 * 4. void
 */
void printMoney(Game *game){
    for (int giocatore = 1; giocatore <= game->nGiocatori; ++giocatore) {
        printf("%s hai %.1f soldi\n", game->giocatori[giocatore].nome, game->giocatori[giocatore].money);
    }
}