#include <string>
#include <math.h>
#include <fstream>

using namespace std;

void saveEvents(string repertory, vector<string> columsName,vector<vector<double>> events)
{
	//print in a file  the results of one experience = subset of parameters
	//Suppose there is already a line explaining the variables and parameters (this fonction only output the numerical values)
  
	ofstream saveFile(repertory);//Save file for basic parameters (cut range etc.) Serve as identifier of what was done 
	if (!saveFile) {
		exit(1);
	}
	int paramNum = columsName.size();
	//Saving Titles
	for(int i =0; i< paramNum;i++){
		saveFile << "," << columsName[i];
	}
	saveFile << "\n";
	for (int numEvent = 0;numEvent <events.size();numEvent++){
		saveFile << numEvent;
		for(int i =0; i< paramNum;i++)
			saveFile << "," << events[numEvent][i];
		saveFile << "\n";
	}	  
}
