/*******************************************************************************************
* Programmer: Xuhang Liu                                                                   *
* Class: Cpts 122, Summer 2016                                                             *
* PA #3                                                                                    *
* Date: June 27, 2016                                                                      *
* Description: Grocery Store Simulation					                                   *
*******************************************************************************************/

#ifndef STORE_H
#define STORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


// Structs
typedef struct customerData
{
	int customerNumber;
	int serviceTime;
	int totalTime;
} CustomerData;

typedef struct queueNode
{
	CustomerData data;
	struct queueNode *pNext;
} QueueNode;

typedef struct queue
{
	QueueNode *pHead;
	QueueNode *pTail;
} Queue;

typedef struct keyInput
{
	char key;
	char specialKey;
} KeyInput;

typedef enum bool
{
	false,
	true
} Bool;


//functions
void pause_clear(BOOL shouldPause, BOOL shouldClear);
void randomGen(BOOL assignTime, int randomSeed);
int getRandomNumber(int minNum, int maxNum);
int int_check(int minGood, int maxGood);
double double_check(double minGood, double maxGood);
void printMessage(char message[128], BOOL shouldPause, BOOL shouldClear);
int display_menu(int inputMode, int itemPreType, char itemPreChar, int minItem, int numItems, char itemDelim[], char programTitle[], char menuTitle[], char menuItems[]);
KeyInput getKeySelected(void);
int char_convert(char value);
char char_reverse_convert(int value, int is_upper);


void pa3_main(int simulation_number, BOOL isSaving);
int arrive_time(BOOL isExpress);
int service_time(BOOL isExpress);

CustomerData clearData(void);

void initializeQueue(Queue *pQueue);
BOOL isEmpty(Queue theQ);
BOOL enqueue(Queue *pQueue, CustomerData newData);
BOOL dequeue(Queue *pQueue);
void printQueueRecursive(Queue theQ, FILE *outFile, BOOL isSaving);

#endif


