#ifndef LIST_H
#define LIST_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>


//nodo della lsita doppiamente concatenata
typedef struct Node{
	void* info;//contenuto informativo generico
	struct Node* next;//handle al nodo successivo
	struct Node* prev;//handle al nodo precedente
}Node;

typedef void(*freeFunction)(Node*);//funzione di deallocazione

typedef struct List{
	unsigned int size;
	size_t typeSize;//spazio occupato dal contenuto informativo di ogni singolo nodo
	freeFunction freeFunc;//puntatore a funzione per eliminare lo spazio di un singolo nodo
	Node* head;//handle alla testa della lista
	Node* tail;//handle alla coda della lista
}List;

/*alloca lo spazio per una nuova lista, richiede come parametri lo spazio occupato da ogni dato che 
deve contenere la lista e il puntatore ad una funzione per deallocare lo specifico tipo di dato 
contenuto dalla lista
*/
List* createList(size_t typeSize,freeFunction freeFunc);
int add(List* list,void* element);//aggiunge un nuovo elemento in testa alla lista
void loopH(List* list,void(*func)(Node* node));//loopa la lista dalla testa alla coda e per ogni elemento esegue una funzione
void loopT(List* list,void(*func)(Node* node));//loopa la lista dalla coda alla testa
void freeList(List* list);//libera lo spazio allocato dalla lista 
Node* getHead(List* list);//ritorna la testa della lista
Node* getTail(List* list);//ritorna la coda della lsitas
//---WRAPPER FUNCTIONS----
void freePrimitive(Node* node);//libera lo spazio di un qualsiasi dato non allocato nell'heap
double getDouble(Node* node);//ritorna l'info di un nodo della lista effettuando un cast a double
void setDouble(Node* node,double info);//setta il valore di un nodo della lista effettuando un cast a double
float getFloat(Node* node);//....
void setFloat(Node* node,float info);//...
int getInt(Node* node);//...
void setInt(Node* node,int info);//...


#endif


