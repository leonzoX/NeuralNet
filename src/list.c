#include"list.h"


List* createList(size_t typeSize,freeFunction freeFunc){
	List* list = (List*)malloc(sizeof(List));
	list->size = 0;
	list->typeSize = typeSize;
	list->freeFunc = freeFunc;
	list->saveFunc = NULL;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

int add(List* list,void* element){
	//inizializzo il primo elemento allocando la memoria per la struct Node e per il suo contenuto informativo
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->info = malloc(list->typeSize);

	//successivamente copio il contenuto del parametro element nella parte informativa di Node , byte per byte, utilizzando i char
	memcpy(newNode->info,element,list->typeSize);

	//controllo se le malloc sono andate a buon fine 
	if(newNode == NULL || newNode->info == NULL)
		return -1;
	//se la lista è vuota allora avrò head e tail uguali al nuovo nodo, inoltre il nodo precedente sarà nullo
	if(list->size == 0){
		list->head = newNode;
		list->tail = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;
	}else{
		newNode->next = list->head;
		(list->head)->prev = newNode;
		list->head = newNode;
		newNode->prev = NULL;
	}
	list->size += 1;
	return 1;
}

int isEmpty(List* list){
	if(list != NULL){
		if(getHead(list) == NULL && getTail(list) == NULL)
			return 1;
		else return -1;
	}else{
		return 1;
	}
}

void loopH(List* list,void(*func)(Node* node)){//loopa dalla testa alla coda e per ogni elemento esegue la funzione passata come parametro
	Node* tempHead = list->head;
	while(tempHead != NULL){
		(*(func))(tempHead);
		tempHead = tempHead->next;
	}
}

void loopT(List* list,void(*func)(Node* node)){//loopa dalla testa alla coda
	Node* tempTail = list->tail;
	while(tempTail != NULL){
		(*(func))(tempTail);
		tempTail = tempTail->prev;
	}
}

void freeList(List* list){
	//scorro tutta la lista dalla testa e per ogni elemento eseguo la funzione freeFunc
	if(list != NULL){
		Node* tempHead = list->head;
		Node* delNode = NULL;
		while(tempHead != NULL){
			delNode = tempHead;
			tempHead = tempHead->next;
			list->freeFunc(delNode);
		}
		free(list);
	}
}

//getters e setters per Node----
void* getInfo(Node* node){
	return node->info;
}
void setInfo(Node* node,void* info){
	node->info = info;
}

Node* getNext(Node* node){
	return node->next;
}
void setNext(Node* node,Node* next){
	node->next = next;
}

Node* getPrev(Node* node){
	return node->prev;
}
void setPrev(Node* node,Node* prev){
	node->prev = prev;
}

//----------------------------

Node* getHead(List* list){
	return list->head;
}
Node* getTail(List* list){
	return list->tail;
}
//------WRAPPER FUNCTIONS ------
//NOTA : questa funzione può essere utilizzata solamente per i tipi primitivi che non sono stati allocati nell'heap
void freePrimitive(Node* node){
	free(node->info);
	free(node);
}

double getDouble(Node* node){
	return *(double*)node->info;
}
void setDouble(Node* node,double info){
	*(double*)(node->info) = info;
}
float getFloat(Node* node){
	return *(float*)node->info;
}
void setFloat(Node* node,float info){
	*(float*)(node->info) = info;
}
int getInt(Node* node){
	return *(int*)node->info;
}
void setInt(Node* node,int info){
	*(int*)(node->info) = info;
}

void printDouble(Node* node){
	double val = *(double*)node->info;
	printf(" %lf ",val);
}

void printList(List* list,void(*printFunc)(Node* node)){
	printf("[");
	loopT(list,printFunc);
	printf("]\n");
}
void freeLists(Node* node){
	List* info = *(List**)node->info;//informazione puntata da dal valore puntato da void *
	freeList(info);
	free(node->info);//void *
	free(node);
}
char* getString(Node* node){
	return *(char**)(node->info);
}
char* getStringAt(List* list,unsigned int position){
	unsigned int count = 0;
	Node* tail = getTail(list);
	while(tail != NULL){
		if(count == position) return getString(tail);
		tail = tail->prev;
		count++;
	}
	return NULL;
}
void printString(Node* node){
	char* string = getString(node);
	printf("%s ",string);
}
int* toIntArray(List* list){
	int* array = (int*)malloc(sizeof(int) * (list->size));
	unsigned int conta = 0;
	Node* scan = list->tail;
	while(scan!= NULL){
		array[conta] = *((int*)(scan->info));
		conta++;
		scan = scan->prev;
	}
	return array;
}

int saveList(List* list,FILE* file){
	short int listDim = (short int)list->size;
	short int listSpace = (short int)list->typeSize;
	if(file!= NULL){
		//scrivo nel file un header composto da due short int, il primo indica la lunghezza della lista
		//il secondo indica lo spazio occupato da ciascun elemento della lista, in byte
		fwrite(&listDim,sizeof(short int),1,file);
		fwrite(&listSpace,sizeof(short int),1,file);
		Node* scan = list->tail;
		while(scan != NULL){
			list->saveFunc(scan,file);
			scan = scan->prev;
		}
		
		return 1;
	}
	return -1;
}
List* loadList(FILE* file,loadFunction loadFunc,freeFunction freeFunc){
	
	List* list = NULL;

	if(file !=NULL){
		short int dim = 0;
		short int size = 0;

		fread(&dim,sizeof(short int),1,file);
		fread(&size,sizeof(short int),1,file);


		list = createList((size_t)size,freeFunc);

		

		loadFunc(list,dim,file);

	


	}
	return list;
}
void loadPrimitive(List* list,short int dim,FILE* file){

	for(int i = 0; i < dim; i++){
		void* data = malloc(list->typeSize);
		
		fread(data,list->typeSize,1,file);
		
		add(list,data);
		free(data);
	}
}

void saveDouble(Node* node,FILE* file){
	double info = *(double*)node->info;

	fwrite(&info,sizeof(double),1,file);
}
void saveLists(Node* node,FILE* file){
	List* info = *(List**)node->info;

	saveList(info,file);
}

void setSaveFunc(List* list,saveFunction saveFunc){
	list->saveFunc = saveFunc;
}

unsigned int getSize(List* list){
	return list->size;
}
void setSize(List* list,unsigned int size){
	list->size = size;
}

size_t getTypeSize(List* list){
	return list->typeSize;
}
void setTypeSize(List* list,size_t typeSize){
	list->typeSize = typeSize;
}

freeFunction getFreeFunction(List* list){
	return list->freeFunc;
}
void setFreeFunction(List* list,freeFunction freeFunc){
	list->freeFunc = freeFunc;
}

saveFunction getSaveFunction(List* list){
	return list->saveFunc;
}
