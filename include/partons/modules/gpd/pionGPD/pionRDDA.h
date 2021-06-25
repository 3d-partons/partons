#ifndef pionRDDA_H
#define pionRDDA_H

/**
 * @file pionRDDA.h
 * @author Cédric Mezrag (CEA Saclay)
 * @date 11th June 2021
 * @version 1.0
 *
 * This code present a RDDA model of the pion based on the xfitter pdfs obtained in PHYSICAL REVIEW D 102, 014040 (2020)
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>


#include "../GPDModule.h"
#include "../../MathIntegratorModule.h"


namespace PARTONS {

class pionRDDAModel: public PARTONS::GPDModule, public MathIntegratorModule
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
    pionRDDAModel(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~pionRDDAModel();
    virtual pionRDDAModel* clone() const;
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

    pionRDDAModel(const pionRDDAModel& other);

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

    std::vector<double> m_valPara;                                   // valence PDF-parameters {a,b,N_v}
    std::vector<double> m_seaPara;                                   // sea PDF-parameters {a,b,N_s}
    std::vector<double> m_gPara;                                     // gluons PDF-parameters {a,b,N_g}
    std::vector<double> m_reggePara ;                                // Regge parameters {alpha',A,B}


    double mRDDA_Para;											// RDDA parameter n
    double m_Mx;                                                // -x variable

    double Profile(double alpha, double beta);        // Compute the Radyushkin Profile function
    // x-fitter pdf set
    double valencePdfAnsatz(double beta);	// Valence ansatz u(x) = bar(d) (x) = N_v * x^a (1-x)^b
    double seaPdfAnsatz(double beta);	// Sea ansatz  2 ub + 2 d + s + sb = 6 ub = N_s * x^(a-1) (1-x)^b
    double gluonPdfAnsatz(double beta);	// gluon ansatz  x g(x) = N_g * (1-x)^b

    //reggeized t-depedence
    double tReggeizedAnsatz(double beta);

    void throwBetaException(const std::string &funcName, double betaValue);

    void initFunctorsForIntegrations();

    NumA::FunctionType1D* m_pIntegralHuVal;
    NumA::FunctionType1D* m_pIntegralHuValMx;
    NumA::FunctionType1D* m_pIntegralHdVal;
    NumA::FunctionType1D* m_pIntegralHdValMx;

    NumA::FunctionType1D* m_pIntegralxLargeHsSea;
    NumA::FunctionType1D* m_pIntegralxSmallHsSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HsSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HsSea;
    NumA::FunctionType1D* m_pIntegralxLargeHsSeaMx;

    NumA::FunctionType1D* m_pIntegralxLargeHg;
    NumA::FunctionType1D* m_pIntegralxSmall1Hg;
    NumA::FunctionType1D* m_pIntegralxSmall2Hg;
    NumA::FunctionType1D* m_pIntegralxLargeHgMx;


    double IntegralHuVal(double beta, std::vector<double> Par);
    double IntegralHuValMx(double beta, std::vector<double> Par);
    double HuValDD(double beta, double alpha);

    double IntegralHdVal(double beta, std::vector<double> Par);
    double IntegralHdValMx(double beta, std::vector<double> Par);
    double HdValDD(double beta, double alpha);

    double IntegralxSmall2Hg(double beta, std::vector<double> Par);
    double IntegralxSmall1Hg(double beta, std::vector<double> Par);
    double IntegralxLargeHgMx(double beta, std::vector<double> Par);
    double IntegralxLargeHg(double beta, std::vector<double> Par) ;
    double HsDD(double beta, double alpha);

    double IntegralxLargeHsSea(double beta, std::vector<double> Par);
    double IntegralxLargeHsSeaMx(double beta, std::vector<double> Par);
    double IntegralxSmallHsSea(double beta, std::vector<double> Par);
    double IntegralxSmall1HsSea(double beta, std::vector<double> Par);
    double IntegralxSmall2HsSea(double beta, std::vector<double> Par);
    double HgDD(double beta, double alpha);


};


} //end of namespace

# endif
