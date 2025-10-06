#ifndef GPD_RESULT_H
#define GPD_RESULT_H

/**
 * @file GPDResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 April 2015
 * @version 1.0
 */

#include <map>
#include <string>
#include <vector>

#include "../parton_distribution/PartonDistribution.h"
#include "../Result.h"
#include "GPDKinematic.h"
#include "GPDType.h"

namespace PARTONS {

/**
 * @class GPDResult
 *
 * @brief Class representing single result of GPD computation.
 *
 * This class is used to store results of a single GPD computation. In other words, the class stores a collection of PartonDistribution objects, with each of them containing a part of the result defined for a specific GPD type. This is illustrated by the following example:
 \code{.cpp}
 //evaluate exemplary GPD result

 //retrieve GPD service
 GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //define GPD kinematics used in computation
 GPDKinematic gpdKinematic(-0.1, 0.05, 0., 2., 2.);

 //define list of GPD types to be computed
 List<GPDType> gpdTypeList;
 gpdTypeList.add(GPDType::H);
 gpdTypeList.add(GPDType::E);

 //evaluate
 GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel, gpdTypeList);

 //retrieve value of GPD H for up quarks:
 double Hu;
 //1. retrieve a reference to PartonDistribution object that stores the part of result for GPD type H
 const PartonDistribution& partonDistributionH = gpdResult.getPartonDistribution(GPDType::H);
 //2. retrieve a reference to QuarkDistribution object that stores the part of result for quark type u
 const QuarkDistribution& quarkDistributionHu = partonDistributionH.getQuarkDistribution(QuarkFlavor::UP);
 //3. retrieve the value
 Hu = quarkDistributionHu.getQuarkDistribution();

 //or simply
 Hu = gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution();

 //retrieve value of GPD E for gluons:
 double Eg;
 //1. retrieve a reference to PartonDistribution object that stores the part of result for GPD type E
 const PartonDistribution& partonDistributionE = gpdResult.getPartonDistribution(GPDType::E);
 //2. retrieve a reference to GluonDistribution object that stores the part of result for gluons
 const GluonDistribution& gluonDistributionE = partonDistributionE.getGluonDistribution();
 //3. retrieve the value
 Eg = gluonDistributionE.getGluonDistribution();

 //or simply
 Eg = gpdResult.getPartonDistribution(GPDType::E).getGluonDistribution().getGluonDistribution();
 \endcode
 * See the documentation of QuarkDistribution and GluonDistribution classes to check what kind of information you can access in objects of those types.
 */
class GPDResult: public Result<GPDKinematic> {

public:

    /**
     * Default constructor.
     */
    GPDResult();

    /**
     * Assignment constructor.
     * @param kinematic GPD kinematics to be assigned.
     */
    GPDResult(const GPDKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDResult(const GPDResult &other);

    /**
     * Destructor.
     */
    virtual ~GPDResult();

    virtual std::string toString() const;

    /**
     * Add parton distribution associated to given GPD type.
     * @param gpdType GPD type of parton distribution to be inserted.
     * @param partonDistribution Parton distribution to be added.
     */
    void addPartonDistribution(GPDType::Type gpdType,
            const PartonDistribution& partonDistribution);

    /**
     * Get reference to parton distribution associated to given GPD type.
     * @param gpdType GPD type associated to parton distribution to be selected.
     * @return Reference to selected parton distribution.
     */
    const PartonDistribution& getPartonDistribution(
            GPDType::Type gpdType) const;

    /**
     * Check if the object stores parton distribution of given GPD type.
     * @param gpdType GPD type to be checked.
     * @return True if the object stores parton distribution of given GPD type, otherwise false.
     * @see GPDResult::getLastAvailable()
     */
    bool isAvailable(const GPDType::Type &gpdType) const;

    /**
     * Get reference to parton distribution marked by the last call of GPDResult::isAvailible() function.
     * @return Reference to selected parton distribution.
     * @see GPDResult::isAvailible()
     */
    PartonDistribution& getLastAvailable() const;

    /**
     * Get list of GPD types associated to stored parton distributions.
     * @return Vector of associated types.
     */
    std::vector<GPDType> listGPDTypeComputed() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to map containing stored parton distributions distinguished by associated GPD types.
     */
    const std::map<GPDType::Type, PartonDistribution>& getPartonDistributions() const;

    /**
     * Set map containing stored parton distributions distinguished by associated GPD types.
     */
    void setPartonDistributions(
            const std::map<GPDType::Type, PartonDistribution>& partonDistributions);

private:

    /**
     * Map containing stored parton distributions distinguished by associated GPD types.
     */
    std::map<GPDType::Type, PartonDistribution> m_partonDistributions;

    /**
     * Iterator used to mark a specific entry in GPDResult::m_partonDistributions.
     */
    mutable std::map<GPDType::Type, PartonDistribution>::const_iterator m_it;
};

} /* namespace PARTONS */

#endif /* GPD_RESULT_H */
