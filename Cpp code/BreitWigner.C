/*
	Functions to compute Breit-Wigner terms (allow uniformity and simplificity in way to compute them)
*/

double BW2(double sR,double mR,double gammaR)
{
    /*
        Generic squared Breit-Wigner factor for a resonance in R on variable sR
    */
    return 1/((sR-mR*mR)*(sR-mR*mR) + mR*mR*gammaR*gammaR);
}


double ReBWTwoResonances(double sR1,double mR1,double gammaR1,double sR2,double mR2,double gammaR2)
{
    /*
        For factor type ReAC,ReBD,... with two different resonances
        If two parameters come from the same resonance, BW2 is equivalent
        /!\ Formula suppose parameters are real /!\ 
    */
    return ((sR1 - mR1*mR1)*(sR2 - mR2*mR2) + mR1*mR2*gammaR1*gammaR2)*BW2( sR1, mR1, gammaR1)*BW2( sR2, mR2, gammaR2);
}
