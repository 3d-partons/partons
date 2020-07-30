#ifndef GPD_MMS13_H
#define GPD_MMS13_H

/**
 * @file GPDMMS13.h
 * @author Pawel Sznajder (IPNO)
 * @date March 29, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <LHAPDF/LHAPDF.h>

#include "../../beans/QuarkFlavor.h"
#include "../MathIntegratorModule.h"
#include "GPDModule.h"

namespace PARTONS {

/**
 * @class GPDMMS13
 *
 * Mezrag-Moutarde-Sabatié GPD model. For the reference, see arxiv:1304.7645 @cite Mezrag:2013mya.
 *
 * Available GPD types: H, E.
 */
class GPDMMS13: public GPDModule, public MathIntegratorModule {

public:

    static const std::string PARAM_NAME_SET_NAME;
    static const std::string PARAM_NAME_MEMBER;

    static const std::string PARAMETER_NAME_MMS13MODEL_NHpE; ///< Name of parameter to set #m_NHpE via configure()
    static const std::string PARAMETER_NAME_MMS13MODEL_NE; ///< Name of parameter to set #m_NE via configure()
    static const std::string PARAMETER_NAME_MMS13MODEL_C; ///< Name of parameter to set #m_C via configure()

    static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDMMS13(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GPDMMS13();
    virtual GPDMMS13* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString() const;

    std::string getSetName() const;
    int getMember() const;

    void setSetName(const std::string &setname);
    void setMember(const int &member);

    /** D term function.
     @param zeta Variable equals x/xi.
     */
    double DTerm(double zeta) const;

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    GPDMMS13(const GPDMMS13& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();

private:

    int m_NHpE; ///< %Double distribution profile function parameter for H_DD + E_DD.
    int m_NE; ///< %Double distribution profile function parameter for E+.
    double m_C; ///< Scaling parameter for D term.

    std::string m_setName;
    int m_member;
    LHAPDF::PDF* m_pForward;

    /** Forward limit of GPD H.
     @param beta %Double distribution parameter.
     @param flavor Quark flavor.
     */
    double forwardHval(double beta, QuarkFlavor::Type flavor) const;

    /** Forward limit of GPD E.
     @param beta %Double distribution parameter.
     @param flavor Quark flavor.
     */
    double forwardEval(double beta, QuarkFlavor::Type flavor) const;

    /** Functional form of forward limit of GPD E.
     @param beta %Double distribution parameter.
     @param kappa Anomalous magnetic moment.
     @param nu power for (1 - beta)^nu term.
     @param mu power for beta^-mu term.
     */
    double forwardEvalFunction(double beta, double kappa, double nu,
            double mu) const;

    /** %Double distribution profile function.
     @param beta, alpha %Double distribution variables.
     @param N Profile parameter.
     */
    double profileFunction(double beta, double alpha, int N);

    /** H_DD + E_DD component.
     @param x GPD variable.
     @param flavor Quark flavor.
     @param N Profile parameter.
     */
    double HpEDDVal(double x, QuarkFlavor::Type flavor, int N);

    /** %Double distribution function for H_DD + E_DD component ((x-beta)/xi-alpha = 0 line).
     @param beta %Double distribution variable.
     @param par Additional parameters.
     */
    double IntHpEDDval(double beta, std::vector<double> par);

    /** E+ component.
     @param x GPD variable.
     @param flavor Quark flavor.
     @param N Profile parameter.
     */
    double EValPlus(double x, QuarkFlavor::Type flavor, int N);

    /** %Double distribution function for E+ component ((x-beta)/xi-alpha = 0 and x-alpha*xi lines).
     @param alpha %Double distribution variable.
     @param par Additional parameters.
     */
    double IntEvalPlusAB(double alpha, std::vector<double> par);

    /** %Double distribution function for E+ component ((x-beta)/xi-alpha = 0 line).
     @param alpha %Double distribution variable.
     @param par Additional parameters.
     */
    double IntEvalPlusA(double alpha, std::vector<double> par);

    /** %Double distribution function for E+ component (x/xi-alpha = 0 line).
     @param alpha %Double distribution variable.
     @param par Additional parameters.
     */
    double IntEvalPlusB(double alpha, std::vector<double> par);

    NumA::FunctionType1D* m_pint_IntHpEDDval; ///< Functor related to  IntHpEDDval().
    NumA::FunctionType1D* m_pint_IntEvalPlusAB; ///< Functor related to IntEvalPlusAB().
    NumA::FunctionType1D* m_pint_IntEvalPlusA; ///< Functor related to IntEvalPlusA().
    NumA::FunctionType1D* m_pint_IntEvalPlusB; ///< Functor related to IntEvalPlusB().

    /** Initialize functors.
     */
    void initFunctorsForIntegrations();
};

} /* namespace PARTONS */

#endif /* GPD_MMS13_H */
