#ifndef PARTON_DISTRIBUTION_H
#define PARTON_DISTRIBUTION_H

/**
 * @file PartonDistribution.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 April 2015
 * @version 1.0
 */

#include <map>
#include <string>
#include <vector>

#include "../../utils/compare/ComparisonReport.h"
#include "../List.h"
#include "../QuarkFlavor.h"
#include "GluonDistribution.h"
#include "QuarkDistribution.h"

class ComparisonReport;

/**
 * @class PartonDistribution
 *
 * @brief Parton distributions for single physics object.
 *
 * This class is used to store parton distributions for a single physics object (e.g. GPD of a given type). It contains quark distributions (QuarkDistribution objects) and a gluon distribution (GluonDistribution object). Each quark distribution is defined for a different quark flavor. This is illustrated by the following example:
 \code{.cpp}
 //create new parton distribution
 PartonDistribution partonDistribution;

 //since it is an empty container now, let us fill it:

 //with quark distributions for up and down quarks
 QuarkDistribution quarkDistributionUp(QuarkFlavor::UP, 1.123);
 QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN, 0.456);

 partonDistribution.addQuarkDistribution(quarkDistributionUp);
 partonDistribution.addQuarkDistribution(quarkDistributionDown);

 //with gluon distribution
 GluonDistribution gluonDistribution(2.345);

 partonDistribution.setGluonDistribution(gluonDistribution);

 //check what is inside
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Parton distribution contains: " << partonDistribution.toString());
 \endcode
which gives via Logger:
\code
20-05-2017 09:29:44 [INFO] (example::main) Parton distribution contains: [PartonDistribution]
m_className = PartonDistribution - m_objectId = 195 indexId = -1
GluonDistribution = 2.345
u = 1.123
u(+) = 0
u(-) = 0
d = 0.456
d(+) = 0
d(-) = 0
\endcode
 * See the documentation of QuarkDistribution and GluonDistribution classes to check what kind of information you can access in objects of those types. Check also GPDResult class, where QuarkDistribution objects are used extensively.
 */
class PartonDistribution: public BaseObject {
public:

    /**
     * Default constructor.
     */
    PartonDistribution();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    PartonDistribution(const PartonDistribution &other);

    /**
     * Destructor.
     */
    virtual ~PartonDistribution();

    /**
     * Add quark distribution associated to given quark flavor that is defined in the object to be added.
     * @param quarkDistribution Reference to quark distribution to be added.
     */
    void addQuarkDistribution(const QuarkDistribution &quarkDistribution);

    /**
     * Add quark distribution associated to given quark flavor that is defined in the object to be added.
     * @param quarkDistribution Reference to quark distribution to be added.
     */
    void addQuarkDistribution(QuarkDistribution &quarkDistribution);

    /**
     * Get reference to quark distribution associated to given quark flavor.
     * @param quarkFlavorType Quark flavor associated to quark distribution to be selected.
     * @return Reference to selected quark distribution.
     */
    const QuarkDistribution& getQuarkDistribution(
            QuarkFlavor::Type quarkFlavorType) const;

    /**
     * Get list of quark flavors associated to stored quark distributions.
     * @return Vector of associated flavors.
     */
    std::vector<QuarkFlavor::Type> listTypeOfQuarkFlavor();

    /**
     * Get number of quark distributions stored in this object.
     * @return Number of quark distributions stored in this object.
     */
    unsigned int getQuarkDistributionsSize() const;

    /**
     * Get list of stored quark distributions.
     * @return Retrieved List of QuarkDistribution objects.
     */
    List<QuarkDistribution> getListOfQuarkDistribution() const;

    /**
     * Get sum of singlet values for stored quark distributions.
     * Return<br>
     * \f$\sum_q F^{q(+)}\f$ <br>
     * where F is physics object (e.g. GPD) for which distributions are defined.<br>
     * <br>
     * Note that sum elements are not weighted here by square of corresponding quark charges.
     * @return Sum of singlet values.
     */
    double getSinglet();

    virtual std::string toString() const;

    /**
     * Compare to other PartonDistribution object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const PartonDistribution &referenceObject,
            std::string parentObjectInfo = "") const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to map containing stored quark distributions distinguished by associated quark flavors.
     */
    const std::map<QuarkFlavor::Type, QuarkDistribution>& getQuarkDistributions() const;

    /**
     * Set map containing stored quark distributions distinguished by associated quark flavors.
     */
    void setQuarkDistributions(
            const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkDistributions);

    /**
     * Get reference to gluon distribution.
     */
    const GluonDistribution& getGluonDistribution() const;

    /**
     * Set gluon distribution.
     */
    void setGluonDistribution(const GluonDistribution &gluonDistribution);

private:

    /**
     * Map containing stored quark distributions distinguished by associated quark flavors.
     */
    std::map<QuarkFlavor::Type, QuarkDistribution> m_quarkDistributions;

     /**
     * Gluon distribution.
     */
    GluonDistribution m_gluonDistribution;
};

#endif /* PARTON_DISTRIBUTION_H */
