#include "ReadCSV.Cxx"

std::vector<std::vector<double>> LoadEvents(std::string fileName,vector<int> selectedCol);
std::vector<std::vector<double>> LoadNamedEvents(std::string fileName,vector<std::string> columnsName);
std::vector<std::string> GetTitleLine(std::string fileName);
std::vector<int> SelectColumns(std::string fileName, std::vector<std::string> columnsName);


/*
	Functions to load events in csv format
*/

std::vector<std::vector<double>> LoadEvents(std::string fileName,vector<int> selectedCol)
{
	/*
		Function to load events from a csv file 
		/!\ the first column can not be loaded for some unknown reason /!\ 
		(which is not an issue since it usually is the row number)
		/!\ column numbers begins at 0 for the 2nd column /!\ (which is the reason you can't load the first one)
	*/
	
    std::ifstream file(fileName);
	std::vector<std::vector<double>> returnTable;
	std::vector<double> oneEvent;
	
	
	std::cout  << "\n";
	std::setvbuf(stdout, NULL, _IONBF, 0);
	
	CSVIterator loop(file);//Loop initialization to avoid title line
	loop++;
    for(; loop != CSVIterator(); ++loop) {
		oneEvent = {};
		for (std::size_t indexCol(0); indexCol< selectedCol.size(); ++indexCol)
			oneEvent.push_back(std::stod(asString((*loop)[selectedCol[indexCol]])));
		returnTable.push_back(oneEvent);
    }
	return returnTable;
}

std::vector<std::vector<double>> LoadNamedEvents(std::string fileName,vector<std::string> columnsName)
{
	/*
		Function to load events from a csv file 
		/!\ the first column can not be loaded for some unknown reason /!\ 
	*/
	std::vector<int> selectedCol (SelectColumns(fileName, columnsName));
	return LoadEvents(fileName, selectedCol);
}


std::vector<std::string> GetTitleLine(std::string fileName)
{
    std::ifstream file(fileName);
	
	CSVIterator loop(file);
	std::vector<std::string>titleLine = {};
	for (std::size_t indexLine(0); indexLine< loop->size(); ++indexLine)
	{
		titleLine.push_back(asString((*loop)[indexLine]));
	}
	
	
	return titleLine;
}

std::vector<int> SelectColumns(std::string fileName, std::vector<std::string> columnsName)
{
	std::vector<std::string> titles(GetTitleLine(fileName));
	std::vector<int> selectedCol;
	
	for(std::size_t indexCol(0); indexCol < columnsName.size();++indexCol){
		for(std::size_t indexTitle(0); indexTitle < titles.size();++indexTitle){
			if(titles[indexTitle] == columnsName [indexCol])
				selectedCol.push_back(indexTitle);
		}
	}
	
	return selectedCol;
}

