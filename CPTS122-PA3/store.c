//Xuhang Liu---PA3

#include"store.h"


/*************************************************************
* Function: pa3_main ()
* Description: This function controls the simulation.
* Preconditions: the simulation needs to be run
* Postconditions: the simulation has been run
*************************************************************/

void pa3_main(int simulation_number, BOOL isSaving)
{
	// variable declaration
	FILE *outFile = NULL;
	char simulationFile[64], itoaBuffer[4];
	CustomerData tempData;
	Queue normalLane, expressLane;
	int sim_state = 0, ticks = 0, maxTicks = 0, custNum = 0,
		nextNormal = 0, nextExpress = 0,
		totalNormal = 0, totalExpress = 0, totalDifference = 0;
	KeyInput keys;

	// print simulation control keys for user to know
	printf("Manually Run Hint:\n");
	printf("Press [Spacebar] at any point to end the simulation.\n");
	printf("Press [P] at any point to pause the simulation.\n\n");
	// main simulation control loop
	while (sim_state >= 0)
	{
		// initial setup
		if (sim_state == 0)
		{
			// create the simulation save file if supposed to
			if (isSaving)
			{
				strcpy(simulationFile, "simulation");
				strcat(simulationFile, itoa(simulation_number, itoaBuffer, 10));
				strcat(simulationFile, ".txt");
				outFile = fopen(simulationFile, "w");
			}
			// print welcome screen
			printf("************ Grocery Store Simulator *************\nSimulation #%d\n\n", simulation_number);
			if (isSaving)
				fprintf(outFile, "************** Grocery Store Simulator **************\nSimulation #%d\n\n", simulation_number);
			// ask how long simulation should last
			printf("How long should the Simulation last (enter -1 = till manually ended)? ");
			maxTicks = getIntInputCheck(-1, 99999999999999999);
			// initialize the lanes
			initializeQueue(&normalLane);
			initializeQueue(&expressLane);
			// set tick count to 0
			ticks = 0;
			// seed the random number generator
			seedRandomGenerator(true, 0);
			// get and print the first arrival times
			nextNormal = getArrivalTime(false);
			nextExpress = getArrivalTime(true);
			printf("First Normal Line Arrival: %d minute(s).\nFirst Express Line Arrival: %d minute(s).\n", nextNormal, nextExpress);
			if (isSaving)
				fprintf(outFile, "First Normal Line Arrival: %d minute(s).\nFirst Express Line Arrival: %d minute(s).\n", nextNormal, nextExpress);
			sim_state = 1;
		}
		// normal simulation
		else if (sim_state == 1)
		{
			// as long as we having meet the final tick
			if (ticks != maxTicks)
			{
				// tick increment/decrement
				ticks++;
				nextNormal--;
				nextExpress--;
				// NEW CUSTOMER ARRIVAL
				// check if a new normal customer has arrived
				if (nextNormal == 0)
				{
					// add the new arrival to the lane
					tempData = clearData();
					custNum++;
					printf("Customer #%d has arrived.\n", custNum);
					if (isSaving)
						fprintf(outFile, "Customer #%d has arrived.\n", custNum);
					tempData.customerNumber = custNum;
					tempData.serviceTime = getServiceTime(false);
					if (totalNormal == 0)
						totalNormal = tempData.serviceTime;
					else
						totalNormal = totalNormal + tempData.serviceTime;
					tempData.totalTime = totalNormal;
					if (isEmpty(normalLane))
						tempData.serviceTime++;
					enqueue(&normalLane, tempData);
					// get a new arrival time
					nextNormal = getArrivalTime(false);
				}
				// check if a new express customer has arrived
				if (nextExpress == 0)
				{
					// add a new arrival to the lane
					tempData = clearData();
					custNum++;
					printf("Customer #%d has arrived.\n", custNum);
					if (isSaving)
						fprintf(outFile, "Customer #%d has arrived.\n", custNum);
					tempData.customerNumber = custNum;
					tempData.serviceTime = getServiceTime(true);
					if (totalExpress == 0)
						totalExpress = tempData.serviceTime;
					else
						totalExpress = totalExpress + tempData.serviceTime;
					tempData.totalTime = totalExpress;
					if (isEmpty(expressLane))
						tempData.serviceTime++;
					enqueue(&expressLane, tempData);
					// get a new arrival time
					nextExpress = getArrivalTime(true);
				}
				// SERVICE
				// Normal Line
				if (!isEmpty(normalLane))
				{
					// decrement current service time
					normalLane.pHead->data.serviceTime--;
					// if they are done, dequeue the customer
					if (0 == normalLane.pHead->data.serviceTime)
					{
						printf("Customer #%d has finished checking out.\n", normalLane.pHead->data.customerNumber);
						if (isSaving)
							fprintf(outFile, "Customer #%d has finished checking out.\n", normalLane.pHead->data.customerNumber);
						dequeue(&normalLane);
					}
				}
				// Express Line
				if (!isEmpty(expressLane))
				{
					// decrement current service time
					expressLane.pHead->data.serviceTime--;
					// if they are done, dequeue the customer
					if (0 == expressLane.pHead->data.serviceTime)
					{
						printf("Customer #%d has finished checking out.\n", expressLane.pHead->data.customerNumber);
						if (isSaving)
							fprintf(outFile, "Customer #%d has finished checking out.\n", expressLane.pHead->data.customerNumber);
						dequeue(&expressLane);
					}
				}

				// 10 TICK PRINTOUT
				if ((ticks % 10) == 0)
				{
					// print report message and current queues
					printf("\n\n******** TEN MINUTE REPORT ********\n");
					if (isSaving)
						fprintf(outFile, "\n\n******** TEN MINUTE REPORT ********\n");
					printf("******** NORMAL LANE ********\n");
					if (isSaving)
						fprintf(outFile, "******** NORMAL LANE ********\n");
					printQueueRecursive(normalLane, outFile, isSaving);
					printf("******** EXPRESS LANE ********\n");
					if (isSaving)
						fprintf(outFile, "******** EXPRESS LANE ********\n");
					printQueueRecursive(expressLane, outFile, isSaving);
					printf("\n");
					if (isSaving)
						fprintf(outFile, "\n");
				}
				// check for keys being pressed
				keys = getKeySelected();
				// is the spacebar being pressed?
				if (keys.key == '~')
				{
					if (keys.specialKey == 'S')
					{
						// if it is, end the simulation
						sim_state = 2;
					}
				}
				// is the simulation being paused?
				else if (keys.key == 'P')
				{
					// if it is, pause the simulation
					pause_clear(true, false);
				}
			}
			// if we *are* at the end of a simulation, end the simulation
			else
			{
				sim_state = 2;
			}
		}
		// End of Simulation
		else
		{
			// print final report
			printf("\n******** END OF SIMULATION ********\n");
			if (isSaving)
				fprintf(outFile, "******** END OF SIMULATION ********\n");
			printf("Simulation ran for %d minute(s).\n", ticks);
			if (isSaving)
				fprintf(outFile, "Simulation ran for %d minute(s).\n", ticks);
			printf("NOTE: Total times counts in those people who are still in line at the time of the end of the simulation.\n");
			if (isSaving)
				fprintf(outFile, "NOTE: Total times count in those people who are still in line at end of the simulation.\n");
			printf("Normal Lane Total Time: %d minute(s).\n", totalNormal);
			if (isSaving)
				fprintf(outFile, "Normal Lane Total Time: %d minute(s).\n", totalNormal);
			printf("Express Lane Total Time: %d minute(s).\n", totalExpress);
			if (isSaving)
				fprintf(outFile, "Express Lane Total Time: %d minute(s).\n", totalExpress);
			// did the normal lane take longer? Say so
			if (totalNormal > totalExpress)
			{
				totalDifference = totalNormal - totalExpress;
				printf("Express Lane was more efficient (by %d minute(s)).\n", totalDifference);
				if (isSaving)
					fprintf(outFile, "Express Lane was more efficient (by %d minute(s)).\n", totalDifference);
			}
			// did the express lane take longer? Say so
			else if (totalNormal < totalExpress)
			{
				totalDifference = totalExpress - totalNormal;
				printf("Normal Lane was more efficient (by %d minute(s)).\n", totalDifference);
				if (isSaving)
					fprintf(outFile, "Normal Lane was more efficient (by %d minute(s)).\n", totalDifference);
			}
			// did neither lane take longer? say so
			else
			{
				printf("Neither Lane was more efficient.\n");
				if (isSaving)
					fprintf(outFile, "Neither Lane was more efficient.\n");
			}
			printf("\n");
			// if the program is saving, tell the user that
			if (isSaving)
			{
				printf("A record has been made of this simulation.\n");
				fclose(outFile);
			}
			// end the simulation loop (end the function and goto main menu)
			sim_state = -1;
		}
	}
}

/*************************************************************
* Function: getArrivalTime ()
* Description: This function gets a random arrival time
* Preconditions: an arrival time is needed
* Postconditions: an arrival time is gotten
*************************************************************/
int getArrivalTime(BOOL isExpress)
{
	int arrivalTime = 0;
	// if the service time should be an express type
	if (isExpress)
	{
		arrivalTime = getRandomNumber(1, 5);
	}
	// otherwise, it must be a normal type
	else
	{
		arrivalTime = getRandomNumber(3, 8);
	}
	// return the time
	return arrivalTime;
}

/*************************************************************
* Function: getServiceTime ()
* Description: This function gets a random service time
* Preconditions: an service time is needed
* Postconditions: an service time is gotten
*************************************************************/
int getServiceTime(BOOL isExpress)
{
	int serviceTime = 0;
	// if the service time should be an express type
	if (isExpress)
	{
		serviceTime = getRandomNumber(1, 5);
	}
	// otherwise, it must be a normal type
	else
	{
		serviceTime = getRandomNumber(3, 8);
	}
	// return the time
	return serviceTime;
}

/*************************************************************
* Function: clearData ()
* Description: This function returns a cleared CustomerData struct.
*					Used "just in case" (had problems with one of the
*					structs at one point).
* Preconditions: a cleared CustomerData struct is needed
* Postconditions: a cleared CustomerData struct is gotten
*************************************************************/
CustomerData clearData(void)
{
	// create/return cleared CustomerData
	CustomerData tempData = { 0, 0, 0 };
	return tempData;
}

void pause_clear(BOOL shouldPause, BOOL shouldClear)
{
	// does the screen need to be paused
	if (shouldPause)
		system("pause");
	// does the screen need to be cleared
	if (shouldClear)
		system("cls");
}

/*************************************************************
* Function: seedRandomGenerator ()
* Description: This function seeds the random number generator
* Input parameters: whether the time should be assigned as the
*						seed or not and the random seed
* Returns: N/A
* Preconditions: a random number is needed
* Postconditions: a random number is returned
*************************************************************/
void seedRandomGenerator(BOOL assignTime, int randomSeed)
{
	// if the time should be assigned as the seed, assign it
	if (assignTime)
		srand((unsigned)time(NULL));
	// otherwise assign the seed var as the seed
	else
		srand((unsigned)randomSeed);
}

/*************************************************************
* Function: getRandomNumber ()
* Description: This function returns a random int
* Input parameters: the minimum number and max number for the
*						random number's range
* Returns: the random number
* Preconditions: a random number is needed
* Postconditions: a random number is returned
*************************************************************/
int getRandomNumber(int minNum, int maxNum)
{
	int randomNum = 0, backupMax = maxNum;
	// increase the max num by one so it can actually be included in the range
	maxNum++;

	do
	{
		randomNum = rand() % maxNum + minNum;
	} while ((randomNum < minNum) || (randomNum > backupMax));
	// return a random number modded by the max number and added to
	//			by the min number
	return randomNum;
}

/*************************************************************
* Function: getIntInputCheck ()
* Description: This function gets a "good" int input
* Input parameters: the minimum good value, the maximum good value
* Returns: the good int input
* Preconditions: a "good" int input value is needed
* Postconditions: a "good" int input value is returned
*************************************************************/
int getIntInputCheck(int minGood, int maxGood)
{
	// declare variables
	int goodInputMain = 0, goodInputSub = 0, input = 0;
	// repeat while the desiered input is not achieved
	while (!goodInputMain)
	{
		// clear the buffer
		_flushall();
		// get the input, assigned to the input variable.
		// assign the results of the scanf to a variable
		goodInputSub = scanf(" %d", &input);
		// if the scanf was successful...
		if (goodInputSub)
		{
			// check if the input falls in the range...
			if ((input >= minGood) && (input <= maxGood))
			{
				// break the input loop if it does
				goodInputMain = 1;
			}
		}
	}
	return input;
}

/*************************************************************
* Function: getDoubleInputCheck ()
* Description: This function gets a "good" double input
* Input parameters: the minimum good value, the maximum good value
* Returns: the good double input
* Preconditions: a "good" double input value is needed
* Postconditions: a "good" double input value is returned
*************************************************************/
double getDoubleInputCheck(double minGood, double maxGood)
{
	// declare the variables
	int goodInputMain = 0, goodInputSub = 0;
	double input = 0.0;
	// repeat while the desiered input is not achieved
	while (!goodInputMain)
	{
		// clear the buffer
		_flushall();
		// get the input, assigned to the input variable.
		// assign the results of the scanf to a variable
		goodInputSub = scanf(" %lf", &input);
		// check if the scanf was successful
		if (goodInputSub)
		{
			// check if the input falls within desiered range
			if ((input >= minGood) && (input <= maxGood))
			{
				// break the input loop
				goodInputMain = 1;
			}
		}
	}
	return input;
}

void printMessage(char message[128], BOOL shouldPause, BOOL shouldClear)
{
	printf("%s", message);
	pause_clear(shouldPause, shouldClear);
}

int display_menu(int inputMode, int itemPreType, char itemPreChar, int minItem, int numItems,
	char itemDelim[], char programTitle[], char menuTitle[], char menuItems[])
{
	int option = minItem, i = 0, j = 0, isSelected = 0,
		inputCycle = 1, specialKey = 0, optionCheck = 0;
	char backupMenu[4096], menu[64][64], *tempItem, char_item = '\0';
	KeyInput keysSelected;

	strcpy(backupMenu, menuItems);
	tempItem = strtok(backupMenu, itemDelim);
	strcpy(menu[0], tempItem);
	for (i = 1; i < numItems; i++)
	{
		tempItem = strtok(NULL, itemDelim);
		strcpy(menu[i], tempItem);
	}

	do
	{
		//// DISPLAY MENU ////
		// Display Program and Menu Titles
		printf("%s\n\n", programTitle);
		printf("%s\n\n", menuTitle);
		// Display Items
		for (i = 1; i <= numItems; i++)
		{
			// Print selector icon
			// option is selected
			if (option == i)
			{
				printf(" > ");
			}
			// option is not selected
			else
			{
				printf("  ");
			}

			// print number or characters if necessary
			if (inputMode == 0 || inputMode == 2)
			{
				// if nothing, print nothing
				if (itemPreType == 1 || itemPreType == 2)
				{
					j = i;
					while (j > 0)
					{
						char_item = char_reverse_convert(j, itemPreType - 2);
						printf("%c", char_item);
						j -= 26;
					}
				}
				// print the char after the id but before the item
				printf("%c ", itemPreChar);
			}
			// print item string
			printf("%s\n", menu[i - 1]);
		}

		//GET INPUT
		do
		{
			// sleep the program for a bit to help with input detection
			Sleep(100);
			// get input
			keysSelected = getKeySelected();
			// check keys if one was pressed
			if (keysSelected.key != '\0')
			{
				// check if a special key was pressed
				if (keysSelected.key == '~')
				{
					specialKey = 1;
				}

				// if a special key was pressed
				if (specialKey == 1)
				{
					// check arrows if allowed
					if ((inputMode == 1) || (inputMode == 2))
					{
						// up and left
						if ((keysSelected.specialKey == 'W') || (keysSelected.specialKey == 'A'))
						{
							option--;
						}
						// down and right
						else if ((keysSelected.specialKey == 'S') || (keysSelected.specialKey == 'D'))
						{
							option++;
						}
					}
					// check enter
					if (keysSelected.specialKey == 'E')
					{
						isSelected = 1;
					}
				}
				// otherwise
				else
				{
					// check if allowed to do letter input
					if ((inputMode == 0) || (inputMode == 2))
					{
						// assign the key selected to a check variable and check if it is
						//			a valid option
						optionCheck = char_convert(keysSelected.key);
						if ((optionCheck >= minItem) && (optionCheck <= numItems))
						{
							// if it is, assign the option to the selected key
							option = optionCheck;
						}
					}
				}
				// end the input cycle
				inputCycle = 0;
			}
		} while (inputCycle);

		// OPTION VALIDITY CHECK
		if (option < minItem)
		{
			option = numItems;
		}
		else if (option > numItems)
		{
			option = minItem;
		}

		// RESET INPUT CYCLE
		inputCycle = 1;
		specialKey = 0;

		// CLEAR SCREEN
		system("cls");
	} while (!isSelected);

	// return the option selected
	return option;
}

/*************************************************************
* Function: getKeySelected ()
* Description: This function gets the current
*			   pressed key. Special input keys
*			   have priority over regular input keys..
* Returns: A value for the key
* Preconditions: A key input is needed
* Postconditions: The key input is gotten
*************************************************************/
KeyInput getKeySelected(void)
{
	int i = 0;
	KeyInput key = { '\0', '\0' };
	/* SPECIALKEY KEYCODES
	* Key = ~
	Special Key =
	* Up Arrow: W
	* Left Arrow: A
	* Down Arrow: S
	* Right Arrow: D
	* Enter: Enter
	*/

	//// LETTERS
	for (i = 1; i <= 26; i++)
	{
		if (GetAsyncKeyState((char)(i + 64)))
		{
			key.key = (char)(i + 64);
		}
	}

	//// SPECIAL KEYS
	// up arrow selected
	if (GetAsyncKeyState(VK_UP))
	{
		key.key = '~';
		key.specialKey = 'W';
	}
	// left arrow selected
	else if (GetAsyncKeyState(VK_LEFT))
	{
		key.key = '~';
		key.specialKey = 'A';
	}
	// right arrow selected

	else if (GetAsyncKeyState(VK_RIGHT))
	{
		key.key = '~';
		key.specialKey = 'D';
	}
	// down arrow selected
	else if (GetAsyncKeyState(VK_DOWN))
	{
		key.key = '~';
		key.specialKey = 'S';
	}
	// enter selected
	else if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		key.key = '~';
		key.specialKey = 'E';
	}
	else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		key.key = '~';
		key.specialKey = 'S';
	}

	// return key
	return key;
}

/*************************************************************
* Function: char_convert ()
* Description: This function converts a char value to a
*						int version of itself (adjusted so A = 0 and
*						lower + uppercases have the same value).
* Input parameters: value - the char value to adjust
* Returns: The adjusted char value
* Preconditions: A char value is needed to be adjusted.
* Postconditions: The char value has been adjusted.
*************************************************************/
int char_convert(char value)
{
	// assign the char value to a int version and reduce it's value by 64
	int int_value = (int)value - 64;
	// if the value is greater than 32, reduce it by 32
	if (int_value > 32)
	{
		int_value -= 32;
	}
	// return the value
	return int_value;
}

/*************************************************************
* Function: char_reverse_convert ()
* Description: This function converts a passed char
*						into a lower or upper-cased char.
* Input parameters: value - the value to adjust.
*								is_upper - Should it be uppercase?
* Returns: The adjusted char value
* Preconditions: A char value is needed to be adjusted.
* Postconditions: The char value has been adjusted.
*************************************************************/
char char_reverse_convert(int value, int is_upper)
{
	// create a temporary char variable
	char char_value = '\0';
	// add 64 to the passed value
	int int_value = value + 64;
	// if it is supposed to be lowercase, add another 32 to it
	if (!is_upper)
	{
		int_value += 32;
	}
	// convert it to a char and return it
	char_value = (char)int_value;
	return char_value;
}

/*************************************************************
* Function: initializeQueue ()
* Description: This function initializes a queue
* Preconditions: a queue needs to be initialized
* Postconditions: a queue has been initialized
*************************************************************/
void initializeQueue(Queue *pQueue)
{
	// assign the queue head/tail to nothing
	pQueue->pHead = NULL;
	pQueue->pTail = NULL;
}

/*************************************************************
* Function: isEmpty ()
* Description: This checks if aa queue is empty.
* Preconditions: a queue needs to be checked
* Postconditions: a queue has been checked
*************************************************************/
Bool isEmpty(Queue theQ)
{
	// return whether the node is empty
	return (theQ.pHead == NULL);
}

/*************************************************************
* Function: enqueue ()
* Description: This function adds a node to the end of a queue
* Preconditions: a node needs to be added
* Postconditions: a node has been added
*************************************************************/
Bool enqueue(Queue *pQueue, CustomerData newData)
{
	QueueNode *pNew = NULL;
	Bool success = false;

	// get memory for the new node
	pNew = (QueueNode *)malloc(sizeof(QueueNode));

	// if we successfully got memory, finish creating the node
	if (pNew != NULL)
	{
		pNew->data = newData;
		pNew->pNext = NULL;

		if (isEmpty(*pQueue))
		{
			pQueue->pHead = pNew;
		}
		else
		{
			pQueue->pTail->pNext = pNew;
		}

		pQueue->pTail = pNew;

		success = true;
	}

	return success;
}

/*************************************************************
* Function: dequeue ()
* Description: This function deletes the first node in a queue
* Preconditions: a node needs to be dequeued
* Postconditions: a node has been dequeued
*************************************************************/
Bool dequeue(Queue *pQueue)
{
	QueueNode *pTemp = NULL;
	Bool success = false;

	// if the queue is not empty, delete the first node
	if (!isEmpty(*pQueue))
	{
		if (pQueue->pHead == pQueue->pTail)
		{
			pQueue->pTail = NULL;
		}

		pTemp = pQueue->pHead;
		pQueue->pHead = pQueue->pHead->pNext;
		free(pTemp);
	}

	return success;
}

/*************************************************************
* Function: printQueueRecursive ()
* Description: This function prints the queue in a recursive
*					fashion
* Preconditions: the queue needs to be printed
* Postconditions: the queue has been printed
*************************************************************/
void printQueueRecursive(Queue theQ, FILE *outFile, BOOL isSaving)
{
	// as long as the current node is not empty, print it and then go to the next call of this function with the next node
	if (!isEmpty(theQ))
	{
		printf("Customer #%d\nService Time: %d minutes.\n",
			theQ.pHead->data.customerNumber, theQ.pHead->data.serviceTime, theQ.pHead->data.totalTime);
		if (isSaving)
		{
			fprintf(outFile, "Customer #%d\nService Time: %d minutes.\n",
				theQ.pHead->data.customerNumber, theQ.pHead->data.serviceTime, theQ.pHead->data.totalTime);
		}
		theQ.pHead = theQ.pHead->pNext;
		printQueueRecursive(theQ, outFile, isSaving);
	}
}