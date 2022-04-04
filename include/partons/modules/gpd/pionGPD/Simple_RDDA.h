#ifndef SIMPLERDDA_H
#define SIMPLERDDA_H

/**
 * @file simpleRDDA.h
 * @author Cédric Mezrag (CEA Saclay)
 * @date 17th May 2021
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>


#include "../GPDModule.h"
#include "../../MathIntegratorModule.h"


namespace PARTONS {

class simpleRDDAModel: public PARTONS::GPDModule, public MathIntegratorModule
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
    simpleRDDAModel(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~simpleRDDAModel();
    virtual simpleRDDAModel* clone() const;
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

    simpleRDDAModel(const simpleRDDAModel& other);

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

    std::vector<double> mPara;                                   // PDF-parameters
    double mRDDA_Para;											// RDDA parameter n
    double m_Mx;                                                // -x variable

    double Profile(double alpha, double beta);        // Compute the Radyushkin Profile function
    double SimplePdfAnsatz(double beta);	// Simple PDF ansatz q(x) =  x^a (1-x)^b

    void throwBetaException(const std::string &funcName, double betaValue);

    void initFunctorsForIntegrations();

    NumA::FunctionType1D* m_pIntegralHuVal;
    NumA::FunctionType1D* m_pIntegralHuValMx;
    NumA::FunctionType1D* m_pIntegralHdVal;
    NumA::FunctionType1D* m_pIntegralHdValMx;


    double IntegralHuVal(double beta, std::vector<double> Par);
    double IntegralHuValMx(double beta, std::vector<double> Par);

    double HuValDD(double beta, double alpha);

    double IntegralHdVal(double beta, std::vector<double> Par);
    double IntegralHdValMx(double beta, std::vector<double> Par);

    double HdValDD(double beta, double alpha);

};


} //end of namespace

# endif
