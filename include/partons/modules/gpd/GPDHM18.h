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

//TODO add citation to the bibliography
/**
 * @class GPDHM18
 *
 * The proposition of the scalar di-quark model (SDQM) in the context
 * of GPDs back to the Jakob-Mulders-Rodrigues paper hep-ph/9704335
 * @cite Jakob:1997wg.
 *
 * Later has been studied by Dae Sung Hwang and Dieter Mueller.
 * The short description of the model is given in arXiv:0710.1567
 * @cite Hwang:2007tb. In the Ref. arXiv:1407.16655 @cite Muller:2014tqa
 * full description is given.
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
    double m_N; ///< Normalization of the GPDs.

    /**
     * Function setting the normalization of the GPDs @param m_N
     * according to the Eq. (22) Ref. @cite Hwang:2007tb.     *
     */
    void Normalize();

    /**
     * Integral given in Eq. (22) Ref. @cite Hwang:2007tb.
     *
     * @param y
     * @param par optional parameters of integration, not used.
     * @return 1/m_N
     */
    double IntNorm(double y, std::vector<double> par);
    NumA::FunctionType1D* m_pint_IntNorm; ///< Functor related to IntNorm.

    /** %Double distribution function for GPD E. Definition can be found
     * in Eqs. (18) and (19) in Ref. @cite Hwang:2007tb.
     *
     * @param y %Double distribution parameter y.
     * @param z %Double distribution parameter z.
     * @param t %Double distribution parameter t.
     * @return GPD E
     */
    double DD_E(double y, double z, double t);
    double IntE(double y, std::vector<double> par); ///< Integrant of GPD E for xi <> 0.
    double IntE0(double z, std::vector<double> par); ///< Integrant of GPD E for xi == 0.
    NumA::FunctionType1D* m_pint_IntE; ///< Functor related to IntE.
    NumA::FunctionType1D* m_pint_IntE0; ///< Functor related to IntE0.

    /**
     * %Double distribution function for GPD H. Definition can be found
     * in Eqs. (21) and (19) in Ref. @cite Hwang:2007tb.
     *
     * @param y %Double distribution parameter y.
     * @param z %Double distribution parameter z.
     * @param t %Double distribution parameter t.
     * @return GPD H
     */
    double DD_H(double y, double z, double t);
    double IntH(double y, std::vector<double> par); ///< Integrant of GPD H for xi <> 0.
    double IntH0(double z, std::vector<double> par); ///< Integrant of GPD H for xi == 0.
    NumA::FunctionType1D* m_pint_IntH; ///< Functor related to IntH.
    NumA::FunctionType1D* m_pint_IntH0; ///< Functor related to IntH0.

    /** %Double distribution function for GPD Et. Definition can be found
     * in Eqs. (4.23) and (4.27) in Ref. @cite Muller:2014tqa.
     *
     * @param y %Double distribution parameter y.
     * @param z %Double distribution parameter z.
     * @param t %Double distribution parameter t.
     * @return GPD Et
     */

    double DD_Et(double y, double z, double t);
    double IntEt(double y, std::vector<double> par);
    double IntEt0(double y, std::vector<double> par); ///< Integrant of GPD Et for xi <> 0.
    NumA::FunctionType1D* m_pint_IntEt; ///< Functor related to IntEt.
    NumA::FunctionType1D* m_pint_IntEt0; ///< Functor related to IntE for xi <> 0.

    /**
     * %Double distribution function for GPD Ht. Definition can be found
     * in Eqs. (4.23) and (4.26) in Ref. @cite Muller:2014tqa.
     *
     * @param y %Double distribution parameter y.
     * @param z %Double distribution parameter z.
     * @param t %Double distribution parameter t.
     * @return GPD Ht
     */
    double DD_Ht(double y, double z, double t);
    double IntHt(double y, std::vector<double> par); ///< Integrant of GPD Ht for xi <> 0.
    double IntHt0(double z, std::vector<double> par); ///< Integrant of GPD Ht for xi == 0.
    NumA::FunctionType1D* m_pint_IntHt; ///< Functor related to IntHt.
    NumA::FunctionType1D* m_pint_IntHt0; ///< Functor related to IntHt0.

    /**
     * Compute GPDs using %double distribution function
     * by integrating p_fun0 or p_fun function depending on kinematics.
     *
     * @param x
     * @param p_fun0 integrate this function if xi == 0
     * @param p_fun  integrate this function if xi <> 0
     * @return computed GPD
     */
    double evaluate(double x, NumA::FunctionType1D* p_fun0,
            NumA::FunctionType1D* p_fun);
    PartonDistribution compute(NumA::FunctionType1D* p_fun0,
            NumA::FunctionType1D* p_fun);

    void initializeFunctorsForIntegrations(); ///< Initialize functors.
    void deleteFunctorsForIntegrations(); ///< Delete functors.

};

} /* namespace PARTONS */

#endif /* GPD_HM18_H */
