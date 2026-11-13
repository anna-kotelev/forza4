# Forza 4: specifica delle regole

> **Scopo del documento.** Descrive *cosa* deve fare il gioco, non *come* è implementato.
> È la fonte di verità: se codice e documento non sono d'accordo, si corregge uno dei due.
> Ogni regola ha un **identificativo** (es. `R-BRD-01`) da citare nei commenti del codice e nei test.

| Campo     | Valore       |
|-----------|--------------|
| Versione  | 1.0          |
| Data      | 2026-11-13   |
| Stato     | Bozza        |

---

## 0. Convenzioni del documento

- **DEVE**: requisito obbligatorio. Se manca, il gioco è sbagliato.
- **DOVREBBE**: requisito consigliato. Si può rimandare.
- **Prefissi degli identificativi**:
  - `R-BRD`: Board, cioè la griglia
  - `R-MOV`: Move, cioè la mossa
  - `R-TRN`: Turn, cioè il turno
  - `R-END`: End, cioè la fine della partita
  - `R-IO`: Input/Output, cioè l'interazione con l'utente
  - `R-AI`: Artificial Intelligence, cioè l'Intelligenza Artificiale (IA) del giocatore computer

---

## 1. Glossario

| Termine            | Significato |
|--------------------|-------------|
| **Griglia**        | Il tabellone verticale di celle. |
| **Cella**          | Una posizione della griglia. È *vuota* oppure contiene la pedina di un giocatore. |
| **Pedina**         | Il segno di un giocatore. |
| **Colonna piena**  | Colonna in cui la cella più in alto non è vuota. |
| **Allineamento**   | 4 o più pedine dello stesso giocatore consecutive su una stessa linea. |
| **Mossa**          | La scelta di una colonna da parte del giocatore di turno. |
| **Ultima mossa**   | La cella appena occupata dalla pedina inserita. |

---

## 2. Sistema di coordinate

È definito una volta sola qui e vale per **tutto** il codice.

- Le coordinate interne partono da **0** (*0-based*).
- **Riga 0 = riga in alto**, riga 5 = riga in basso (stesso ordine della stampa a schermo).
- **Colonna 0 = colonna a sinistra**, colonna 6 = colonna a destra.
- Una cella si indica come `(riga, colonna)`.
- **All'utente** le colonne si mostrano numerate da **1 a 7** (*1-based*). La conversione `colonnaUtente - 1` avviene **solo** nello strato di Input/Output (I/O), mai nella logica.

```
      1   2   3   4   5   6   7      ← come le vede l'utente
    +---+---+---+---+---+---+---+
 0  |   |   |   |   |   |   |   |    ← riga 0 = in alto
 1  |   |   |   |   |   |   |   |
 2  |   |   |   |   |   |   |   |
 3  |   |   |   |   |   |   |   |
 4  |   |   |   |   |   |   |   |
 5  |   |   |   |   |   |   |   |    ← riga 5 = in basso (le pedine cadono qui)
    +---+---+---+---+---+---+---+
      0   1   2   3   4   5   6      ← indici interni
```

---

## 3. Regole della griglia (`R-BRD`)

| ID         | Regola |
|------------|--------|
| `R-BRD-01` | La griglia **DEVE** avere **6 righe** e **7 colonne**. Le dimensioni sono costanti con nome, mai numeri scritti direttamente nel codice. |
| `R-BRD-02` | All'inizio della partita tutte le celle **DEVONO** essere vuote. |
| `R-BRD-03` | Una cella **DEVE** trovarsi in uno solo di tre stati: `Vuota`, `Giocatore1`, `Giocatore2`. |
| `R-BRD-04` | Una cella occupata **NON DEVE** mai cambiare stato durante la partita: le pedine non si spostano e non si rimuovono. |
| `R-BRD-05` | Sotto una cella occupata non **DEVONO** esserci celle vuote. Questo è l'*invariante di gravità*: deve restare vero dopo ogni mossa. |

---

## 4. Regole della mossa (`R-MOV`)

| ID         | Regola |
|------------|--------|
| `R-MOV-01` | Una mossa **DEVE** consistere nella scelta di una **colonna** e mai di una cella. |
| `R-MOV-02` | La pedina **DEVE** occupare la cella vuota con l'**indice di riga più alto**, cioè la più in basso, nella colonna scelta. |
| `R-MOV-03` | Una mossa **DEVE** essere rifiutata se la colonna è fuori dall'intervallo valido (interno: `< 0` oppure `> 6`). |
| `R-MOV-04` | Una mossa **DEVE** essere rifiutata se la colonna è piena. |
| `R-MOV-05` | Una mossa rifiutata **NON DEVE** modificare la griglia né cambiare il turno. |
| `R-MOV-06` | Una mossa accettata **DEVE** comunicare la riga in cui la pedina è caduta, perché serve al controllo della vittoria (`R-END-03`). |

**Casi di prova** (diventeranno test automatici):

- Griglia vuota, mossa in colonna 3 → la pedina va in `(5, 3)`.
- Seconda mossa in colonna 3 → la pedina va in `(4, 3)`.
- 6 mosse in colonna 0 → la 7ª mossa in colonna 0 viene rifiutata e la griglia resta invariata.
- Mossa in colonna `-1` o `7` → rifiutata.

---

## 5. Regole del turno (`R-TRN`)

| ID         | Regola |
|------------|--------|
| `R-TRN-01` | La partita **DEVE** avere esattamente **2 giocatori**. |
| `R-TRN-02` | Il **Giocatore 1** **DEVE** muovere per primo. |
| `R-TRN-03` | Dopo ogni mossa **accettata** che non termina la partita, il turno **DEVE** passare all'altro giocatore. |
| `R-TRN-04` | Un giocatore **NON DEVE** poter passare il turno. Se esiste almeno una colonna non piena, deve muovere. |
| `R-TRN-05` | Ogni giocatore **DEVE** poter essere *umano* o *computer*, senza che le regole cambino. La logica di gioco non distingue i due tipi. |

---

## 6. Regole di fine partita (`R-END`)

| ID         | Regola |
|------------|--------|
| `R-END-01` | Un giocatore **vince** se, dopo una sua mossa, esiste un allineamento di **almeno 4** sue pedine in una di queste **4 direzioni**: |
|            | • orizzontale (←→) |
|            | • verticale (↑↓) |
|            | • diagonale discendente (↖↘) |
|            | • diagonale ascendente (↙↗) |
| `R-END-02` | Allineamenti di **più di 4** pedine (es. 5 in fila) **DEVONO** contare come vittoria. |
| `R-END-03` | Il controllo della vittoria **DEVE** considerare solo le linee che passano per l'**ultima mossa**, perché nessun'altra cella può aver creato un nuovo allineamento. |
| `R-END-04` | Se dopo una mossa **non** c'è vittoria e la griglia è **piena** (42 pedine), la partita **DEVE** terminare in **pareggio**. |
| `R-END-05` | La vittoria **DEVE** essere controllata **prima** del pareggio: una mossa che riempie la griglia e allinea 4 pedine è una vittoria. |
| `R-END-06` | A partita terminata **NON DEVE** essere accettata nessuna mossa. |
| `R-END-07` | Il conteggio dell'allineamento **NON DEVE** "uscire" dalla griglia: le celle fuori dai bordi non esistono e non vanno lette. |

**Casi di prova:**

- 4 pedine del G1 in `(5,0) (5,1) (5,2) (5,3)` → vince G1 (orizzontale).
- 4 pedine del G2 in `(5,6) (4,6) (3,6) (2,6)` → vince G2 (verticale).
- 4 pedine del G1 in `(5,0) (4,1) (3,2) (2,3)` → vince G1 (diagonale ascendente).
- 4 pedine del G1 in `(2,0) (3,1) (4,2) (5,3)` → vince G1 (diagonale discendente).
- Ultima mossa **in mezzo** alla linea, es. `(5,0) (5,1) _ (5,3)` e poi mossa in colonna 2 → vince. Il controllo deve guardare **in entrambi i versi**.
- 3 pedine in fila + 1 dell'avversario → **nessuna** vittoria.
- 4 pedine in fila ma "spezzate" dal bordo, es. `(5,5) (5,6)` e `(4,0) (4,1)` → **nessuna** vittoria: le righe non si collegano tra loro.
- Griglia piena senza allineamenti → pareggio.

---

## 7. Interazione con l'utente (`R-IO`)

Queste regole riguardano **solo** lo strato di interfaccia (console). La logica (`R-BRD`, `R-MOV`, `R-TRN`, `R-END`) non deve mai stampare né leggere input.

| ID        | Regola |
|-----------|--------|
| `R-IO-01` | La griglia **DEVE** essere mostrata dopo ogni mossa accettata. |
| `R-IO-02` | Simboli: cella vuota `.`, Giocatore 1 `X`, Giocatore 2 `O`. |
| `R-IO-03` | L'utente **DEVE** inserire un numero da **1 a 7**. |
| `R-IO-04` | Un input non numerico (es. `abc`) **NON DEVE** far bloccare o terminare il programma. Si mostra un messaggio d'errore e si richiede l'input. |
| `R-IO-05` | Una mossa rifiutata (`R-MOV-03`, `R-MOV-04`) **DEVE** produrre un messaggio che spiega il motivo, e poi si richiede l'input. |
| `R-IO-06` | A fine partita **DEVE** essere mostrato l'esito: chi ha vinto oppure il pareggio. |
| `R-IO-07` | A fine partita il programma **DOVREBBE** chiedere se giocarne un'altra. |

---

## 8. Giocatore computer (`R-AI`)

| ID        | Regola |
|-----------|--------|
| `R-AI-01` | Il computer **DEVE** scegliere sempre una mossa **valida**. |
| `R-AI-02` | **Livello 1:** il computer **DEVE** saper scegliere una colonna valida a caso. |
| `R-AI-03` | **Livello 2:** il computer **DOVREBBE** vincere subito se può e bloccare l'avversario se questo sta per vincere. |
| `R-AI-04` | **Livello 3:** il computer **DOVREBBE** usare l'algoritmo *minimax*, che esplora le mosse future assumendo che l'avversario giochi al meglio, con una profondità configurabile. |
| `R-AI-05` | Il computer **DEVE** usare le stesse regole del giocatore umano: niente mosse "speciali". |

---

## 9. Fuori dall'ambito (versione 1.0)

Queste cose **non** si fanno ora. Scriverle evita che il progetto cresca senza controllo:

- Interfaccia grafica: in futuro, per esempio, con la libreria SFML (Simple and Fast Multimedia Library).
- Gioco in rete.
- Salvataggio e caricamento della partita.
- Annullamento della mossa (*undo*).
- Griglie di dimensioni diverse da 6×7.
- Più di 2 giocatori.

---

## 10. Collegamento regole → codice

Indica dove ogni gruppo di regole verrà implementato. Si aggiorna man mano che il progetto cresce.

| Regole           | Classe responsabile             |
|------------------|---------------------------------|
| `R-BRD`, `R-MOV` | `Board`                         |
| `R-END`          | `Board` (controllo) + `Game` (stato della partita) |
| `R-TRN`          | `Game`                          |
| `R-IO`           | `ConsoleView`, `HumanPlayer`    |
| `R-AI`           | `AIPlayer` (derivata da `Player`) |
