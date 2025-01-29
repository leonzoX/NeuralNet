#include"image.h"
#include"neuralNet.h"
#include "trainData.h"

#define NUMTEST 10000
#define NUMTRAIN 60000

typedef enum{
	true,
	false,
	error
}boolean;

typedef struct{
	TrainData trainData;//dati di training

	Image** testImages;//immagini per effettuare il testing
	Image** trainImages;//immagini per effettuare il training

	NeuralNet* net;//rete neurale

	unsigned int batchSize;//batch size se si utilizza il STGD

	unsigned int numberOfLayers;//numero di layer per effettuare l'inizializzazione della rete neurale

	boolean customConf;//valore booleano per sapere che tipo di configurazione si sceglie (custom o default)

	unsigned int* layerConf;//array contenente la configurazione dei layer

	Mode trainingMode;//modalità di training
}ImageRecognizer;

//getters e setters---------

TrainData getTrainData(ImageRecognizer* imgRec);
void setTrainData(ImageRecognizer* imgRec,TrainData trainData);

Image** getTestImages(ImageRecognizer* imgRec);
void setTestImages(ImageRecognizer* imgRec,Image** testImages);

Image** getTrainImages(ImageRecognizer* imgRec);
void setTrainImages(ImageRecognizer* imgRec,Image** trainImages);

NeuralNet* getNet(ImageRecognizer* imgRec);
void setNet(ImageRecognizer* imgRec,NeuralNet* net);

unsigned int getBatchSize(ImageRecognizer* imgRec);
void setBatchSize(ImageRecognizer* imgRec,unsigned int batchSize);

unsigned int getNumLayers(ImageRecognizer* imgRec);
void setNumLayers(ImageRecognizer* imgRec,unsigned int numLayers);

boolean getCustomConf(ImageRecognizer* imgRec);
void setCustomConf(ImageRecognizer* imgRec,boolean customConf);

unsigned int* getLayerConf(ImageRecognizer* imgRec);
void setLayerConf(ImageRecognizer* imgRec,unsigned int* layerConf);

Mode getTrainMode(ImageRecognizer* imgRec);
void setTrainMode(ImageRecognizer* imgRec,Mode trainMode);
//--------------------------------------------------------------

int createImageRecognizer(ImageRecognizer** imgRec);//ritorna 1 se tutto va a buon fine, -1 altrimenti
int initImageRecognizer(ImageRecognizer* imgRec);//ritorna 1 se va bene e -1 altrimenti

int saveImageRecognizer(ImageRecognizer* imgRec,char* filePath);//ritorna 1 se va tutto bene, -1 se il file non è trovato e -2 se la rete neurale non è stata allocata
int loadImageRecognizer(ImageRecognizer* imgRec,char* filePath);

int trainImageOnline(ImageRecognizer* imgRec);//ritorna -1 se qualcosa va storto , -2 se viene chiamata la funzione senza aver prima inzializzato la rete neurale
int trainImageMiniBatch(ImageRecognizer* imgRec);

boolean isPredictionCorrect(unsigned int expected,List* response);//verifica se l'output è uguale alla label
unsigned int predict(List* response);//ritorna l'output della rete neurale

int testImageRecognizer(ImageRecognizer* imgRec);//effettua il testing della rete neurale

void freeImageRecognizer(ImageRecognizer* imgRec);//libera lo spazio allocato dalla rete neurale
