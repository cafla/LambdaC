#include <string>
#include <math.h>
#include <fstream>

using namespace std;

//Functions to save tables, used to save events

void SaveTwoNamedDimTable(string repertory, const vector<string>& columnsNames,const vector<string>& linesNames,const vector<vector<double>>& table)
{
	//Save vector<vector<double>> table in CSV format
	//Needs names for both columns and lines

  
	ofstream saveFile(repertory);
	if (!saveFile) {
		exit(1);
	}
	for(size_t i =0; i< columnsNames.size();i++){
		saveFile << "," << columnsNames[i];
	}
	saveFile << "\n";
	for (size_t iLine = 0;iLine <table.size();iLine++){
		saveFile << linesNames[iLine];
		for(size_t iCol =0; iCol< table[iLine].size();iCol++)
			saveFile << "," << table[iLine][iCol];
		saveFile << "\n";
	}	  
}

void SaveTwoNamedDimOneIndexTable(string repertory, const vector<string>& columnsNames,const vector<string>& linesNames,const double* table)
{
	//Save table in format double[iLine*sizeLine + iCol] 
	//Needs to have names to each of the columns and lines
  
	ofstream saveFile(repertory);
	if (!saveFile) {
		exit(1);
	}
	int sizeLine = columnsNames.size();
	//Saving Titles
	for(size_t iCol =0; iCol< sizeLine;iCol++){
		saveFile << "," << columnsNames[iCol];
	}
	saveFile << "\n";
	for (size_t iLine = 0;iLine <linesNames.size();iLine++){
		saveFile << linesNames[iLine];
		for(size_t iCol =0; iCol< columnsNames.size();iCol++)
			saveFile << "," << table[iLine*sizeLine + iCol];
		saveFile << "\n";
	}	  
}