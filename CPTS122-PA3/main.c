//Xuhang Liu---PA3

#include"store.h"


int main(void)
{
	// variables
	FILE *inFile = NULL;
	int sim_state = 0, option = 0, simNumber = 0;
	BOOL isSaving = false;

	// main program loop
	while (sim_state >= 0)
	{
		// initial loading state
		if (sim_state == 0)
		{
			// load the resources file to check if it exists (and create one if need be).
			printf("Loading resources file...");
			inFile = fopen("resources.simr", "r");
			if (inFile == NULL)
			{
				printf("Can't find the resources file, creating a new resources file...");
				inFile = fopen("resources.simr", "w");
				fprintf(inFile, "0\n1");
				printf("Resources file created...");
			}
			fclose(inFile);
			inFile = fopen("resources.simr", "r");
			fscanf(inFile, "%d", &simNumber);
			fscanf(inFile, " %d", &isSaving);
			fclose(inFile);
			printf("Resources file loaded!\n");
			// goto the main menu state
			sim_state = 1;
		}
		// show the main menu
		else
		{
			// get option from displayed menu
			option = display_menu(1, 0, ':', 1, 3, ";", "Grocery Store Simulation", "Main Menu",
				"1.Start New Simulation;2.Toggle Simulation Record Creation ON or OFF;3.Exit");
			// OPTIONS
			// 1. run a new simulation
			if (option == 1)
			{
				//collect data from resources file
				inFile = fopen("resources.simr", "r");
				fscanf(inFile, "%d", &simNumber);
				fscanf(inFile, " %d", &isSaving);
				simNumber++;
				fclose(inFile);
				inFile = fopen("resources.simr", "w");
				fprintf(inFile, "%d\n%d", simNumber, isSaving);
				fclose(inFile);
				// run sim
				pa3_main(simNumber, isSaving);
			}
			// 2.toggle simulation saving on or off
			else if (option == 2)
			{
				// if it is currently saving, make it not
				if (isSaving)
				{
					printf("No longer creating a record of simulations.\n");
					isSaving = false;
					inFile = fopen("resources.simr", "w");
					fprintf(inFile, "%d\n%d", simNumber, isSaving);
					fclose(inFile);
				}
				// otherwise, make it save
				else
				{
					printf("Now creating a record of simulations.\n");
					isSaving = true;
					inFile = fopen("resources.simr", "w");
					fprintf(inFile, "%d\n%d", simNumber, isSaving);
					fclose(inFile);
				}
			}
			// 3.exit
			else
			{
				fclose(inFile);
				sim_state = -1;
			}
		}
	}

	return 0;
}