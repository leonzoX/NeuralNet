
#include<math.h>
#include<stdio.h>
#include"list.h"
#include"layers.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H


typedef double(*CostFunction)(double currValue,double expValue);
typedef double(*ActFunction)(double value);

/*tipo di dato che contiene i puntatori a funzioni cost e squish, con le loro rispettive derivate prime*/
typedef struct NetFunctions{
	CostFunction costFunc;//cost function
	CostFunction costFuncPrime;//derivata prima della cost function
	ActFunction squishFunc;//squishification function
	ActFunction squishFuncPrime;//derivata prima della squishification function
}NetFunctions;

//getters e setters ----------------------------
NetFunctions getFunctions(CostFunction costFunc,CostFunction costFuncPrime,ActFunction squishFunc,ActFunction squishFuncPrime);

CostFunction getCostFunc(NetFunctions functions);
void setCostFunc(NetFunctions* functions,CostFunction costFunc);

CostFunction getCostFuncPrime(NetFunctions functions);
void setCostFuncPrime(NetFunctions* functions,CostFunction costFuncPrime);

ActFunction getSquishFunc(NetFunctions functions);
void setSquishFunc(NetFunctions* functions,ActFunction squishFunc);

ActFunction getSquishFuncPrime(NetFunctions functions);
void setSquishFuncPrime(NetFunctions* functions,ActFunction squishFuncPrime);
//-----------------------------------

//funzioni disponibili---------------------
double linearActv(double value);
double linearActvPrime(double value);
double cost(double currValue,double expValue);
double costPrime(double currValue,double expValue);
double sigmoid(double value);
double relu(double value);
double sigmoidPrime(double value);
double logLoss(double pY,double y);
double logLossPrime(double pY,double y);
double reluPrime(double value);

double softMax(List* outLayer,double z);
double softMaxPrime(List* outLayer,double z);
#endif