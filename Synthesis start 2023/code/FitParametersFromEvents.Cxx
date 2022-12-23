#include "main.h"
#include "LoadEvents.Cxx"
#include "Math/IFunction.h"
#include "Minuit2/FCNBase.h"
#include "Minuit2/Minuit2Minimizer.h"
//#include "TFitter.h"
#include "TSystem.h"

/*
	Macro to fit parameters from events stored in eventRepertory
	Fit on 8 parameters (A,B,C,...,B2)
	Has written in hard the value of the integration of f_x_i which is used in computation 
*/


class FunctionForL : public ROOT::Math::IBaseFunctionMultiDim { 

public: 
	FunctionForL() {
		//intF_x(IntegrateF_x_i()) Doesn't work for now : Issue with 0
		intF_x = {1.58347, 25.9879, 0.102176, 2.13561, 216.843, 420.406, 598.861, 44.0982, 561.823, 232.594, 0.465147, -0.257775, -0.0346614, -0.46307, -0.730897, 0.69261, -3.23078, -17.2995, -6.07569, -0.880437};//Result from Emi : analytical integration
	}
	FunctionForL(const vector<vector<double>> & events)  {
		//intF_x(IntegrateF_x_i())
		valF_x_i =ComputeValF_x_i(events);
		intF_x = {1.58347, 25.9879, 0.102176, 2.13561, 216.843, 420.406, 598.861, 44.0982, 561.823, 232.594, 0.465147, -0.257775, -0.0346614, -0.46307, -0.730897, 0.69261, -3.23078, -17.2995, -6.07569, -0.880437};
	}
	FunctionForL(const vector<vector<double>> & inValF_x_i,vector<double> inIntF_x): valF_x_i(inValF_x_i), intF_x(inIntF_x) {
	}
	unsigned int NDim ()const{return 8;}
	ROOT::Math::IBaseFunctionMultiDim * Clone () const {return new FunctionForL(valF_x_i,intF_x);}
	double ComputeL(const double *initialParam) const;
private: 
	vector<vector<double>> valF_x_i;//All f_x_i /!\ in format valF_x_i[i][x] /!\ with x = parameter, i = event
	vector<double> intF_x; //Integrated terms of F_x_i over i to normalize, could be saved as a numerical value to increase speed
	double DoEval(const double *initialParam) const {
		return this->ComputeL(initialParam);
	} 

};



void FitParametersFromEvents(string eventRepertory)
{
	gSystem->Load("libMinuit2");
	bool minimized;
	
	std::vector<std::vector<double>> events;
	//Load events
	events = LoadNamedEvents(eventRepertory,{"mkpsq","mppisq"});
	S12S13ToS12S23(events);
	
	
	cout << "Event Num : " << events.size();
	FunctionForL mainFunction(events);//Compute f_x_i & Integrate f_x for normalization
	double startParam[] = {1,1, 1,1, 0.1,0.1,0.1,0.1};
	vector<string> paramName = {"A","B", "C","D", "A1","A2","B1","B2"};
	cout<< "L for all 1 : " << mainFunction.ComputeL(startParam) << endl;
	//minimize computeL
	
	ROOT::Minuit2::Minuit2Minimizer *minuit = new ROOT::Minuit2::Minuit2Minimizer();

	minuit->SetFunction(mainFunction);
	for(int indexParam(0); indexParam < 8; indexParam++)
		minuit->SetVariable(indexParam,paramName[indexParam].c_str(),startParam[indexParam],0.0001);
	cout << "Variables set" << endl;
	//minuit->SetPrintLevel(3);
	minimized = minuit->Minimize();
	if (minimized){
		cout << "Minimization successful" << endl;
		minuit->PrintResults();
		cout << "Lmin = " << minuit->MinValue ()<<endl;
		cout << "Ncalls =" << minuit->NCalls()<<endl;
		
		//Covariance		
		cout << "\nCovariance" << endl;
		cout << "Cov matrix status : " << minuit->CovMatrixStatus() << endl;
		for(unsigned int indexParam(0); indexParam < 8; indexParam++)
			cout << ",\t"<< paramName[indexParam];
		cout << endl;
		double covarianceMatrix[64];
		minuit->GetCovMatrix (covarianceMatrix);
		for(unsigned int indexParam1(0); indexParam1 < 8; indexParam1++){
			cout << paramName[indexParam1] << ",\t";
			for(unsigned int indexParam2(0); indexParam2 < 8; indexParam2++)
				cout << covarianceMatrix[indexParam1*8 + indexParam2] << ",\t";
			cout <<endl; 	
		}
		SaveTwoNamedDimOneIndexTable("covariance.csv", paramName,paramName,covarianceMatrix);
		
		//Correlation
		vector <vector<double>> corrMatrix;
		vector<double> oneLineCorr;
		cout << "\nCorrelation" << endl;
		for(unsigned int indexParam(0); indexParam < 8; indexParam++)
			cout << ",\t"<< paramName[indexParam];
		cout << endl;
		
		for(unsigned int indexParam1(0); indexParam1 < 8; indexParam1++){
			cout << paramName[indexParam1] << ",\t";
			oneLineCorr = {};
			for(unsigned int indexParam2(0); indexParam2 < 8; indexParam2++) {
				cout << minuit->Correlation(indexParam1,indexParam2) << ",\t";
				oneLineCorr.push_back(minuit->Correlation(indexParam1,indexParam2));
			}
			cout <<endl; 	
			corrMatrix.push_back(oneLineCorr);
		}
		SaveTwoNamedDimTable("correlation.csv", paramName,paramName,corrMatrix);	
	}
	else
		cout << "Couldn't minimize"<<endl;
	
				
}


double FunctionForL::ComputeL(const double *initialParam) const
{
	/*
		Computes L = sum(ln f^^) = sum_i (ln(sum_X(X'*f_X(i))) )
		with X = A2,B2,C2,...,Re(A2B), X' normalized X, i = event

		/!\ Needs valF_x_i to have been computed /!\
		/!\ Needs intF_x to have been computed /!\
	*/

	double returnVal = 0;
	vector<double> paramValues = AdaptParam(initialParam);
	double normDen = 0; //Normalization factor (denominator)
	double oneEventSum(0);
	for(size_t paramNum = 0; paramNum < paramValues.size();paramNum++)
		normDen += paramValues[paramNum]*intF_x[paramNum];
	
	int cutEvent = 0;// Number of events that are cut because they have an imaginary value in the distribution

	for(size_t eventNum = 0; eventNum<valF_x_i.size();eventNum++){
		oneEventSum = 0;
		for(size_t paramNum = 0; paramNum < paramValues.size();paramNum++) 
			oneEventSum += paramValues[paramNum]*valF_x_i[eventNum][paramNum];
		if (oneEventSum == oneEventSum) //Means that there is no NaN error, ie everything went right (precisely it is a sqrt(negative) that should return NaN)
			returnVal += log(oneEventSum/normDen);
		else 
			cutEvent++;
	}
	//cout << "cut events : " << cutEvent << endl; //switch on to display number of cut events
	return -returnVal;

}

