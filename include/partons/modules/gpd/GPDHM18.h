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

    double m_m; ///< Mass of the active quark.
    double m_lambda; ///< Mass of the spectator
    double m_p; ///< Parameter controlling the power.
    double m_N; ///< Normalization of the wave-function.

    /** %Double distribution function for GPD E, see Eq. (18) and (19) in Ref. @cite Hwang:2007tb.
     */
    double int_e(double y, double z, double t);
    double intE(double y, std::vector<double> par);
    double intE0(double z, std::vector<double> par);
    double evaluateE(double x);

};

} /* namespace PARTONS */

#endif /* GPD_HM18_H */
