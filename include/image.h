#ifndef IMAGE_H
#define IMAGE_H

#include<stdlib.h>
#include"list.h"

//ADT per memorizzare immagini di numeri interi lette da un file csv

#define MAXIMGS 60000//numero massimo di immagini da caricare

typedef struct{
	unsigned int label;//label, ovvero il valore che rappresenta quell'immagine
	List* data;//lista di dati dell'immagine, (valori interi da 0-255)
}Image;

Image* createImage();
void freeImage(Image* img);

Image** loadImages(const char* filePath);//carica in un array di puntatori ad immagini i dati presenti su un file csv

unsigned int getLabel(Image* image);
void setLabel(Image* image,unsigned int label);

List* getData(Image* image);
void setData(Image* image,List* data);

#endif