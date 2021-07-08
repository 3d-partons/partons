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
    std::vector<double> m_reggeParaVal ;                                // Regge parameters {alpha',A,B}
    std::vector<double> m_reggeParaSea ;                                // Regge parameters {alpha',A,B}



    double mRDDA_Para;											// RDDA parameter n
    double m_Mx;                                                // -x variable

    double Profile(double alpha, double beta);        // Compute the Radyushkin Profile function
    // x-fitter pdf set
    double valencePdfAnsatz(double beta);	// Valence ansatz u(x) = bar(d) (x) = N_v * x^a (1-x)^b
    double seaPdfAnsatz(double beta);	// Sea ansatz  2 ub + 2 d + s + sb = 6 ub = N_s * x^(a-1) (1-x)^b
    double gluonxPdfAnsatz(double beta);	// gluon ansatz  x g(x) = N_g * (1-x)^b

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
    double Integralgluon(double beta, std::vector<double> Par);
    double IntegralgluonMx(double beta, std::vector<double> Par);
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
