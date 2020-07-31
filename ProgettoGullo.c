#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>



//_______STRUTTURA CONTATTO_______
struct contatto_s
{
    char nome[20];
    char cognome[20];
    char username[20];
    int idUtenteAgg;
    struct contatto_s *next;

};
typedef struct contatto_s contatto;



//_______STRUTTURA MESSAGGIO___________
struct messaggio_s
{
    char mittente[20];
    char destinatario[20];
    char conversazione[500];
    char data[30];
    int idUtenteMitt;
    int idUtenteDest;
    struct messaggio_s *next;
    
};
typedef struct messaggio_s messaggio;



//__________STRUTTURA UTENTE____________
struct utente
{
    char username[20];
    char password[20];
    char nome[20];
    char cognome[20];
    char email[30];
    int idUtente;
    contatto *contatti;
    messaggio *messaggi;
    messaggio *nuovoMess;
    struct utente *next;
};


//_____________PROTOTIPI FUNZIONI SALVATAGGI SU FILE__________
void salvaUtente(struct utente *headUtente);
void salvaNuovoContatto(contatto *nuovoContatto);
void salvaMessaggi(struct utente **utenteAttuale);
void salvaContattiCancellato(struct utente **headUtente,struct utente *utenteAttuale,char contattoCanc[]);
void salvaMessaggiCancellato(struct utente **headUtente, struct utente *utenteAttuale, char messCanc[]);
void salvaContattiCancellatoUtente(struct utente **headUtente,char utenteCanc[],int idUtenteEliminato,int nUtenti);


//____________PROTOTIPI FUNZIONI CARICAMENTO DA FILE__________
void inserisciUtenteF(struct utente **headUtente,char username[],char password[],char nome[],char cognome[],char email[],int ID);
void caricaUtente(struct utente **headUtente);
void inserisciContattoF(struct utente **headUtente,char nome[],char cognome[],char username[],int idUtenteAgg);
void caricaContatto(struct utente **utenteAttuale);
void inserisciMessaggioF(struct utente **headUtente,char mittente[],char destinatario[],char conversazione[],char data[],int idUtenteMitt,int idUtenteDest);
void caricaMessaggio(struct utente **utenteAttuale);

//_________PROTOTIPI FUNZIONI OPERAZIONI SU LISTE_________
struct utente *Registrazione(struct utente **headUtente);
struct utente *Login(struct utente *headUtente);
int eliminaUtente(struct utente **headUtente,struct utente *utenteAttuale);
contatto * aggiungiContattoUsername(struct utente **utenteAttuale,struct utente *headUtente);
contatto * aggiungiContattoNC(struct utente **utenteAttuale,struct utente *headUtente);
contatto * ricercaContattoUsername(struct utente *utenteAttuale,char *nickname);
contatto * ricercaContattoNC(struct utente *utenteAttuale);
int rimuoviContatto(struct utente **utenteAttuale,char *nickname);
int inviaMessaggio(struct utente **utenteAttuale,struct utente *headUtente);
void visualizzaChat(struct utente *utenteAttuale);
void cancellaMessaggio(struct utente **utenteAttuale, char messCanc[]);
void svuotaLista(struct utente **utenteAttuale);



//__________FUNZIONE SVUOTAMENTO LISTA MESSAGGI_________
void svuotaLista(struct utente **utenteAttuale)
{
     messaggio *m=NULL;
     while((*utenteAttuale)->messaggi!=NULL)
     {
         m=(*utenteAttuale)->messaggi->next;
         free((*utenteAttuale)->messaggi);
         (*utenteAttuale)->messaggi=m;
     }
}


//_____________SALVATAGGIO UTENTI SU FILE___________
void salvaUtente(struct utente *headUtente)
{
    //Dichiarazione ed apertura del file in scrittura per andare a scrivere gli utenti
    FILE *f;
    f=NULL;
    f=fopen("utenti.txt","w");

    //Se il file viene aperto correttamente
    if(f!=NULL)
    {
         //Ciclo che scorre la lista degli utenti e scrive ciascun utente nel file fino a quando non si raggiunge la fine della lista
         while(headUtente!=NULL)
         {
              fprintf(f,"%s %s %s %s %s %d\n",headUtente->username,headUtente->password,headUtente->nome,headUtente->cognome,headUtente->email,headUtente->idUtente);
              headUtente=headUtente->next;
         }
    }
    else
    {
         //Se il file non è aperto correttamente
         printf("Errore durante scrittura del file.\n");
         return;
    }
    //Chiusura del file
    fclose(f);
}


//_________SALVATAGGIO NUOVO CONTATTO SU FILE_________
void salvaNuovoContatto(contatto *nuovoContatto)
{
    //Dichiarazione ed apertura del file in scrittura per scrivere ciascun contatto che viene via via aggiunto alle liste contatti
    FILE *f1;
    f1=NULL;
    f1=fopen("contatti.txt","a");

    //Se il file viene aperto correttamente
    if(f1!=NULL)
    {
         //Scrittura del nuovo contatto sul file in modalità Append
         fprintf(f1,"%s %s %s %d\n",nuovoContatto->nome,nuovoContatto->cognome,nuovoContatto->username,nuovoContatto->idUtenteAgg);
    }
    else
    {
         //Se il file non è aperto correttamente
         printf("Errore durante scrittura del file.\n");
         return;
    }
    //Chiusura del file
    fclose(f1);

}


//_________SALVATAGGIO MESSAGGI SU FILE___________
void salvaMessaggi(struct utente **utenteAttuale)
{
    //Dichiarazione ed apertura del file in scrittura per scrivere ciascun messaggio che viene inviato
    FILE *f2;
    f2=NULL;
    f2=fopen("messaggi.txt","a");

    //Se il file viene aperto correttamente
    if(f2!=NULL)
    {
         //Conversione della data in modo tale da poter essere letta dal file
         time_t t = time(NULL);
         struct tm tm = *localtime(&t);
        
         //Scrittura del nuovo messaggio sul file in modalità append 
         fprintf(f2,"%s \n%s %s %d-%02d-%02d %d %d\n",(*utenteAttuale)->nuovoMess->conversazione,(*utenteAttuale)->nuovoMess->mittente,(*utenteAttuale)->nuovoMess->destinatario,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,(*utenteAttuale)->nuovoMess->idUtenteMitt,(*utenteAttuale)->nuovoMess->idUtenteDest);
         
         //Svuotamento delle liste dei messaggi
         free((*utenteAttuale)->nuovoMess);
         
         (*utenteAttuale)->nuovoMess=NULL;

          svuotaLista(utenteAttuale);
     }
    else
    {
         //Se il file non è aperto correttamente
         printf("Errore durante scrittura del file.\n");
         return;
    }
    //Chiusura del file
    fclose(f2);

}


//__________SALVATAGGIO CONTATTI SU FILE DOPO RIMOZIONE CONTATTO___________
void salvaContattiCancellato(struct utente **headUtente,struct utente *utenteAttuale,char contattoCanc[])
{
    //Dichiarazione variabili contatti
    char nome[20]="";
    char cognome[20]="";
    char username[20]="";
    int idUtenteAgg=0;
   
    //Apertura del file dei contatti in lettura
    FILE *f1;
    f1=NULL;
    f1=fopen("contatti.txt","r");
   
    //Se il file viene aperto corretamente
    if(f1!=NULL)
    {
         //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso, ed inserimento di ciascun contatto nella lista contatti
         while(fscanf(f1,"%s %s %s %d\n",(nome),(cognome),(username),(&idUtenteAgg))!=EOF)
         {
             inserisciContattoF(headUtente,nome,cognome,username,idUtenteAgg);
         }
    }
   else
   {
         //Se il file non viene aperto corretamente
         printf("\nFile aperto in modo errato.\n");
         return;
   }
   //Chiusura del file
   fclose(f1);

   //Riapertura del file dei contatti in scrittura, per riscrivere i contatti senza però il contatto eliminato dall'utente
   f1=NULL;
   f1=fopen("contatti.txt","w");

   //Se il file viene aperto corretamente
   if(f1!=NULL)
   {
         //Ciclo che scorre la lista dei contatti fino a quando non si raggiunge la fine
         while((*headUtente)->contatti!=NULL)
         {
             //Se scorrendo la lista dei contatti incontro il contatto che ho cancellato, e l'id di aggiunta corrisponde con il mio id, allora il contatto non viene aggiunto al file, altrimenti 
             //se si verifica solo una di queste condizioni allora il contatto viene aggiunto normalmente
             if(!((strcmp((*headUtente)->contatti->username,contattoCanc)==0) && (utenteAttuale->idUtente == (*headUtente)->contatti->idUtenteAgg)))
             {
                 fprintf(f1,"%s %s %s %d\n",(*headUtente)->contatti->nome,(*headUtente)->contatti->cognome,(*headUtente)->contatti->username,(*headUtente)->contatti->idUtenteAgg);
             }
             (*headUtente)->contatti= (*headUtente)->contatti->next;
         }
   }
   else
   {
         //Se il file non viene aperto corretamente
         printf("Errore durante scrittura del file.\n");
         return;
   }
   //Chiusura del file
   fclose(f1);
}


//__________SALVATAGGIO MESSAGGI SU FILE DOPO ELIMINAZIONE MESSAGGIO_________
void salvaMessaggiCancellato(struct utente **headUtente, struct utente *utenteAttuale, char messCanc[])
{
     //Dichiarazione variabili messaggi
     char mittente[20]="";
     char destinatario[20]="";
     char conversazione[500]="";
     char data[30]="";
     int idUtenteMitt=0;
     int idUtenteDest=0;
     int trovato=0;
     
     //Apertura del file dei messaggi in lettura
     FILE *f2;
     f2=NULL;
     f2=fopen("messaggi.txt","r");
     
     //Se il file viene aperto corretamente
     if(f2!=NULL)
     {
         //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso, ed inserimento di ciascun messaggio nella lista messaggi
         while(!feof(f2))
         {
             //Utilizzo della fgets per la lettura del testo del messaggio
             fgets(conversazione,sizeof(conversazione),f2);

             //Utilizzo della fscanf per la lettura delle credenziali del messaggio
             fscanf(f2,"%s %s %s %d %d\n",(mittente),(destinatario),(data),(&idUtenteMitt),(&idUtenteDest));
             
             //Richiamo della funzione di inserimento dei messaggi in lista messaggi
             inserisciMessaggioF(headUtente,mittente,destinatario,conversazione,data,idUtenteMitt,idUtenteDest);
         }
     }
     else
     {
         //Se il file non viene aperto corretamente
         printf("\nFile aperto in modo errato.\n");
         return;
     }
     //Chiusura del file
     fclose(f2);
    
     //Riapertura del file dei messaggi in scrittura, per riscrivere i messaggi senza però il messaggio eliminato dall'utente
     f2=NULL;
     f2=fopen("messaggi.txt","w");
    
     //Se il file viene aperto corretamente
     if(f2!=NULL)
     {
         //Ciclo che scorre la lista dei contatti fino a quando non si raggiunge la fine
         while((*headUtente)->messaggi!= NULL)
         {
             //Io voglio andare ad eliminare l'ultimo messaggio inviato o ricevuto con un determinato contatto, quindi tramite una variabile intera evito di eliminare tutti i messaggi
             //con quel contatto. Quando quindi il mittente o destinatario del messaggio corrisponde a quello indicato da me, il messaggio viene saltato dalla scrittura del file.
             //Questo accade solo una volta poichè appunto io voglio eliminare solo un messaggio e non l'intera conversazione con esso.
            if(
                (
                    ((strcmp((*headUtente)->messaggi->mittente,messCanc)==0) && ((*headUtente)->messaggi->idUtenteDest == utenteAttuale->idUtente))
                    ||
                    ((strcmp((*headUtente)->messaggi->destinatario,messCanc)==0)  && ((*headUtente)->messaggi->idUtenteMitt == utenteAttuale->idUtente))
                )
                &&  (!trovato)
              )
            {
                 trovato=1;
            }
            else
            {
                 //Se il mittente o destinatario del messaggio non corrisponde a quello da me indicato, oppure ho già eliminato un messaggio, allora il messaggio viene comunque scritto sul file
                 fprintf(f2,"%s \n%s %s %s %d %d\n",(*headUtente)->messaggi->conversazione,(*headUtente)->messaggi->mittente,(*headUtente)->messaggi->destinatario,(*headUtente)->messaggi->data,(*headUtente)->messaggi->idUtenteMitt,(*headUtente)->messaggi->idUtenteDest);
            }
            //Passaggio al messaggio successivo
            (*headUtente)->messaggi=(*headUtente)->messaggi->next;
        }
     }
     else
     {
         //Se il file non viene aperto corretamente
         printf("Errore durante scrittura del file.\n");
         return;
     }
     //Chiusura del file
     fclose(f2);
}


//___________SALVATAGGIO CONTATTI SU FILE DOPO ELIMINAZIONE ACCOUNT___________
void salvaContattiCancellatoUtente(struct utente **headUtente,char utenteCanc[],int idUtenteEliminato,int nUtenti)
{
     //Dichiarazione variabili contatti
     char nome[20]="";
     char cognome[20]="";
     char username[20]="";
     int idUtenteAgg=0;
     
     FILE *f1;
     f1=NULL;

     //Se non è più presente alcun utente all'interno della lista utenti, il file dei contatti viene svuotato con una semplice apertura in w e chiusura
     if(nUtenti==0)
     {
         
         f1=fopen("contatti.txt","w");
         
         //Se il file viene aperto corretamente
         if(f1!=NULL)
         {
         }
         else
         {
              //Se il file non viene aperto corretamente
             printf("\nFile aperto in modo errato.\n");
             return;
         }
         
         //Chiusura del file
         fclose(f1);
     }
     else
     {
        //Nella lista utenti è presente almeno un utente, e dunque il file dei contatti viene aperto prima in lettura per inserire in lista dei contatti quelli rimanenti, e poi in scrittura per salvare questa lista aggiornata sul file
         f1=fopen("contatti.txt","r");
         
         //Se il file viene aperto corretamente
         if(f1!=NULL)
         {
             //Se la lista dei contatti non è vuota
             if((*headUtente)->contatti!=NULL)
             {
                 //Svuotamento della lista dei contatti  
                 (*headUtente)->contatti=NULL;
         
                 //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso
                 while(fscanf(f1,"%s %s %s %d\n",(nome),(cognome),(username),(&idUtenteAgg))!=EOF)
                 {
                     //Se l'username del contatto da aggiungere corrisponde a quello dell'utente che ha eliminato il suo account oppure se incontriamo un contatto aggiunto da esso, 
                     //viene saltato l'inserimento di quel contatto.
                     if((strcmp(username,utenteCanc)==0 || (idUtenteEliminato == idUtenteAgg)))
                     {
                     }
                     else
                     {
                         //Se il contatto ha un'altro username e non è stato aggiunto dall'utente che ha eliminato il suo account, esso viene aggiunto alla lista dei contatti
                         inserisciContattoF(headUtente,nome,cognome,username,idUtenteAgg);
                     }
                 }
             }
             else
             {
                 //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso
                 while(fscanf(f1,"%s %s %s %d\n",(nome),(cognome),(username),(&idUtenteAgg))!=EOF)
                 {
                     //Se l'username del contatto da aggiungere corrisponde a quello dell'utente che ha eliminato il suo account,viene saltato l'inserimento di quel contatto.
                     if((strcmp(username,utenteCanc)==0))
                     {
                     }
                     else
                     {
                         //Se il contatto ha un'altro username, esso viene aggiunto alla lista dei contatti
                         inserisciContattoF(headUtente,nome,cognome,username,idUtenteAgg);
                     }
                 }
         
             }
         }
         else
         {
             //Se il file non viene aperto corretamente
             printf("\nFile aperto in modo errato.\n");
             return;
         }
         //Chiusura del file
         fclose(f1);

         
         
         //Riapertura del file dei contatti in scrittura, per riscrivere i contatti senza però quelli aggiunti dall'utente eliminato e quelli che coinvolgono esso
         f1=NULL;
         f1=fopen("contatti.txt","w");
      
         //Se il file viene aperto corretamente
         if(f1!=NULL)
         {
             //Se la lista dei contatti non è vuota
             if((*headUtente)->contatti!=NULL)
             {
                 //Ciclo che scorre la lista dei contatti fino a quando non si raggiunge la fine
                 while((*headUtente)->contatti!=NULL)
                 {
                     //Scrittura sul file di tutti i contatti precedentemente letti in maniera corretta
                     fprintf(f1,"%s %s %s %d\n",(*headUtente)->contatti->nome,(*headUtente)->contatti->cognome,(*headUtente)->contatti->username,(*headUtente)->contatti->idUtenteAgg);
                     (*headUtente)->contatti=(*headUtente)->contatti->next;
                 }
             }
         }
         else
         {
             //Se il file non viene aperto corretamente
             printf("Errore durante scrittura del file.\n");
             return;
         }
         //Chiusura del file
         fclose(f1);
     }
}


//____________CARICAMENTO UTENTI DA FILE________
void inserisciUtenteF(struct utente **headUtente,char username[],char password[],char nome[],char cognome[],char email[],int ID)
{
     //Creazione nodo dell'utente
     struct utente *u=NULL;
     u=(struct utente *)malloc(sizeof(struct utente));

     //Copia delle credenziali all'interno del nodo utente
     strcpy(u->username,username);
	 strcpy(u->password,password);
     strcpy(u->cognome,cognome);
	 strcpy(u->nome,nome);
	 strcpy(u->email,email);
     u->idUtente=ID;
    
    //Inizializzazione a NULL delle liste messaggi e contatti dell'utente 
     u->contatti=NULL;
     u->messaggi=NULL;

     //Inserimento dell'utente in testa alla lista utenti
     u->next=(*headUtente);
     (*headUtente)=u;

     return;
}


void caricaUtente(struct utente **headUtente)
{
     //Dichiarazione ed apertura del file in lettura per andare a leggere gli utenti
     FILE *f;
     f=NULL;
     f=fopen("utenti.txt","r");

     //Dichiarazione variabili utenti
     char username[20]="";
     char password[20]="";
     char nome[20]="";
     char cognome[20]="";
     char email[30]="";
     int ID=0;

     //Se il file viene aperto correttamente
     if(f!=NULL)
     {
         //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso, ed inserimento di ciascun utente nella lista utenti
        while(fscanf(f,"%s %s %s %s %s %d\n",(username),(password),(nome),(cognome),(email),(&ID))!=EOF)
         {
             inserisciUtenteF(headUtente,username,password,nome,cognome,email,ID);
         }
     }
     else
     {
         //Se il file non viene aperto correttamente
         printf("\nFile aperto in modo errato.\n");
         return;
     }
     //Chiusura del file
     fclose(f);
}


//_________CARICAMENTO CONTATTI DA FILE__________
void inserisciContattoF(struct utente **headUtente,char nome[],char cognome[],char username[], int idUtenteAgg)
{
     //Creazione nodo del contatto
     contatto *c=NULL;
     c=(contatto *)malloc(sizeof(contatto));
    
     //Copia delle credenziali all'interno del nodo contatto
     strcpy(c->nome,nome);
     strcpy(c->cognome,cognome);
	 strcpy(c->username,username);
	 c->idUtenteAgg=idUtenteAgg;

     //Inserimento del contatto in testa alla lista contatti
     c->next=(*headUtente)->contatti;
     (*headUtente)->contatti=c;

     return;
}

void caricaContatto(struct utente **utenteAttuale)
{
     //Dichiarazione ed apertura del file in lettura per andare a leggere i contatti
     FILE *f1;
     f1=NULL;
     f1=fopen("contatti.txt","r");

     //Dichiarazione variabili contatti
     char nome[20]="";
     char cognome[20]="";
     char username[20]="";
     int idUtenteAgg=0;

     //Se il file viene aperto correttamente
     if(f1!=NULL)
     {
         //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso
         while(fscanf(f1,"%s %s %s %d\n",(nome),(cognome),(username),(&idUtenteAgg))!=EOF)
         {
             //Se l'id del contatto aggiunto corrisponde con quello dell'utente attuale, e quindi il contatto è stato aggiunto dall'utente attuale, esso viene aggiunto alla lista contatti dell'utente attuale
             if(idUtenteAgg == (*utenteAttuale)->idUtente)
             {
                 inserisciContattoF(utenteAttuale,nome,cognome,username,idUtenteAgg);
             }
         }
     }
     else
     {
         //Se il file non viene aperto correttamente
         printf("\nFile aperto in modo errato.\n");
         return;
     }
     //Chiusura del file
     fclose(f1);
}


//_________CARICAMENTO MESSAGGI DA FILE_________
void inserisciMessaggioF(struct utente **headUtente,char mittente[],char destinatario[],char conversazione[],char data[],int idUtenteMitt,int idUtenteDest)
{
     //Creazione nodo del messaggio
     messaggio *m=NULL;
     m=(messaggio*)malloc(sizeof(messaggio));

     //Copia delle credenziali all'interno del nodo messaggio
     strcpy(m->mittente,mittente);
     strcpy(m->destinatario,destinatario);
     strcpy(m->conversazione,conversazione);
     strcpy(m->data,data);
     m->idUtenteMitt=idUtenteMitt;
     m->idUtenteDest=idUtenteDest;

     //Inserimento del messaggio in testa alla lista messaggi
     m->next=(*headUtente)->messaggi;
     (*headUtente)->messaggi=m;

     return;
}

void caricaMessaggio(struct utente **utenteAttuale)
{
     //Dichiarazione ed apertura del file in lettura per andare a leggere i messaggi
     FILE *f2;
     f2=NULL;
     f2=fopen("messaggi.txt","r");

     //Dichiarazione variabili contatti
     char mittente[20]="";
     char destinatario[20]="";
     char conversazione[500]="";
     char data[30];
     int idUtenteMitt=0;
     int idUtenteDest=0;

     //Se il file viene aperto correttamente
     if(f2!=NULL)
     {
         //Ciclo che scorre il file fino a quando non si raggiunge la fine di esso
         while(!feof(f2))
         {
             //Utilizzo della fgets per la lettura del testo del messaggio
             fgets(conversazione,sizeof(conversazione),f2);

             //Utilizzo della fscanf per la lettura delle credenziali del messaggio
             fscanf(f2,"%s %s %s %d %d\n",(mittente),(destinatario),(data),(&idUtenteMitt),(&idUtenteDest));
             
             //Se l'id del mittente o del destinatario corrispondono all'id dell'utente attuale, il messaggio viene aggiunto alla lista dei messaggi dell'utente attuale
             if((idUtenteMitt == (*utenteAttuale)->idUtente) || (idUtenteDest==(*utenteAttuale)->idUtente))
             {
                 inserisciMessaggioF(utenteAttuale,mittente,destinatario,conversazione,data,idUtenteMitt,idUtenteDest);
             }
         }
     }
     else
     {
         //Se il file non viene aperto correttamente
        printf("\nFile aperto in modo errato.\n");
        return;
     }
     //Chiusura del file
     fclose(f2);
}






//___________REGISTRAZIONE NUOVO UTENTE___________
struct utente *Registrazione(struct utente **headUtente)
{
     char nickname[20]="";
     char password[20]="";
     char nome[20]="";
     char cognome[20]="";
     char email[30]="";
     int idUtente=0;

     //Creazione di una variabile temporanea per scorrere la lista Utenti
     struct utente *temp1=NULL;
  
     //La variabile temporanea prende il valore iniziale della testa della lista Utenti
     temp1=(*headUtente);

     //Creazione di 2 variabili intere per verificare se l'username esiste già in lista utenti
     int usato=0;
     int a=0;

   

     //Se c'è almeno un utente già registrato
     if(temp1!=NULL)
     {
         
         //Ciclo while che richiede all'utente di inserire un username fino a quando non ne viene inserito uno non esistente
         while(usato==0)
         {
             //Inserimento username scelto dall'utente
             printf("\nInserisci il tuo username:");
             fflush(stdin);
             scanf("%s",nickname);
       
             a=0;
             temp1=(*headUtente);
       
             //Finchè non si raggiunge la fine della lista e finchè non si trova un username uguale a quello inserito dall'utente, la variabile temporanea scorre la lista utenti
             while((temp1!=NULL)&&(a==0))
             {
                 if(strcmp(temp1->username,nickname)==0)
                 {
                     a=1;
                 }
                 else
                 { 
                     temp1=temp1->next;
                 }
             }

             //Uscita dal ciclo while.Verifica se è stato trovato un username uguale a quello richiesto dall'utente 
             if(a==1)
             {
                 printf("\nUsername già utilizzato.Scegline un'altro.\n");
             }
             else
             {
                 //Username non trovato in lista, proseguimento dell'inserimento del nuovo utente. La variabile di verifica diventa 1 e quindi si esce dal ciclo
                 usato=1;
                 printf("\nOk username disponibile.\n");
             }
        
        }
     }
     else
     {
         //Inserimento username scelto dall'utente
         printf("\nInserisci il tuo username:");
         fflush(stdin);
         scanf("%s",nickname);  
     }

     //Inserimento delle successive credenziali del nuovo utente
     fflush(stdin);
     printf("\nInserisci la tua password:");
     fflush(stdin);
     scanf("%s",password);
     fflush(stdin);
     printf("\nInserisci il tuo nome:");
     scanf("%s",nome);
     fflush(stdin);
     printf("\nInserisci il tuo cognome:");
     scanf("%s",cognome);
     fflush(stdin);
     printf("\nInserisci la tua email:");
     scanf("%s",email);
     fflush(stdin);
    
     //Generazione di un idUtente random tra 1 e 100
     srand(time(NULL));   
     idUtente=rand()% 100+1;
     printf("\nIl tuo id è:%d\n",idUtente);
 
     //Richiamo della funzione di inserimento di un utente in testa alla lista degli utenti
     inserisciUtenteF(headUtente,nickname,password,nome,cognome,email,idUtente);
     printf("\nUtente inserito.\n");
     return (*headUtente);
}



//___________LOGIN UTENTE GIA' REGISTRATO_______
struct utente *Login(struct utente *headUtente)
{
    //Dichiarazione variabili per verificare il login
     int trovato=0;
     int a=0;
     char nickname[20]="";
     char password[20]="";
 
     //Dichiarazione variabile di appoggio per scorrere la lista degli utenti nel caso essa non sia vuota
     struct utente *temp1=NULL;
     
     //Ciclo di inserimento di un username finchè non si trova un utente esistente
     do
     {
         printf("\nInserisci il tuo username:");
         fflush(stdin);
         scanf("%s",nickname);
       
         //Creazione di una variabile temporanea per scorrere la lista Utenti
         temp1=headUtente;

         //Scorrimento della lista per trovare l'username 
         //Finchè non si raggiunge la fine della lista e finchè non si trova un username uguale a quello inserito dall'utente, la variabile temporanea scorre la lista utenti
         while((temp1!=NULL)&&(a==0))
         {
             if(strcmp(temp1->username,nickname)==0)
             {
                 a=1;
             }
             else
             {
                 temp1=temp1->next;
             }
         }

         //Uscita dal ciclo while.Verifica se è stato trovato un username uguale a quello dell'utente 
         if(a==1)
         {
             printf("\nUtente trovato.\n");
             //La variabile di verifica diventa 1 e dunque l'utente è stato trovato nella lista
             trovato=1;
         }
         else
         {
             printf("\nUsername non trovato in lista.Inseriscine uno esistente.\n");
         }

     }while(trovato==0);

     //Inizializzazione della variabile di verifica a 0 per l'inserimento della password
     trovato=0;

     //Ciclo di inserimento della password finchè non viene inserita uguale a quella dell'utente 
     do
     {    
         //Inserimento della password 
          printf("\nInserisci password:");
          fflush(stdin);
          scanf("%s",password);

          //Se la password inserita è corretta, la variabile di verifica diventa 1 e si esce dal ciclo accedendo al programma
         if(strcmp(temp1->password,password)==0)
         {
             printf("\nPassword corretta.Accesso in corso.\n");
             trovato=1;
         }
         else
         {
             //Se la password inserita è errata l'utente viene invitato ad inserirla di nuovo
             printf("\nPassword errata.Riprova.\n");
         }

     }while(trovato==0);

 return temp1;
}



//_____________ELIMINAZIONE UTENTE__________
int eliminaUtente(struct utente **headUtente,struct utente *utenteAttuale)
{
     //Creazione variabile per scorrere la lista degli utenti ed eliminare l'utente dalla lista contatti di ciascun utente
     struct utente *u=NULL;
     u=(*headUtente);
     
     //Creazione variabile intera per verificare se il contatto viene eliminato dalla lista contatti di ciascun utente
     int eliminato=0;
   
     //Ciclo che scorre la lista utenti ed esce quando si raggiunge la fine
     while(u!=NULL)
     {
         eliminato=0;
         //Se ci troviamo nell'utente che deve essere cancellato, automaticamente si passa al successivo
         if(strcmp(u->username,utenteAttuale->username)==0)
         {
             u=u->next;
         }
         else
         { 
             //Verifica se l'utente che sta eliminando il suo account si trova in lista contatti dell'utente attuale, se viene trovato viene rimosso
             eliminato=rimuoviContatto(&u,utenteAttuale->username);
             u=u->next;
         }
     }

     //Verifica se l'utente da eliminare si trova in testa alla lista utenti
     if(utenteAttuale==(*headUtente))
     {
         (*headUtente)=(*headUtente)->next;
     }
     else
     {
         //L'utente da eliminare si trova in mezzo alla lista utenti oppure in coda
         //Creazione di una variabile per scorrere la lista degli utenti ed individuare l'utente precedente all'utente da eliminare
         struct utente *utentePrec;
         utentePrec=(*headUtente);
      
         //Ciclo che scorre la lista utenti fino a quando l'utente successivo corrisponde all'utente da eliminare
         while(utentePrec->next!=utenteAttuale)
         {
             utentePrec=utentePrec->next;
         }
      
         //Agganciamento dell'utente precedente con l'utente successivo a quello da eliminare
         utentePrec->next=utenteAttuale->next;
     }
     
     //Creazione variabile intera per ottenere l'id dell'utente eliminato
     int idUtenteEliminato=0;
     idUtenteEliminato=utenteAttuale->idUtente;

     //Eliminazione utente
     free(utenteAttuale);
     return idUtenteEliminato;
}


//_____________AGGIUNGI CONTATTO TRAMITE RICERCA PER USERNAME___________
contatto * aggiungiContattoUsername(struct utente **utenteAttuale,struct utente *headUtente)
{
     char nome[20]="";
     char cognome[20]="";
     char nickname[20]="";
    
     //Inserimento username dell'utente da aggiungere ai contatti
     printf("\nInserisci l'username dell'utente che vuoi aggiungere ai tuoi contatti:");
     fflush(stdin);
     scanf("%s",nickname);
  
     //Se la lista dei contatti non è vuota, viene verificato se il contatto è già presente  
     if((*utenteAttuale)->contatti!=NULL)
     {
         //Creazione variabile temporanea per scorrere la lista contatti 
         contatto *cerca=NULL;
         
         //Variabile temporanea assume il valore della testa della lista contatti
         cerca=(*utenteAttuale)->contatti;
         
         //Scorrimento della lista contatti fino a quando non si raggiunge la fine della lista oppure si trova un contatto con l'username ricercato
         while((cerca->next!=NULL)&&(strcmp(cerca->username,nickname)!=0))
         {
             cerca=cerca->next;
         }
         
         //Uscita dal while e verifica se è stato trovato il contatto ricercato nella lista contatti
         if(strcmp(cerca->username,nickname)==0)
         {
             printf("\n\nL'utente si trova già nella tua lista contatti.\n");
             return cerca;
         }
         else
         {
             //Il contatto non è stato trovato in lista.Creazione di una variabile temporanea per scorrere la lista utenti e trovare l'utente da aggiungere ai contatti
             struct utente *cercaUtente;
             cercaUtente=headUtente;

             //Ciclo che scorre la lista degli utenti ed esce se si raggiunge la fine della lista o si trova un utente con l'username ricercato
             while((cercaUtente->next!=NULL)&&(strcmp(cercaUtente->username,nickname)!=0))
             {
                 cercaUtente=cercaUtente->next;
             }
          
             //Verifica se è stato trovato l'utente tramite l'username inserito
             if(strcmp(cercaUtente->username,nickname)==0)
             {
                 printf("\nUtente trovato.Creazione contatto\n");
           
                 
                 //Creazione contatto da aggiungere alla lista contatti
                 contatto *nuovoContatto;
                 nuovoContatto=(contatto*)malloc(sizeof(contatto));
             
                 //Copia credenziali dell'utente nel contatto
		         strcpy(nuovoContatto->username,nickname);
                 printf("Username:%s\n",nuovoContatto->username);
		      
                 strcpy(nuovoContatto->cognome,cercaUtente->cognome);
                 printf("Cognome:%s\n",nuovoContatto->cognome);

	             strcpy(nuovoContatto->nome,cercaUtente->nome);
                 printf("Nome:%s\n",nuovoContatto->nome);

	             nuovoContatto->idUtenteAgg=(*utenteAttuale)->idUtente;
                 printf("idUtenteAgg:%d\n",(*utenteAttuale)->idUtente);
               
                 //La lista dei contatti non è vuota, quindi il contatto andrà inserito in coda alla lista contatti
                 //Creazione variabile per raggiungere la coda della lista contatti
                 contatto *cercaCoda;
                 cercaCoda=(*utenteAttuale)->contatti;
                 
                 //Ciclo che scorre la lista dei contatti fino a quando non si raggiunge la coda 
                 while(cercaCoda->next!=NULL)
                  {
                     cercaCoda=cercaCoda->next;
                  }

                  //Inserimento del contatto in coda alla lista contatti
                  nuovoContatto->next=NULL;
                  cercaCoda->next=nuovoContatto;
                  printf("\n\nUtente aggiunto alla tua lista contatti!.\n");
                  return nuovoContatto;
             }
             else
             {
                 //L'utente non è stato trovato nella lista utenti e quindi non è possibile aggiungerlo ai contatti
                 printf("\nL'utente che stai cercando non esiste.\n");
                 return 0;
             }
         }
     }
     else 
     {
         //Se la lista dei contatti è vuota si passa direttamente alla ricerca dell'utente da aggiungere ai contatti
          
         //Creazione variabile temporanea per scorrere la lista utenti e cercare l'utente da aggiungere ai contatti
         struct utente *cercaUtente;
         cercaUtente=headUtente;
              
         //Scorrimento della lista utenti fino a quanto non si raggiunge la fine della lista oppure si trova un utente con l'username ricercato 
         while((cercaUtente->next!=NULL)&&(strcmp(cercaUtente->username,nickname)!=0))
         {
             cercaUtente=cercaUtente->next;
         }
           
         //Verifica se l'utente che si vuole aggiungere alla lista dei contatti è stato trovato tramite il suo username
         if(strcmp(cercaUtente->username,nickname)==0)
         {
             printf("\nUtente trovato.Creazione contatto\n");
           
             //Creazione contatto da aggiungere alla lista contatti
             contatto *nuovoContatto;
             nuovoContatto=(contatto*)malloc(sizeof(contatto));
              
             //Copia credenziali dell'utente nel contatto
		     strcpy(nuovoContatto->username,nickname);
             printf("Username:%s\n",nuovoContatto->username);
		      
             strcpy(nuovoContatto->cognome,cercaUtente->cognome);
             printf("Cognome:%s\n",nuovoContatto->cognome);

	         strcpy(nuovoContatto->nome,cercaUtente->nome);
             printf("Nome:%s\n",nuovoContatto->nome);

             nuovoContatto->idUtenteAgg=(*utenteAttuale)->idUtente;
             printf("idUtenteAgg:%d\n",(*utenteAttuale)->idUtente);
		      
             //Ramo di inserimento in testa alla lista contatti poichè essa è vuota
             nuovoContatto->next=(*utenteAttuale)->contatti;
             (*utenteAttuale)->contatti=nuovoContatto;
             return nuovoContatto;
         }
         else
         {
             //L'utente non è stato trovato in lista utenti
             printf("\n\nL'utente che stai cercando non esiste.\n");
             return 0;
         }   
     }
}
        
      
//_________AGGIUNGI CONTATTO TRAMITE RICERCA PER NOME E COGNOME________
contatto * aggiungiContattoNC(struct utente **utenteAttuale,struct utente *headUtente)
{
     char nome[20]="";
     char cognome[20]="";
     char nickname[20]="";

     //Inserimento del nome dell'utente da aggiungere ai contatti
     printf("\nInserisci il nome dell'utente che vuoi aggiungere ai tuoi contatti:");
     fflush(stdin);
     scanf("%s",nome);

     //Inserimento del cognome dell'utente da aggiungere ai contatti
     printf("\nInserisci il cognome dell'utente che vuoi aggiungere ai tuoi contatti:");
     fflush(stdin);
     scanf("%s",cognome);

    
     //Se la lista dei contatti non è vuota viene effettuata una ricerca per verificare se l'utente è già in lista contatti 
     if((*utenteAttuale)->contatti!=NULL)
     {
         //Creazione variabile temporanea per scorrere la lista contatti 
         contatto *cercaC;
         cercaC=NULL;
         cercaC=(*utenteAttuale)->contatti;
             
         //Scorrimento della lista contatti fino a quando non si raggiunge la fine della lista oppure si trova un contatto con nome e cognome inseriti
         while((cercaC->next!=NULL)&&(strcmp(cercaC->nome,nome)!=0)&&(strcmp(cercaC->cognome,cognome)!=0))
         {
             cercaC=cercaC->next;
         }
       
         //Uscita dal while e verifica se è stato trovato il contatto ricercato nella lista contatti
         if((strcmp(cercaC->nome,nome)==0)&&(strcmp(cercaC->cognome,cognome)==0))
         {
             //Contatto già presente in lista contatti, ritorno al menù
             printf("\n\nL'utente si trova già nella tua lista contatti.\n");
             return cercaC;
         }
         else
         {
             //Utente non presente in lista contatti.Creazione variabile temporanea per scorrere la lista degli utenti e trovare l'utente con nome e cognome inseriti
             struct utente *cercaUtente;
             cercaUtente=headUtente;

             //Ciclo che scorre la lista utenti ed esce se viene raggiunta la fine della lista oppure si trova un utente con nome e cognome inseriti
             while((cercaUtente->next!=NULL)&&(strcmp(cercaUtente->nome,nome)!=0)&&(strcmp(cercaUtente->cognome,cognome)!=0))
             {
                 cercaUtente=cercaUtente->next;
             }
                
             //Verifica se è stato trovato l'utente con nome e cognome inseriti
             if((strcmp(cercaUtente->nome,nome)==0)&&(strcmp(cercaUtente->cognome,cognome)==0))
                  {
                     printf("\nUtente trovato.Creazione contatto\n");
           
                     //Creazione contatto da aggiungere alla lista contatti
                     contatto *nuovoContatto;
                     nuovoContatto=(contatto*)malloc(sizeof(contatto));
             
                     //Copia credenziali dell'utente nel contatto
		             strcpy(nuovoContatto->username,cercaUtente->username);
                     printf("Username:%s\n",nuovoContatto->username);
		      
                     strcpy(nuovoContatto->cognome,cercaUtente->cognome);
                     printf("Cognome:%s\n",nuovoContatto->cognome);

	                 strcpy(nuovoContatto->nome,cercaUtente->nome);
                     printf("Nome:%s\n",nuovoContatto->nome);
           
                    
                     //La lista dei contatti non è vuota, quindi il nuovo contatto verrà inserito in coda alla lista contatti
                     //Creazione variabile temporanea per raggiungere la coda della lista contatti
                     contatto *cercaCoda;
                     cercaCoda=(*utenteAttuale)->contatti;
                      
                     //Ciclo che scorre la lista dei contatti fino a quando non si raggiunge la coda
                     while(cercaCoda->next!=NULL)
                     {
                         cercaCoda=cercaCoda->next;
                     }
                    
                     //Inserimento del nuovo contatto in coda alla lista contatti
                     nuovoContatto->next=NULL;
                     cercaCoda->next=nuovoContatto;
                     printf("\n\nUtente aggiunto alla tua lista contatti!\n");

                     return nuovoContatto;
                    }
            }
     }
     else
     {
         //Se la lista dei contatti è vuota si passa direttamente alla ricerca dell'utente da aggiungere alla lista 
         //Creazione variabile temporanea per scorrere la lista utenti e cercare l'utente da aggiungere ai contatti
         struct utente *cercaUtente;
         cercaUtente=headUtente;
              
         //Ciclo che scorre la lista utenti fino a quanto non si raggiunge la fine oppure si trova un utente con nome e cognome inseriti
         while((cercaUtente->next!=NULL)&&(strcmp(cercaUtente->nome,nome)!=0)&&(strcmp(cercaUtente->cognome,cognome)!=0))
         {
             cercaUtente=cercaUtente->next;
         }
           
         
         //Verifica se l'utente che si vuole aggiungere alla lista dei contatti è stato trovato tramite il suo nome e cognome
         if(strcmp(cercaUtente->nome,nome)==0)
         {
             if(strcmp(cercaUtente->cognome,cognome)==0)
             {
                 printf("\nUtente trovato.Creazione contatto\n");
           
                 //Creazione contatto da aggiungere alla lista contatti
                 contatto *nuovoContatto;
                 nuovoContatto=(contatto*)malloc(sizeof(contatto));
              
                 //Copia credenziali dell'utente nel contatto
		         strcpy(nuovoContatto->username,cercaUtente->username);
                 printf("Username:%s\n",nuovoContatto->username);
		      
                 strcpy(nuovoContatto->cognome,cercaUtente->cognome);
                 printf("Cognome:%s\n",nuovoContatto->cognome);

	             strcpy(nuovoContatto->nome,cercaUtente->nome);
                 printf("Nome:%s\n",nuovoContatto->nome);
		      
                 //Inserimento in testa alla lista contatti poichè essa è vuota
                 nuovoContatto->next=(*utenteAttuale)->contatti;
                 (*utenteAttuale)->contatti=nuovoContatto;
                   
                 printf("\n\nUtente aggiunto alla tua lista contatti!\n");

                 return nuovoContatto;
             }             
              
         }
         else
         {
             //L'utente ricercato non esiste in lista utenti
             printf("\n\nL'utente che stai cercando non esiste.\n");
             return 0;
         }
     }
     return 0;
}
 


//_______________RICERCA CONTATTO USERNAME______________
contatto * ricercaContattoUsername(struct utente *utenteAttuale,char *nickname)
{
     //Creazione variabile temporanea per scorrere la lista contatti 
     contatto *cerca=NULL;
     cerca=utenteAttuale->contatti;
         
     //Ciclo while che scorre la lista contatti fino a quando non si raggiunge la fine della lista oppure si trova un contatto con l'username ricercato
     while((cerca->next!=NULL)&&(strcmp(cerca->username,nickname)!=0))
     {
         cerca=cerca->next;
     }

     //Uscita dal while e verifica se è stato trovato il contatto ricercato nella lista contatti tramite username
     if(strcmp(cerca->username,nickname)==0)
     {
         //Visualizzazione del contatto trovato
         printf("\n\nIl contatto è stato trovato.\n");
         printf("\nUsername:%s\n",cerca->username);
         printf("\nNome:%s\n",cerca->nome);
         printf("\nCognome:%s\n",cerca->cognome);

         return cerca;
     }
     else
     {
         //Il contatto ricercato non è presente in lista
         return 0;
     }
}



//_________RICERCA CONTATTO NOME COGNOME________
contatto * ricercaContattoNC(struct utente *utenteAttuale)
{
     char nome[20]="";
     char cognome[20]="";
  
     //Ricerca del contatto tramite nome e cognome
     printf("\nInserisci il nome dell'utente che vuoi ricercare nei tuoi contatti:");
     fflush(stdin);
     scanf("%s",nome);

     printf("\nInserisci il cognome dell'utente che vuoi ricercare nei tuoi contatti:");
     fflush(stdin);
     scanf("%s",cognome);

     //Creazione variabile temporanea per scorrere la lista contatti 
     contatto *cerca;
     cerca=utenteAttuale->contatti;
       
     //Ciclo while che scorre la lista contatti fino a quando non si raggiunge la fine della lista oppure si trova un contatto con nome e cognome inseriti
     while((cerca->next!=NULL)&&(strcmp(cerca->nome,nome)!=0)&&(strcmp(cerca->cognome,cognome)!=0))
     {
         cerca=cerca->next;
     }

     //Uscita dal while e verifica se è stato trovato il contatto ricercato nella lista contatti tramite nome e cognome
     if((strcmp(cerca->nome,nome)==0)&&(strcmp(cerca->cognome,cognome)==0))
     {
         //Visualizzazione del contatto trovato
         printf("\n\nIl contatto è stato trovato.\n");
         printf("\nUsername:%s\n",cerca->username);
         printf("\nNome:%s\n",cerca->nome);
         printf("\nCognome:%s\n",cerca->cognome);

         return cerca;
     }
     else
     {
         //Il contatto non viene trovato in lista contatti
         return 0;
     }
}

   




//___________RIMUOVI CONTATTO___________
int rimuoviContatto(struct utente **utenteAttuale,char *nickname)
{
     //Creazione variabile temporanea per scorrere la lista contatti 
     contatto *cerca=NULL;
     cerca=(*utenteAttuale)->contatti;
      
     //Creazione variabile temporanea per trovare il contatto precedente a quello da eliminare per agganciarlo al successivo 
     contatto *Cprec=NULL;
     Cprec=(*utenteAttuale)->contatti;
    
     //Ciclo while che scorre la lista contatti fino a quando non si raggiunge la fine della lista oppure si trova un contatto con l'username ricercato
     while((cerca!=NULL)&&(strcmp(cerca->username,nickname)!=0))
     {
         Cprec=cerca;
         cerca=cerca->next;
     }
     //Uscita dal while e verifica se è stato trovato il contatto ricercato nella lista contatti
     if(cerca==NULL)
     {
         //Contatto non trovato, ritorno al menù del valore 0 (nessun contatto rimosso)
         printf("\nContatto %s non presente in lista contatti di %s.",nickname,(*utenteAttuale)->username);
         return 0;
     }
      
     //Se il contatto da eliminare si trova in testa alla lista contatti
     if(cerca==(*utenteAttuale)->contatti)
     {
         (*utenteAttuale)->contatti=(*utenteAttuale)->contatti->next;
     }
     else
     {
         //Se il contatto da eliminare non si trova in testa alla lista contatti
         Cprec->next=cerca->next;
     }
     //Rimozione del contatto dalla lista contatti
     printf("\nRimozione del contatto %s dalla lista contatti di %s.",nickname,(*utenteAttuale)->username);
     free(cerca);
      
     //Ritorno al menù del valore 1 (contatto rimosso)
     return 1;
      
}
 




//______________INVIA MESSAGGIO___________
int inviaMessaggio(struct utente **utenteAttuale,struct utente *headUtente)
{
     char mittente[20]="";
     char destinatario[20]="";
     char conversazione[500]="";
     int idUtenteMitt=0;
     int idUtenteDest=0;
     int inviato=0;

   
     //Inserimento username del contatto a cui inviare il messaggio
     printf("\nA quale contatto vuoi inviare un messaggio? Inserisci il suo username:");
     fflush(stdin);
     scanf("%s",destinatario);

     //Creazione variabile temporanea per scorrere la lista utenti e trovare il destinatario in cui inserire il messaggio inviato
     struct utente *utenteDestinatario=NULL;
     utenteDestinatario=headUtente;

     //Ciclo while che scorre la lista utenti ed usce se si raggiunge la fine della lista oppure si trova il destinatario
     while((utenteDestinatario!=NULL)&&(strcmp(utenteDestinatario->username,destinatario)!=0))
     {
         utenteDestinatario=utenteDestinatario->next;
     }

     //Uscita dal while e verifica se l'utente destinatario è stato trovato
     if(utenteDestinatario==NULL)
     {
         printf("\nNon esiste nessun utente con l'username da te inserito.\n");
     }
     else
     {
         //Creazione variabile temporanea per scorrere la lista contatti 
         contatto *cerca;
         cerca=NULL;
         cerca=ricercaContattoUsername(*utenteAttuale,destinatario);
        
         //Se il contatto non viene trovato in lista contatti
         if(cerca==NULL)
         {
             printf("\nL'utente a cui vuoi inviare il messaggio non è presente nella tua lista contatti.\n");
             //Setto la variabile inviato a 0 per indicare che non è stato mandato il messaggio
             inviato=0;
         }
         else
         {
             char a[2]="";
             printf("\nContatto trovato.Premi un tasto qualsiasi per continuare:");
             fflush(stdin);
             gets(a);
             
             //Il contatto è stato trovato in lista contatti, inserimento del messaggio da inviare
             printf("\n\nScrivi il messaggio da inviare a %s.\n",destinatario);
             fflush(stdin);
             gets(conversazione);

             //Creazione nodo messaggio da aggiungere alla lista messaggi
             messaggio *nuovoMessaggio;
             nuovoMessaggio=(messaggio*)malloc(sizeof(messaggio));
                              
             //Copia credenziali nel messaggio
		     strcpy(nuovoMessaggio->mittente,(*utenteAttuale)->username);
             printf("\nMittente:%s\n",nuovoMessaggio->mittente);
		      
             strcpy(nuovoMessaggio->destinatario,destinatario);
             printf("Destinatario:%s\n",destinatario);

	         strcpy(nuovoMessaggio->conversazione,conversazione);
             printf("Testo:%s\n",nuovoMessaggio->conversazione);

             //Ottenimento data odierna    
             time_t rawtime;
             time(&rawtime);
             struct tm *timeinfo=localtime(&rawtime);
             char dataAttuale[64];
             
             assert(strftime(dataAttuale,sizeof(dataAttuale),"%c",timeinfo));
            
             //Copia della data in formato stringa dentro il messaggio
             strcpy(nuovoMessaggio->data,dataAttuale);
             printf("Data invio:%s\n",nuovoMessaggio->data);

             //Assegnazione degli id del messaggio    
             nuovoMessaggio->idUtenteMitt=(*utenteAttuale)->idUtente;
             nuovoMessaggio->idUtenteDest=utenteDestinatario->idUtente;
             
                 
               
             //(_____INSERIMENTO DEL MESSAGGIO IN TESTA ALLA LISTA MESSAGGI DEL MITTENTE______)
             (*utenteAttuale)->nuovoMess=nuovoMessaggio;
            
             printf("\nMessaggio inviato!\n");
             inviato=1;
         }
     }
     return inviato;
}
    





//________________VISUALIZZA CHAT__________
void visualizzaChat(struct utente *utenteAttuale)
{
     //Inserimento variabile per verificare se viene visualizzato almeno un messaggio
     int controllaMessaggi=0;

     //Inserimento del nickname dell'utente di cui visualizzare la conversazione 
     char nickname[20]="";
     
     printf("\nInserisci l'username di un utente e visualizzerai i messaggi scambiati con esso:");
     scanf("%s",nickname);

     //Creazione variabile temporanea per scorrere e stampare i messaggi tra mittente e destinatario
     messaggio *cercaConversazione=NULL;
     cercaConversazione=utenteAttuale->messaggi;

     //Scorrimento della lista messaggi dell'utente fino a quando non si raggiunge la fine e stampa di ogni messaggio scambiato tra mittente e destinatario 
     while(cercaConversazione!=NULL)
     {
         //Se il mittente del messaggio corrisponde al nickname dell'utente ricercato, stampa di data,mittente e testo del messaggio
         if(strcmp(cercaConversazione->mittente,nickname)==0)
         {    
             printf("\nData:%s\n",cercaConversazione->data);
             printf("%s:%s\n",cercaConversazione->mittente,cercaConversazione->conversazione);
             //Stampato un messaggio, incremento variabile di controllo
             controllaMessaggi++;
            
         }
         else
         {
             //Se il destinatario del messaggio corrisponde al nickname dell'utente ricercato, stampa di data,mittente e testo del messaggio
             if(strcmp(cercaConversazione->destinatario,nickname)==0)
             {
                 printf("\nData:%s\n",cercaConversazione->data);
                 printf("%s:%s\n",cercaConversazione->mittente,cercaConversazione->conversazione);
                 //Stampato un messaggio, incremento variabile di controllo
                 controllaMessaggi++;
                 
             }
         }
         cercaConversazione=cercaConversazione->next;
     }

     //Se non è stato stampato neanche un messaggio
     if(controllaMessaggi==0)
     {
         printf("\nNon sono presenti messaggi con il contatto selezionato\n");
     }
 return;
}



//____________CANCELLA MESSAGGIO__________
void cancellaMessaggio(struct utente **utenteAttuale, char messCanc[])
{
  
     //Creazione variabile per uscire dal while nel caso viene trovato il messaggio da eliminare
     int trovato=0;

     //Creazioni variabili per cercare il messaggio da eliminare e quello precedente ad esso per agganciarlo al successivo
     messaggio *cercaMessaggio=(*utenteAttuale)->messaggi;
     messaggio *cercaMessaggioPrec=(*utenteAttuale)->messaggi;

     //Ciclo while che scorre la lista dei messaggi ed esce se si raggiunge la fine oppure viene trovato il messaggio da eliminare
     while((cercaMessaggio!=NULL)&&(trovato==0))
     {
         //Se il mittente del messaggio corrisponde all'username inserito dall'utente, la variabile trovato diventa 1 per uscire dal while
         if(strcmp(cercaMessaggio->mittente,messCanc)==0)
         {
             trovato=1;
         }
         else
         {
             //Se il destinatario del messaggio corrisponde all'username inserito dall'utente, la variabile trovato diventa 1 per uscire dal while
             if(strcmp(cercaMessaggio->destinatario,messCanc)==0)
             {
                 trovato=1;
             }
             else
             {
                 //Il messaggio non è ancora stato trovato, scorrimento della lista
                 cercaMessaggioPrec=cercaMessaggio;
                 cercaMessaggio=cercaMessaggio->next;
             }
     
         }
     }

     //Uscita dal while e verifica se si è raggiunta la coda della lista messaggi, cioè il messaggio non è stato trovato
     if(cercaMessaggio==NULL)
     {
         printf("\nNessun messaggio trovato.\n");
         return;
     }

     //Se il messaggio da eliminare si trova in testa alla lista dei messaggi dell'utente
     if(cercaMessaggio==(*utenteAttuale)->messaggi)
     {  
         (*utenteAttuale)->messaggi=(*utenteAttuale)->messaggi->next;
     }
     else
     {
         //Se il messaggio da eliminare non si trova in testa alla lista dei messaggi dell'utente
         cercaMessaggioPrec->next=cercaMessaggio->next;
     } 

     //Eliminazione del messaggio
     free(cercaMessaggio);
     printf("\nMessaggio eliminato.");
}
    

  




//______MAIN_____
int main()
{   
     //Creazione delle variabili scelta1 di tipo char per inserire le scelte di tipo testuali, e scelta2 di tipo int per inserire le scelte numeriche
     char scelta1[3]="";
     int scelta2=-1;
     char nickname[20]="";

     //Creazione della lista degli utenti e allocazione dinamica della memoria
     struct utente *headUtente=NULL;
     
     //Creazione nodo dell'utente attuale presente nella sessione 
     struct utente *utenteAttuale=NULL;
    
     //Creazione nodo contatto che verrà restituito nelle ricerche   
     contatto *contatto_r;
     contatto_r=NULL;
    
     //Creazione variabili intere di controllo per le varie operazioni
     int inviato=0;
     int eliminato=0;
    
     //Richiamo funzione che carica gli utenti dal file utenti.txt alla lista utenti
     caricaUtente(&headUtente);

     


     //Ciclo while che continua ad eseguire il codice del programma fino a quando l'utente sceglie di uscire definitivamente, inserendo "Si" alla scelta finale di uscita
     while(strcmp(scelta1,"Si")!=0)
     {

         //Dichiarazione variabile di appoggio per scorrere la lista degli utenti
         struct utente *temp1=NULL;
         temp1=headUtente;

         //Dichiarazione variabile intera che conta gli utenti registrati
         int nUtenti=0;
         
         //Se c'è almeno un utente già registrato, viene incrementata la variabile che conta gli utenti e viene stampato ciascun utente già registrato
         if(temp1!=NULL)
         {
             printf("\nLista utenti già registrati:\n");
             while(temp1!=NULL)
             {
                 nUtenti++;
                 printf("%d-%s\n",nUtenti,temp1->username);
                 temp1=temp1->next;
             }
         }



         //Richiesta all'utente di inserire Si o No a seconda se esso sia registrato o meno. Se è registrato eseguirà il login, altrimenti si registrerà
         printf("\nBenvenuto nella chat. Sei già registrato? (Si/No):");
         scanf("%s",scelta1);
    
         //Ramo di Login, l'utente è già registrato e presente in lista utenti
         if((strcmp(scelta1,"Si")==0)&&(headUtente!=NULL))
         {
             //Funzione di Login al programma
             utenteAttuale=Login(headUtente);
            
             //Richiamo funzioni che caricano i contatti ed i messaggi dal file contatti.txt e messaggi.txt
             caricaContatto(&utenteAttuale);
              
              //Svuotamento della lista messaggi dell'utente e successiva chiamata della funzione di caricamento dei messaggi dalla lista dell'utente
              svuotaLista(&utenteAttuale);
              caricaMessaggio(&utenteAttuale);
             
         }
         //Ramo di registrazione, l'utente non è già registrato e quindi provvede alla registrazione con inserimento in testa alla lista utenti e successivo Login diretto
         else
         {
             if(headUtente==NULL)
             {
                 printf("\nNessun utente presente in lista, registrazione in corso.\n");
             }

             //Funzione di Registrazione e Login diretto al programma
             utenteAttuale=Registrazione(&headUtente);
             nUtenti++;
             //Richiamo funzione che salva il nuovo utente nel file di testo utenti.txt
             salvaUtente(headUtente);
         }
    
         printf("\nBenvenuto %s!\n",utenteAttuale->username);
   
         
         //Ciclo do while che permette all'utente attuale di effettuare le operazioni indicate inserendo un numero che corrisponde all'operazione da effettuare.
         //Inserendo il numero 9 l'utente uscirà dal proprio account e deciderà se uscire definitivamente dal programma oppure tornare al Login/Registrazione
         //Se l'utente eliminerà il suo account, automaticamente la variabile scelta2 diventerà pari a 9 e quindi uscirà da questa schermata sempre decidendo se uscire definitivamente dal programma oppure tornare al Login/Registrazione
         do
         {
             printf("Inserisci una fra le possibili operazioni:\n");
             printf("Cancella il tuo account-->1\n");
             printf("Aggiungi un contatto alla tua lista contatti tramite ricerca Username-->2\n");
             printf("Aggiungi un contatto alla tua lista contatti tramite ricerca Nome e Cognome-->3\n");
             printf("Ricerca di un contatto-->4\n");
             printf("Rimuovi un contatto dalla tua lista contatti-->5\n");
             printf("Invia un messaggio ad un contatto-->6\n");
             printf("Visualizza una chat con un contatto-->7\n");
             printf("Cancella un messaggio-->8\n");
             printf("Esci dall'account-->9\n");
      
             //Inserimento dell'operazione da effettuare
             printf("Inserisci la tua scelta:");
             scanf("%d",&scelta2);
          
             //Inserimento di uno switch: a seconda del valore assunto da scelta2, verrà eseguito il ramo del case corrispondente
             switch(scelta2)
             {
                 case 1:
                         //L'utente ha inserito 1:Eliminazione dell'account
                         printf("\nHai scelto di eliminare il tuo account. L'azione sarà irreversibile. Sei sicuro? (Si/No):");
                         scanf("%s",scelta1);
                        
                         //Ramo di cancellazione dell'account.Se l'utente è sicuro di questa azione e inserisce Si, eliminerà il suo account e quindi l'utente verrà eliminato dalla lista utenti
                         if(strcmp(scelta1,"Si")==0)
                         {
                             //Dichiarazioni variabili intere per ottenere id ed username dell'utente eliminato
                             int idUtenteEliminato=0;
                             char userCanc[20]="";
                             strcpy(userCanc,utenteAttuale->username);
             
                             //Funzione eliminaUtente
                             idUtenteEliminato=eliminaUtente(&headUtente,utenteAttuale);
                            
                             //Decremento della variabile che conta gli utenti presenti in lista
                             nUtenti--;

                             //Richiamo funzioni che eliminano l'utente che ha cancellato il suo account e tutti i contatti ad esso associati dai file di testo 
                             salvaContattiCancellatoUtente(&headUtente,userCanc,idUtenteEliminato,nUtenti);
                             salvaUtente(headUtente);
                             
                           
                             printf("\nHai eliminato il tuo account.Uscita in corso.\n");
                             //L'utente è stato eliminato e quindi la scelta è automaticamente posta a 9, quindi si tornerà automaticamente alla scelta1 di uscita definitiva o meno dal programma
                             scelta2=9;
                         }
                         //L'utente ha inserito No e quindi non vuole eliminare il suo account
                         else
                         {
                             printf("\nHai scelto di non eliminare il tuo account.Ritorno al menù.\n\n");
                         }
                         break;
               case 2:
                         //L'utente ha inserito 2:Aggiunta di un contatto tramite ricerca Username
                         printf("\nAggiunta di un contatto tramite ricerca Username.");
                       
                         //Funzione aggiungiContattoUsername
                         contatto *nuovoContatto=NULL;
                         nuovoContatto=aggiungiContattoUsername(&utenteAttuale,headUtente);

                         //Se il contatto è stato effettivamente aggiunto alla lista contatti
                         if(nuovoContatto!=NULL)
                         {
                             //Richiamo funzioni di scrittura sul file del nuovo contatto e caricamento della nuova lista contatti
                             salvaNuovoContatto(nuovoContatto);
                             caricaContatto(&utenteAttuale);
                         }
                         printf("\nRitorno al menù.\n\n");
                         break;
               case 3:
                         //L'utente ha inserito 3:Aggiunta di un contatto tramite ricerca Nome e Cognome
                         printf("\nAggiunta di un contatto tramite ricerca Nome e Cognome.");
                     
                         //Funzione aggiungiContattoNC
                         contatto *nuovoContattoNC=NULL;
                         nuovoContattoNC=aggiungiContattoNC(&utenteAttuale,headUtente);
                         
                         //Se il contatto è stato effettivamente aggiunto alla lista contatti
                         if(nuovoContattoNC!=NULL)
                         {
                             //Richiamo funzioni di scrittura sul file del nuovo contatto e caricamento della nuova lista contatti
                             salvaNuovoContatto(nuovoContattoNC);
                             caricaContatto(&utenteAttuale);
                         }
                         printf("\nRitorno al menù.\n\n");
                         break;
               case 4:
                         //L'utente ha inserito 4:Ricerca di un contatto e visualizzazione di esso
                         printf("\nRicerca di un contatto in corso.");
                       
                          //Se la lista dei contatti dell'utente è vuota
                         if((utenteAttuale)->contatti==NULL)
                         {
                             printf("\n\nNon hai nessun contatto nella tua lista.\n\n");
                         }
                         else
                         {
                             //Se la lista dei contatti non è vuota, viene richiesto all'utente la modalità di ricerca del contatto   
                             printf("\nVuoi ricercare il contatto tramite Username o Nome e Cognome?\n");
                             fflush(stdin);
                             gets(scelta1);
                                 
                             //Se l'utente vuole ricercare il contatto tramite Username
                             if(strcmp(scelta1,"Username")==0)
                             {
                                 //Inserimento username del contatto da ricercare
                                 printf("\nInserisci l'username dell'utente che vuoi ricercare nei tuoi contatti:");
                                 fflush(stdin);
                                 scanf("%s",nickname);

                                 //Funzione ricerca contatto tramite username
                                 contatto_r=ricercaContattoUsername(utenteAttuale,nickname);
                             }
                             else
                             {
                                 //Funzione ricerca contatto tramite nome e cognome
                                 contatto_r=ricercaContattoNC(utenteAttuale);
                             }
                             if(contatto_r==NULL)
                             {
                                 printf("\nQuesto utente non si trova nei tuoi contatti\n");
                             }
                         }
                                  
                         printf("\nRitorno al menù.\n\n");
                         break;
               case 5:
                         //L'utente ha inserito 5 e vuole dunque rimuovere un contatto dalla sua lista contatti
                         printf("\nRimozione di un contatto.\n");
                        
                         //Se la lista dei contatti dell'utente è vuota
                         if((utenteAttuale)->contatti==NULL)
                         {
                             printf("\nNon hai nessun contatto nella tua lista.\n");
                         }
                         else
                         {
                             //Inserimento username del contatto da rimuovere dalla lista contatti
                             printf("\nInserisci l'username del contatto che vuoi rimuovere dalla tua lista contatti:");
                             scanf("%s",nickname);
                             
                             //Richiesta conferma da parte dell'utente sull'eliminazione del contatto
                             printf("L'utente verrà rimosso dai tuoi contatti. Sei sicuro? (Si/No):");
                             scanf("%s",scelta1);
                             
                             //Se l'utente inserisce "Si"
                            if(strcmp(scelta1,"Si")==0)
                            {
                                 //Ramo di cancellazione del contatto.L'utente ha inserito Si e quindi viene eliminato il contatto da lui indicato dalla sua lista contatti
                                 eliminato=rimuoviContatto(&utenteAttuale,nickname);
                                 
                                 //Se il contatto viene effettivamente trovato ed eliminato
                                 if(eliminato==1)
                                 {
                                     //Salvataggio sul file di testo della nuova lista dei contatti dell'utente senza il contatto appena rimosso
                                     salvaContattiCancellato(&headUtente,utenteAttuale,nickname);
                                
                                     //Svuotamento e ricaricamento dal file contatti.txt della lista dei contatti dell'utente
                                     utenteAttuale->contatti=NULL;
                                     caricaContatto(&utenteAttuale);
                                 }

                            }
                            else
                            {
                                 //L'utente ha inserito no e quindi il contatto non viene rimossso dalla lista contatti
                                 printf("\nHai scelto di non rimuovere il contatto.");
                            }
                         }
                         printf("\nRitorno al menù.\n\n");
                         break;
               case 6:
                         //L'utente ha inserito 6:Invio messaggio ad un contatto
                         printf("\nInvio messaggio ad un contatto.\n");
                         
                         //Se la lista dei contatti dell'utente è vuota, non sarà possibile inviare un messaggio ad un contatto 
                         if((utenteAttuale)->contatti==NULL)
                         {
                              printf("\nNon hai nessun contatto nella tua lista.\n");
                         }
                         else
                         {
                             //Funzione inviaMessaggio
                             inviato=inviaMessaggio(&utenteAttuale,headUtente);

                             //Se il messaggio è stato effettivamente inviato
                             if(inviato==1)
                             {
                                 //Salvataggio del messaggio sul file messaggi.txt
                                 salvaMessaggi(&utenteAttuale);
                                 //Caricamento della nuova lista dei messaggi
                                 caricaMessaggio(&utenteAttuale);
                             }
                         }
                         printf("\n\nRitorno al menù.\n\n");
                         break;
               case 7:
                         //L'utente ha inserito 7:Visualizzazione chat con un contatto
                         printf("\nVisualizzazione chat con un contatto.");

                         //Se la lista dei contatti dell'utente è vuota
                         if(utenteAttuale->contatti==NULL)
                         {
                             printf("\n\nNon hai nessun contatto nella tua lista.\n\n");
                         }
                         else
                         {
                             //La lista dei contatti dell'utente non è vuota, richiamo della funzione di visualizzazione chat
                             visualizzaChat(utenteAttuale);
                             printf("\nRitorno al menù.\n\n");
                         }
                         break;
               case 8: 
                         //L'utente ha inserito 8:Eliminazione di un messaggio
                          printf("\nCancella un messaggio.");
                         
                         //Richiesta conferma di cancellazione del messaggio
                         printf("\nStai per cancellare un messaggio che non potrà essere recuperato.Sei sicuro? (Si/No):");
                         scanf("%s",scelta1);
                       
                         //Ramo di eliminazione del messaggio.L'utente ha inserito Si e dunque il messaggio verrà eliminato dalla sua lista messaggi
                         if(strcmp(scelta1,"Si")==0)
                         {
                             //Inserimento del nickname dell'utente di cui cancellarne l'ultimo messaggio
                             char messCanc[20]="";
                             printf("\nInserisci l'username di un utente e cancellerai l'ultimo messaggio scambiato con egli:");
                             scanf("%s",messCanc);

                             //Funzione di cancellazione del messaggio
                             cancellaMessaggio(&utenteAttuale, messCanc);
 
                             //Richiamo funzione che salva sul file dei messaggi la lista dei messaggi escludendo il messaggio appena eliminato
                             salvaMessaggiCancellato(&headUtente, utenteAttuale, messCanc);

                             //Svuotamento e ricaricamento della lista dei messaggi aggiornata
                             svuotaLista(&utenteAttuale); 
                             caricaMessaggio(&utenteAttuale);
                            
                             //Inversione della lista dei messaggi per la corretta lettura delle chat
                             salvaMessaggiCancellato(&headUtente, utenteAttuale,"");
                            
                             svuotaLista(&utenteAttuale);

                             caricaMessaggio(&utenteAttuale);
                         }
                         else
                         {
                             //L'utente ha inserito No e quindi il messaggio non verrà eliminato 
                             printf("\nHai scelto di non eliminare il messaggio.");
                         } 
                         printf("\nRitorno al menù.\n\n");
                         break;
               case 9: 
                         //L'utente ha inserito 9 e quindi uscirà dal proprio account
                         printf("\nUscita dall'account.\n\n");
                         break;
               default:
                         //L'utente ha inserito un numero che non corrisponde ad alcuna operazione e viene invitato ad inserire una scelta valida
                         printf("\nScelta non valida.Inserisci di nuovo.\n");
                         break;
             }
        
         }while(scelta2!=9);

         //Uscita dal while perchè l'utente ha deciso di uscire dal proprio account inserendo il numero 9 oppure ha deciso di cancellarlo
         //Scelta di uscita definitiva dal programma o meno
         printf("\nVuoi uscire dal programma? (Si/No):");
         scanf("%s",scelta1);
     }
    
 printf("\n\nUscita dal programma. Arrivederci.\n\n");
}