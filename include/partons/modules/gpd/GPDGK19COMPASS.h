#ifndef GPD_GK19_COMPASS_H
#define GPD_GK19_COMPASS_H

/**
 * @file GPDGK19COMPASS.h
 * @author Kemal Tezgin
 * @date 2019
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "GPDGK16.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

namespace PARTONS {

/**
 * @class GPDGK19COMPASS
 *
 * @brief TODO
 *
 * TODO
 */

class GPDGK19COMPASS: public GPDGK16, public MathIntegratorModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDGK19COMPASS(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDGK19COMPASS();

    virtual GPDGK19COMPASS* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     */
    GPDGK19COMPASS(const GPDGK19COMPASS& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    double HtUp(double rho, std::vector<double> par);
    double HtDown(double rho, std::vector<double> par);

    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();
    virtual PartonDistribution computeHTrans();
    virtual PartonDistribution computeEbarTrans();

    void calculateHtCoefs();
    void calculateEtCoefs();
    void calculateHTransCoefs();
    void calculateETransCoefs();

private:

    double kHtgluon;                 ///< Exponent of correlated x-t dependence.
    double kHtsea;                   ///< Exponent of correlated x-t dependence.
    double kHtuval;                  ///< Exponent of correlated x-t dependence.
    double kHtdval;                  ///< Exponent of correlated x-t dependence.

    double kEtgluon;                 ///< Exponent of correlated x-t dependence.
    double kEtsea;                   ///< Exponent of correlated x-t dependence.
    double kEtuval;                  ///< Exponent of correlated x-t dependence.
    double kEtdval;                  ///< Exponent of correlated x-t dependence.

    double kHTransgluon;             ///< Exponent of correlated x-t dependence.
    double kHTranssea;               ///< Exponent of correlated x-t dependence.
    double kHTransuval;              ///< Exponent of correlated x-t dependence.
    double kHTransdval;              ///< Exponent of correlated x-t dependence.

    double kETransgluon;             ///< Exponent of correlated x-t dependence.
    double kETranssea;               ///< Exponent of correlated x-t dependence.
    double kETransuval;              ///< Exponent of correlated x-t dependence.
    double kETransdval;              ///< Exponent of correlated x-t dependence.

    std::vector<double> Etuval1tab;         ///< Etval1(i=0,1,2,3) for valence u
    std::vector<double> Etdval1tab;         ///< Etval1(i=0,1,2,3) for valence d
    std::vector<double> Etuval1mtab; ///< Etval1(i=0,1,2,3) for valence u for -xb
    std::vector<double> Etdval1mtab; ///< Etval1(i=0,1,2,3) for valence d for -xb

    std::vector<double> HTransuval1tab; ///< HTransval1(i=0,1,2,3,4,5) for valence u
    std::vector<double> HTransdval1tab; ///< HTransval1(i=0,1,2,3,4,5) for valence d
    std::vector<double> HTransuval1mtab; ///< HTransval1(i=0,1,2,3,4,5) for valence u for -xb
    std::vector<double> HTransdval1mtab; ///< HTransval1(i=0,1,2,3,4,5) for valence d for -xb

    std::vector<double> ETransuval1tab;   ///< ETransval1(i=0,1,2) for valence u
    std::vector<double> ETransdval1tab;   ///< ETransval1(i=0,1,2) for valence d
    std::vector<double> ETransuval1mtab; ///< ETransval1(i=0,1,2) for valence u for -xb
    std::vector<double> ETransdval1mtab; ///< ETransval1(i=0,1,2) for valence d for -xb

    void calculateHtKas();
    void calculateEtKas();
    void calculateHTransKas();
    void calculateETransKas();

    double ValenceExpansion(double x, double i, double k);

    /**
     * Initialize functors.
     */
    void initFunctorsForIntegrations();

    NumA::FunctionType1D* m_pint_HtUp;
    NumA::FunctionType1D* m_pint_HtDown;
};

} /* namespace PARTONS */

#endif /* GPD_GK19_COMPASS_H */
