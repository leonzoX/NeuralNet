
#include<stdlib.h>
#include<stdio.h>

#ifndef TRAIN_DATA_H
#define TRAIN_DATA_H

typedef struct {
	unsigned int nCorrect;//numero di previsioni corrette

	unsigned int epochs;//numero di scorrimenti del dataset

	double* errors;//array contenente l'history di tutti gli errori(per plottare il risultato)

	double meanError;//errore medio
}TrainData;


TrainData createTrainData(unsigned int epochs);//crea un nuovo adt TrainData
void freeTrainData(TrainData* trainData);

unsigned int getNCorrect(TrainData trainData);
void setNCorrect(TrainData* trainData,unsigned int nCorrect);
void updateNCorrect(TrainData*,unsigned int nUp);

unsigned int getEpochs(TrainData trainData);
void setEpochs(TrainData* trainData, unsigned int epochs);

double* getTrainErrors(TrainData trainData);
void setTrainErrors(TrainData* trainData,double* errors);
void setTrainErrorsAt(TrainData* trainData,double value,unsigned int index);

double getMeanError(TrainData trainData);
void setMeanError(TrainData* trainData,double meanError);
void updateMeanError(TrainData* trainData,double meanUp);

void saveTrainData(TrainData trainData);//salva in un file csv l'history di tutti gli errori con le epochs per plottare 

double getAccuracy(TrainData trainData);//ritorna la accuracy della rete neurale


#endif