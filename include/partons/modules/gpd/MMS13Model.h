/*
 * MMS13Model.h
 *
 *  Created on: Mar 29, 2015
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef MMS13MODEL_H_
#define MMS13MODEL_H_

#include <string>
#include <vector>

#include "../../beans/QuarkFlavor.h"
#include "../GPDModule.h"
#include "../MathIntegratorModule.h"

class c_mstwpdf;

class MMS13Model: public GPDModule, public MathIntegratorModule {

public:

    static const std::string PARAMETER_NAME_MMS13MODEL_NHpE;
    static const std::string PARAMETER_NAME_MMS13MODEL_NE;
    static const std::string PARAMETER_NAME_MMS13MODEL_C;

    static const unsigned int classId;
    MMS13Model(const std::string &className);
    virtual ~MMS13Model();
    virtual MMS13Model* clone() const;
    virtual void init();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString();

protected:

    MMS13Model(const MMS13Model& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();

private:

    int m_NHpE;   ///< profile parameter for GPD H_DD + E_DD
    int m_NE;   ///< profile parameter for GPD E
    double m_C; ///<

    c_mstwpdf* m_Forward;   ///< pdfs

    double forwardHval(double beta, QuarkFlavor::Type flavor) const; ///< forward limit of GPD H
    double forwardEval(double beta, QuarkFlavor::Type flavor) const; ///< forward limit of GPD E
    double forwardEvalFunction(double beta, double kappa, double nu,
            double mu) const; ///< function to calculate forward limit of GPD E
    double profileFunction(double beta, double alpha, int N); ///< profile function
    double DTerm(double x) const;    ///< D term

    double HpEDDVal(double x, QuarkFlavor::Type flavor, int N); ///< GPD H_DD + E_DD
    double IntHpEDDval(double beta, std::vector<double> par); ///< DD integral for H_DD + E_DD ((x-beta)/xi-alpha = 0 line)

    double EValPlus(double x, QuarkFlavor::Type flavor, int N); ///< "plus" component of GPD E
    double IntEvalPlusAB(double alpha, std::vector<double> par); ///< DD integral for "plus" component of GPD E ((x-beta)/xi-alpha = 0 and x-alpha*xi lines)
    double IntEvalPlusA(double alpha, std::vector<double> par); ///< DD integral for "plus" component of GPD E ((x-beta)/xi-alpha = 0 line)
    double IntEvalPlusB(double alpha, std::vector<double> par); ///< DD integral for "plus" component of GPD E (x/xi-alpha = 0 line)

    NumA::FunctionType1D* m_pint_IntHpEDDval; ///< functor related to IntHpEDDval()
    NumA::FunctionType1D* m_pint_IntEvalPlusAB; ///< functor related to IntEvalPlusAB()
    NumA::FunctionType1D* m_pint_IntEvalPlusA; ///< functor related to IntEvalPlusA()
    NumA::FunctionType1D* m_pint_IntEvalPlusB; ///< functor related to IntEvalPlusB()

    void initFunctorsForIntegrations();
};

#endif /* MMS13MODEL_H_ */
