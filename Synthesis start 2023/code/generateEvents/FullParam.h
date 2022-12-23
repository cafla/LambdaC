#ifndef FULLPARAM_H
#define FULLPARAM_H

//Functions and enum to deal with all parameters of distribution instead of the ones from a0


#include <math.h>
#include "BreitWigner.C"


vector<double> AdaptParam(const double *paramValues);
bool IsInDalitz(double s,double m1,double m2,double m3,double s12,double s23);


namespace BaseParam {
enum BaseParam : int{A=0,B=1,C=2,D=3,A1=4,A2=5,B1=6,B2=7};
}
namespace FormulaFullParam{
enum FormulaFullParam: int{A2=0,B2,C2,D2,A12,A22,B12,B22,ReA1A2,ReB1B2,ReAD,ReBC,ReA1C,ReA2C,ReB1D,ReB2D,ReA1B,ReA2B,ReB1A,ReB2A,  ReAB, ReCD, ReA1B1,ReA1B2,ReA2B1,ReA2B2, ReAC,ReBD, ReA1D,ReA2D,ReB1C,ReB2C, ReA1A,ReA2A,ReB1B,ReB2B, NUM_FINAL_PARAM };					
inline FormulaFullParam& operator++(FormulaFullParam& param, int) { //To allow to go through with loops
    const int i = static_cast<int>(param)+1;
    param = static_cast<FormulaFullParam>((i) % (static_cast<int>(NUM_FINAL_PARAM)+1));
    return param;
}
}

bool IsInDalitz(double s,double m1,double m2,double m3,double s12,double s23)
{
    //Return whether (s12,s23) is in the Dalitz plot
    
    double E2 = (s12 - m1*m1 + m2*m2)/(2*sqrt(s12));
    double E3 = (s - s12 - m3*m3)/(2*sqrt(s12));
    return ((E2+E3)*(E2+E3)-(sqrt(E2*E2-m2*m2) + sqrt(E3*E3-m3*m3))*(sqrt(E2*E2-m2*m2) + sqrt(E3*E3-m3*m3)) <= s23) and ((E2+E3)*(E2+E3)-(sqrt(E2*E2-m2*m2) - sqrt(E3*E3-m3*m3))*(sqrt(E2*E2-m2*m2) - sqrt(E3*E3-m3*m3)) >= s23);
}

vector<double> AdaptParamFull(const double *paramValues)
{
	/*
		Function to get A2,B2,...,ReB2A from A,B,C,D,...,B2 
		/!\ suppose A,B,C,D,...,B2 are real /!\
	*/
	vector<double> newParam;
	//-------------a0------------------
	//Del2
	newParam.push_back(paramValues[BaseParam::A]*paramValues[BaseParam::A]);//A2
	newParam.push_back(paramValues[BaseParam::B]*paramValues[BaseParam::B]);//B2
	//Lam2
	newParam.push_back(paramValues[BaseParam::C]*paramValues[BaseParam::C]);//C2
	newParam.push_back(paramValues[BaseParam::D]*paramValues[BaseParam::D]);//D2
	//Kst2
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::A1]);//A12
	newParam.push_back(paramValues[BaseParam::A2]*paramValues[BaseParam::A2]);//A22
	newParam.push_back(paramValues[BaseParam::B1]*paramValues[BaseParam::B1]);//B12
	newParam.push_back(paramValues[BaseParam::B2]*paramValues[BaseParam::B2]);//B22
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::A2]);//ReA1A2
	newParam.push_back(paramValues[BaseParam::B1]*paramValues[BaseParam::B2]);//ReB1B2
	//LamDel
	newParam.push_back(paramValues[BaseParam::A]*paramValues[BaseParam::D]);//ReAD
	newParam.push_back(paramValues[BaseParam::B]*paramValues[BaseParam::C]);//ReBC
	//LamKst
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::C]);//ReA1C
	newParam.push_back(paramValues[BaseParam::A2]*paramValues[BaseParam::C]);//ReA2C
	newParam.push_back(paramValues[BaseParam::B1]*paramValues[BaseParam::D]);//ReB1D
	newParam.push_back(paramValues[BaseParam::B2]*paramValues[BaseParam::D]);//ReB2D
	//DelKst
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::B]);//ReA1B
	newParam.push_back(paramValues[BaseParam::A2]*paramValues[BaseParam::B]);//ReA2B
	newParam.push_back(paramValues[BaseParam::B1]*paramValues[BaseParam::A]);//ReB1A
	newParam.push_back(paramValues[BaseParam::B2]*paramValues[BaseParam::A]);//ReB2A
	//-------------b0------------------
	//Del2
	newParam.push_back(paramValues[BaseParam::A]*paramValues[BaseParam::B]);//ReAB
	//Lam2
	newParam.push_back(paramValues[BaseParam::C]*paramValues[BaseParam::D]);//ReCD
	//Kst2
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::B1]);//ReA1B1
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::B2]);//ReA1B2
	newParam.push_back(paramValues[BaseParam::A2]*paramValues[BaseParam::B1]);//ReA2B1
	newParam.push_back(paramValues[BaseParam::A2]*paramValues[BaseParam::B2]);//ReA2B2
	//LamDel
	newParam.push_back(paramValues[BaseParam::A]*paramValues[BaseParam::C]);//ReAC
	newParam.push_back(paramValues[BaseParam::B]*paramValues[BaseParam::D]);//ReBD
	//LamKst
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::D]);//ReA1D
	newParam.push_back(paramValues[BaseParam::A2]*paramValues[BaseParam::D]);//ReA2D
	newParam.push_back(paramValues[BaseParam::B1]*paramValues[BaseParam::C]);//ReB1C
	newParam.push_back(paramValues[BaseParam::B2]*paramValues[BaseParam::C]);//ReB2C
	//DelKst
	newParam.push_back(paramValues[BaseParam::A1]*paramValues[BaseParam::A]);//ReA1A
	newParam.push_back(paramValues[BaseParam::A2]*paramValues[BaseParam::A]);//ReA2A
	newParam.push_back(paramValues[BaseParam::B1]*paramValues[BaseParam::B]);//ReB1B
	newParam.push_back(paramValues[BaseParam::B2]*paramValues[BaseParam::B]);//ReB2B

	return newParam;
}




#endif 
