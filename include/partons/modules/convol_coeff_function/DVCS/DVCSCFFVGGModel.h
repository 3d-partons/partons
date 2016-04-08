/*
 * DVCSCFFVGGModel.h
 *
 *  Created on: Dec 20, 2015
 *      Author: Michel Guidal and Pawel Sznajder (IPNO)
 */

#ifndef DVCSCFFVGGMODEL_H_
#define DVCSCFFVGGMODEL_H_

#include <complex>
#include <string>
#include <vector>

#include "DVCSConvolCoeffFunctionModule.h"

namespace NumA {
class FunctionType1D;
}
class GPDResult;

class DVCSCFFVGGModel: public DVCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string PARAMETER_NAME_EPS;

    DVCSCFFVGGModel(const std::string &className);  ///< constructor

    virtual DVCSCFFVGGModel* clone() const;    ///< clone

    virtual ~DVCSCFFVGGModel(); ///< destructor

    virtual void resolveObjectDependencies();    ///< init function

    virtual void configure(const ElemUtils::Parameters &parameters);    ///< configure

protected:

    DVCSCFFVGGModel(const DVCSCFFVGGModel &other);  ///< copy constructor

    virtual void initModule();  ///< init module
    virtual void isModuleWellConfigured();  ///< check if well configured

private:

    double eps_cffint; ///< step to skip x = xi singularity

    virtual std::complex<double> computeUnpolarized(); ///< compute CFF for unpolarized GPDs
    virtual std::complex<double> computePolarized(); ///< compute CFF for polarized GPDs

    double xixit;   ///< value of GPD at (xi, xi, t)

    std::complex<double> calculate_direct(); ///< calculate contribution from direct Feynman diagram
    std::complex<double> calculate_crossed(); ///< calculate contribution from crossed Feynman diagram
    double calculate_gpd_combination(GPDResult gpdResult); ///< compute combination of GPDs
    void calculate_xixit_value();   ///< compute GPDs at (xi, xi, t)
    double intd_vector_part(double x, std::vector<double> par); ///< wrapper for integral of GPD over x for direct Feynman diagram
    double intc_vector_part(double x, std::vector<double> par); ///< wrapper for integral of GPD over x for crossed Feynman diagram

    NumA::FunctionType1D* m_pIntd_vector_part;
    NumA::FunctionType1D* m_pIntc_vector_part;

    void initFunctorsForIntegrations();
};

#endif /* DVCSCFFVGGMODEL_H_ */
