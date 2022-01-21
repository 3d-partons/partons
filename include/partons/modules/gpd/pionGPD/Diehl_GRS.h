#ifndef Diehl_GRS_H
#define Diehl_GRS_H

/**
 * @file Diehl_GRS.h
 * @author Cédric Mezrag (CEA Saclay)
 * @date 19th January 2022
 * @version 1.0
 *
 * This code present the GPD model 1 described in the section 5 of Eur.Phys.J.C 58 (2008) 179-192
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>


#include "../GPDModule.h"
#include "../../MathIntegratorModule.h"


namespace PARTONS {

class pionDiehlGRSModel1: public PARTONS::GPDModule, public MathIntegratorModule
{

public:

    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;

    /**
     * Default constructor.
     * @param className Name of class.
     */
    pionDiehlGRSModel1(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~pionDiehlGRSModel1();
    virtual pionDiehlGRSModel1* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    // virtual std::string toString() const;

    //setters
    void setProfileParameter(double N);

    //getters
    double getProfileParameter();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    pionDiehlGRSModel1(const pionDiehlGRSModel1& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PARTONS::PartonDistribution computeH();

    /**
      * Computation of the double distribution.
      *
      * Sets the DD attribute of the class.
      */
    void computeDD( bool t0 );


private:

    std::vector<double> m_valPara;                                   // valence PDF-parameters {a,b,N_v,c,d}
    std::vector<double> m_seaPara;                                   // sea PDF-parameters {a,b,N_s,c,d}
    std::vector<double> m_gPara;                                     // gluons PDF-parameters {a,b,N_g,c,d}
    std::vector<double> m_reggeParaVal ;                                // Regge parameters {alpha',A,B}
    std::vector<double> m_reggeParaSea ;                                // Regge parameters {alpha',A,B}
    double m_LambdaDterm2 = 0.53 ;                                    //GeV^2 Dterm Monopole Parameter



    double mRDDA_Para;											// RDDA parameter n
    double m_Mx;                                                // -x variable

    double Profile(double alpha, double beta);        // Compute the Radyushkin Profile function
    // x-fitter pdf set
    double valencePdfAnsatz(double beta);	// Valence ansatz u(x) = bar(d) (x) = N_v * x^a (1-x)^b * (1+c sqrt(x)+d x)
    double seaPdfAnsatz(double beta);	// Sea ansatz  2 ub + 2 d + s + sb = 6 ub = N_s * x^(a-1) (1-x)^b * (1+c sqrt(x)+d x)
    double gluonxPdfAnsatz(double beta);	// gluon ansatz  x g(x) = N_g * x^a * (1-x)^b  * (1+c sqrt(x)+d x)

    //reggeized t-depedence
    double tReggeizedAnsatzVal(double beta);
    double tReggeizedAnsatzSea(double beta);

    void throwBetaException(const std::string &funcName, double betaValue);

    void initFunctorsForIntegrations();

    NumA::FunctionType1D* m_pIntegralHuVal;
    NumA::FunctionType1D* m_pIntegralHuValMx;
    NumA::FunctionType1D* m_pIntegralHdVal;
    NumA::FunctionType1D* m_pIntegralHdValMx;

    NumA::FunctionType1D* m_pIntegralDuVal;
    NumA::FunctionType1D* m_pIntegralDuValMx;
    NumA::FunctionType1D* m_pIntegralDdVal;
    NumA::FunctionType1D* m_pIntegralDdValMx;

    NumA::FunctionType1D* m_pIntegralxLargeHsSea;
    NumA::FunctionType1D* m_pIntegralxSmallHsSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HsSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HsSea;
    NumA::FunctionType1D* m_pIntegralxLargeHsSeaMx;

    NumA::FunctionType1D* m_pIntegralDSea;
    NumA::FunctionType1D* m_pIntegralDSeaMx;

    NumA::FunctionType1D* m_pIntegralxLargeHg;
    NumA::FunctionType1D* m_pIntegralxSmall1Hg;
    NumA::FunctionType1D* m_pIntegralxSmall2Hg;
    NumA::FunctionType1D* m_pIntegralxLargeHgMx;

    NumA::FunctionType1D* m_pIntegralDGluons;
    NumA::FunctionType1D* m_pIntegralDGluonsMx;


    //Hu contribution
    double IntegralHuVal(double beta, std::vector<double> Par);
    double IntegralHuValMx(double beta, std::vector<double> Par);
    double HuValDD(double beta, double alpha);

    //D-term uval contributions
    double IntegralDuVal(double beta, std::vector<double> Par);
    double IntegralDuValMx(double beta, std::vector<double> Par);
    double DtermuValence(double z, double beta);

    // Hd contribution
    double IntegralHdVal(double beta, std::vector<double> Par);
    double IntegralHdValMx(double beta, std::vector<double> Par);
    double HdValDD(double beta, double alpha);

    //D-term dval contribution
    double IntegralDdVal(double beta, std::vector<double> Par);
    double IntegralDdValMx(double beta, std::vector<double> Par);
    double DtermdValence(double z, double beta);


    // H gluons contribution
    double IntegralxSmall2Hg(double beta, std::vector<double> Par);
    double IntegralxSmall1Hg(double beta, std::vector<double> Par);
    double IntegralxLargeHgMx(double beta, std::vector<double> Par);
    double IntegralxLargeHg(double beta, std::vector<double> Par) ;
    double HgDD(double beta, double alpha);


    //D-term gluon contribution
    double IntegralDGluons(double beta, std::vector<double> Par);
    double IntegralDGluonsMx(double beta, std::vector<double> Par);
    double DtermGluons(double z, double beta);

    //Hsea (uds) contribution
    double IntegralxLargeHsSea(double beta, std::vector<double> Par);
    double IntegralxLargeHsSeaMx(double beta, std::vector<double> Par);
    double IntegralxSmallHsSea(double beta, std::vector<double> Par);
    double IntegralxSmall1HsSea(double beta, std::vector<double> Par);
    double IntegralxSmall2HsSea(double beta, std::vector<double> Par);
    double HsDD(double beta, double alpha);

    //Dterm sea (uds) contribution
    double IntegralDSea(double beta, std::vector<double> Par);
    double IntegralDSeaMx(double beta, std::vector<double> Par);
    double DtermSea(double z, double beta);


};


} //end of namespace

# endif
