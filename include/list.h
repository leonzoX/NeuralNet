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

//getters e setters per Node---------------
void* getInfo(Node* node);
void setInfo(Node* node,void* info);

Node* getNext(Node* node);
void setNext(Node* node,Node* next);

Node* getPrev(Node* node);
void setPrev(Node* node,Node* prev);
//----------------------------------------



//getters e setters per Node---------------
typedef void(*freeFunction)(Node*);//funzione di deallocazione
typedef void(*saveFunction)(Node* node,FILE* file);


typedef struct List{

	unsigned int size;//dimensione logica della lista

	size_t typeSize;//spazio occupato dal contenuto informativo di ogni singolo nodo

	freeFunction freeFunc;//puntatore a funzione per eliminare lo spazio di un singolo nodo

	saveFunction saveFunc;//puntatore a funzione per salvare una lista in un file binario

	Node* head;//handle alla testa della lista

	Node* tail;//handle alla coda della lista

}List;

typedef void(*loadFunction)(List* list,short int dim,FILE* file);/*load list si trova fuori dalla struct list, poichè 
nel file binario non viene salvata la funzione per caricare la lista*/

/*alloca lo spazio per una nuova lista, richiede come parametri lo spazio occupato da ogni dato che 
deve contenere la lista e il puntatore ad una funzione per deallocare lo specifico tipo di dato 
contenuto dalla lista
*/
List* createList(size_t typeSize,freeFunction freeFunc);//crea un nuovo ADT di tipo List
void freeList(List* list);//libera lo spazio allocato dalla lista 
void freePrimitive(Node* node);//libera lo spazio di un qualsiasi dato di tipo primitivo(char,int,fload,double e rispettivi array) non allocato nell'heap

int add(List* list,void* element);//aggiunge un nuovo elemento in testa alla lista
int isEmpty(List* list);//ritorna 1 se la lista è vuota , altrimenti -1

void loopH(List* list,void(*func)(Node* node));//scorre la lista dalla testa alla coda e per ogni elemento esegue una funzione
void loopT(List* list,void(*func)(Node* node));//loopa la lista dalla coda alla testa e per ogni elemento esegue una funzione


//getter e setter------------------------------------------------------

Node* getHead(List* list);//ritorna la testa della lista
void setHead(List* list,Node* head);

Node* getTail(List* list);//ritorna la coda della lista
void setTail(List* list,Node* tail);

unsigned int getSize(List* list);
void setSize(List* list,unsigned int size);

size_t getTypeSize(List* list);
void setTypeSize(List* list,size_t typeSize);

freeFunction getFreeFunction(List* list);
void setFreeFunction(List* list,freeFunction freeFunc);

saveFunction getSaveFunction(List* list);
void setSaveFunction(List* list,saveFunction saveFunc);
//------------------------------------------------------------------------------


void printList(List* list,void(*printFunc)(Node* node));//metodo generico per stampare una lista
int saveList(List* list,FILE* file);
List* loadList(FILE* file,loadFunction loadFunc,freeFunction freeFunc);
void setSaveFunc(List* list,saveFunction saveFunc);

void loadPrimitive(List* list,short int dim,FILE* file);


//---WRAPPER FUNCTIONS----
void freeLists(Node* node);//libera lo spazio allocato da una lista di liste
void saveLists(Node* node,FILE* file);//salva in un file binario una lista di liste

double getDouble(Node* node);//ritorna l'info di un nodo della lista effettuando un cast a double
void setDouble(Node* node,double info);//setta il valore di un nodo della lista effettuando un cast a double
void printDouble(Node* node);//..
void saveDouble(Node* node,FILE* file);

float getFloat(Node* node);//....
void setFloat(Node* node,float info);//...

int* toIntArray(List* list);//trasforma una lista di interi in un array di interi
int getInt(Node* node);//...
void setInt(Node* node,int info);//...

char* getString(Node* node);//ritorna una stringa
char* getStringAt(List* list,unsigned int position);//ritorna una stringa nella posizione desiderata
void printString(Node* node);//stampa una stringa


#endif


