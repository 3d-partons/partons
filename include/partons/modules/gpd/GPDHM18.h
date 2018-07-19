#ifndef GPD_HM18_H
#define GPD_HM18_H

/**
 * @file GPDHM18.h
 * @author Arkadiusz P. Trawiński (CEA)
 * @date July 2, 2018
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "GPDModule.h"

namespace PARTONS {

//TODO change desription
/**
 * @class GPDHM18
 *
 * GPD model based on D.S. Hwang and Dieter Mueller paper.
 * For the reference, see arXiv:0710.1567 @cite Hwang:2007tb.
 *
 * Available GPD types: H, Ht, E, Et.
 */

class GPDHM18: public GPDModule, public MathIntegratorModule {
public:
    static const std::string PARAMETER_NAME_HM18MODEL_M; ///< Name of parameter to set #m_M via configure()
    static const std::string PARAMETER_NAME_HM18MODEL_m; ///< Name of parameter to set #m_m via configure()
    static const std::string PARAMETER_NAME_HM18MODEL_lambda; ///< Name of parameter to set #m_lambda via configure()
    static const std::string PARAMETER_NAME_HM18MODEL_p; ///< Name of parameter to set #m_p via configure()
    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;
    /**
     * Default constructor.
     * @param className Name of class.
     */
    GPDHM18(const std::string& className);
    /**
     * Destructor.
     */
    virtual ~GPDHM18();
    virtual GPDHM18* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
protected:
    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDHM18(const GPDHM18& other);
    virtual void isModuleWellConfigured();
    virtual void initModule();
    virtual PartonDistribution computeH();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeEt();

private:
    double m_M; ///< Mass of the proton;
    double m_m; ///< Mass of the active quark.
    double m_lambda; ///< Mass of the spectator
    double m_p; ///< Parameter controlling the power.
    double m_N; ///< Normalization of the wave-function.

    void Normalize();
    double IntNorm(double y, std::vector<double> par);
    NumA::FunctionType1D* m_pint_IntNorm; ///< Functor related to IntNorm.


    /** %Double distribution function for GPD E, see Eqs. (18) and (19) in Ref. @cite Hwang:2007tb.
     */
    double DD_E(double y, double z, double t);
    double IntE(double y, std::vector<double> par);
    double IntE0(double z, std::vector<double> par);
    NumA::FunctionType1D* m_pint_IntE; ///< Functor related to IntE for xi <> 0.
    NumA::FunctionType1D* m_pint_IntE0; ///< Functor related to IntE for xi = 0.

    /** %Double distribution function for GPD H, see Eqs. (21) and (19) in Ref. @cite Hwang:2007tb.
     */
    double DD_H(double y, double z, double t);
    double IntH(double y, std::vector<double> par);
    double IntH0(double z, std::vector<double> par);
    NumA::FunctionType1D* m_pint_IntH; ///< Functor related to IntE for xi <> 0.
    NumA::FunctionType1D* m_pint_IntH0; ///< Functor related to IntE for xi = 0.

    /** %Double distribution function for GPD Et.
     */
    double DD_Et(double y, double z, double t);
    double IntEt(double y, std::vector<double> par);
    double IntEt0(double z, std::vector<double> par);
    NumA::FunctionType1D* m_pint_IntEt; ///< Functor related to IntE for xi <> 0.
    NumA::FunctionType1D* m_pint_IntEt0; ///< Functor related to IntE for xi = 0.

    /** %Double distribution function for GPD Ht.
     */
    double DD_Ht(double y, double z, double t);
    double IntHt(double y, std::vector<double> par);
    double IntHt0(double z, std::vector<double> par);
    NumA::FunctionType1D* m_pint_IntHt; ///< Functor related to IntE for xi <> 0.
    NumA::FunctionType1D* m_pint_IntHt0; ///< Functor related to IntE for xi = 0.


    double evaluate(double x, NumA::FunctionType1D* p_fun0, NumA::FunctionType1D* p_fun);
    PartonDistribution compute(NumA::FunctionType1D* p_fun0, NumA::FunctionType1D* p_fun);

    void initializeFunctorsForIntegrations(); ///< Initialize functors.
    void deleteFunctorsForIntegrations(); ///< Delete functors.

};

} /* namespace PARTONS */

#endif /* GPD_HM18_H */
