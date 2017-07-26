#ifndef DOUBLE_DISTRIBUTION_MODULE_H
#define DOUBLE_DISTRIBUTION_MODULE_H

/**
 * @file DoubleDistributionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date 30 July 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/double_distribution/DoubleDistributionType.h"
#include "../../beans/List.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../ModuleObject.h"

namespace PARTONS {

class DoubleDistributionResult;
class IncompleteGPDModule;
class RadonInverseModule;

/**
 * @class DoubleDistributionModule
 *
 * @brief
 */

class DoubleDistributionModule: public ModuleObject {
public:
    /**
     * Constructor.
     * See BaseObject class for more info about input parameter.
     *
     * @param className class's name of child class.
     */
    DoubleDistributionModule(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~DoubleDistributionModule();

    /**
     * Clone
     *
     * @return
     */
    virtual DoubleDistributionModule* clone() const = 0;

    virtual DoubleDistributionResult compute(double beta, double alpha,
            double t, double MuF2, double MuR2,
            DoubleDistributionType::Type doubleDistributionType);

    virtual PartonDistribution computeF();
    virtual PartonDistribution computeG();
    virtual PartonDistribution computeK();

    bool isInversionDependent() const;
    void setInversionDependent(bool inversionDependent);
    RadonInverseModule* getRadonInverseModule() const;
    void setRadonInverseModule(RadonInverseModule* pRadonInverse);
    IncompleteGPDModule* getIncompleteGPDModule() const;
    void setIncompleteGPDModule(IncompleteGPDModule* pIncompleteGPD);

    double getAlpha() const;
    void setAlpha(double alpha);
    double getBeta() const;
    void setBeta(double beta);
    double getMuF2() const;
    void setMuF2(double muF2);
    double getMuR2() const;
    void setMuR2(double muR2);
    double getT() const;
    void setT(double t);

    List<DoubleDistributionType> getListOfAvailableDDTypeForComputation() const;

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:
    std::map<DoubleDistributionType::Type,
            PartonDistribution (DoubleDistributionModule::*)()> m_listOfAvailableDoubleDistributionFunctions;
    std::map<DoubleDistributionType::Type,
            PartonDistribution (DoubleDistributionModule::*)()>::iterator m_it;

    /**
     * Copy constructor
     *
     * @param other
     */
    DoubleDistributionModule(const DoubleDistributionModule &other);

    void preCompute(double beta, double alpha, double t, double MuF2,
            double MuR2, DoubleDistributionType::Type doubleDistributionType);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    bool m_inversionDependent;
    RadonInverseModule* m_pRadonInverse;
    IncompleteGPDModule* m_pIncompleteGPD;

    double m_beta;
    double m_alpha;
    double m_t;
    double m_MuF2;
    double m_MuR2;

    DoubleDistributionType::Type m_doubleDistributionType;
};

} /* namespace PARTONS */

#endif /* DOUBLE_DISTRIBUTION_MODULE_H */
