#ifndef DVCSCFFVGGMODEL_H_
#define DVCSCFFVGGMODEL_H_

/**
 * @file DVCSCFFVGGModel.h
 * @author Pawel Sznajder (IPNO)
 * @author Michel Guidal (IPNO)
 * @date Dec 20, 2015
 * @version 1.0
 */

#include <complex>
#include <string>
#include <vector>

#include "DVCSConvolCoeffFunctionModule.h"

class PartonDistribution;

namespace NumA {
class FunctionType1D;
}

/**
 * @class DVCSCFFVGGModel
 *
 * VGG DVCS/CFF model.
 *
 * Model implemented for the cross-check between original VGG code and PARTONS VGG1999Model process module.
 * It is not recommended to use this model for the values xi < 0.01 as a loss of numerical precision may appear.
 *
 * Available CFF types: H, E, Ht, Et.
 */
class DVCSCFFVGGModel: public DVCSConvolCoeffFunctionModule {

public:

    static const std::string PARAMETER_NAME_EPS; ///< Name of parameter to set #eps_cffint via configure()

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     *
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFVGGModel(const std::string &className);  ///< constructor

    virtual DVCSCFFVGGModel* clone() const;
    /**
     * Default destructor.
     */
    virtual ~DVCSCFFVGGModel();
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    DVCSCFFVGGModel(const DVCSCFFVGGModel &other);  ///< copy constructor

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    double eps_cffint; ///< Step to avoid the evaluation at x = xi
    double xixit; ///< Value of GPD at (xi, xi, t)

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    /** Calculate contribution coming from the direct diagram
     */
    std::complex<double> calculate_direct();

    /** Calculate contribution coming from the crossed diagram
     */
    std::complex<double> calculate_crossed();

    /** Compute sum of singlet combinations of GPDs weighted by quark charges
     * @param partonDistribution Singlet combinations to be calculated
     */
    double calculate_gpd_combination(
            const PartonDistribution &partonDistribution);

    /** Compute GPDs at (xi, xi, t)
     */
    void calculate_xixit_value();

    /** Wrapper for the x integral and the direct diagram
     * @param x GPD variable
     * @param par Additional parameters
     */
    double intd_vector_part(double x, std::vector<double> par);

    /** Wrapper for the x integral and the crossed diagram
     * @param x GPD variable
     * @param par Additional parameters
     */
    double intc_vector_part(double x, std::vector<double> par);

    NumA::FunctionType1D* m_pIntd_vector_part; ///< Functor related to intd_vector_part()
    NumA::FunctionType1D* m_pIntc_vector_part; ///< Functor related to intc_vector_part()

    /** Initialize functors
     */
    void initFunctorsForIntegrations();
};

#endif /* DVCSCFFVGGMODEL_H_ */
