#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void CheckToken(string& line) {
  size_t pos = line.find("**");
  
  //if (pos != std::string::npos) {
  while ( (pos = line.find("**"))!= std::string::npos) {
    pos = line.find("**");
    //cout << " # in line:" << line << " it is present a token **" << endl;
    
    //size_t beginningPosToken = line.rfind(""
    size_t posToken = pos - 1;
    char currentChar = line.at(posToken);
	int nbParenthesis = 0;
    while (
	   (currentChar != '*'
	   and
	   currentChar != '/'
	   and
	   currentChar != ' '
	   and
	   currentChar != '('
	   and
	   currentChar != '+'
	   and
	   currentChar != '-')
	   or
	   nbParenthesis > 0
	   )
      {
	//cout << "posToken = " << posToken << endl;
	//cout << "current char = " << currentChar << endl;
	if(currentChar == ')')
		nbParenthesis++;
	if(currentChar == '(')
		nbParenthesis--;
      
	posToken--;
	currentChar = line[posToken];
	  }
    //cout << "posToken = " << posToken << endl;
    
    //cout << " - to treat as a power: " << line.substr(pos + 2, 1) << endl;
    size_t power = stoi(line.substr(pos + 2, 1));
    //cout << " power = " << power << endl;
    
    //cout << "the first part = " << line.substr(0, posToken + 1) << endl;
    //cout << " the part to be powered: " << line.substr(posToken + 1, pos - posToken - 1) << endl;
    //cout << " the rest: " << line.substr(pos - posToken + 2, line.size()) << endl;

    /*string line = 
      line.substr(0, posToken + 1) 
      + "pow(" 
      + line.substr(posToken + 1, pos - posToken - 1)
      + ","
      + to_string(power) 
      + ")"
      + line.substr(pos - posToken + 2, line.size());*/

    string beginning = line.substr(0, posToken + 1) 
      + "pow(" ;

    string powerString = line.substr(posToken + 1, pos - posToken - 1)
      + ","
      + to_string(power) 
      + ")";

    string ending = line.substr(pos + 3, line.size());

    //cout << "beginning: " << beginning << endl;
    //cout << "powerString: " << powerString << endl;
    //cout << "ending: " << ending << endl;
    
    line = beginning + powerString + ending;
  }// if token is present


  
  
}


int pythonConverter() {
  fstream fs("PythonExpr.txt", std::ios_base::in);
  fstream of("CppExpr.txt", std::ios_base::out);
  string line;
  stringstream ss;
  
  while (getline(fs, line)) {
    CheckToken(line);
    ss << line;
  }

  line = ss.str();
  cout << "remove np." << endl;
  while (size_t pos = line.find("np.") != std::string::npos) {
    pos = line.find("np.");
    //cout << " pos of np. = " << pos << endl;
    line = line.substr(0, pos) + line.substr(pos + 3, line.size());
  }
  cout << "result: " << line << endl;
    
  of << line;
  of.close();
  //cout << "result = " <<  ss.str() << endl;
  return 0;
}
