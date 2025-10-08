#ifndef QUARK_DISTRIBUTION_H
#define QUARK_DISTRIBUTION_H

/**
 * @file QuarkDistribution.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 April 2015
 * @version 1.0
 */

#include <string>

#include "../../BaseObject.h"
#include "../QuarkFlavor.h"

namespace PARTONS {

/**
 * @class QuarkDistribution
 *
 * @brief Container to store values of single quark distribution.
 *
 * This class represents a quark distribution at a single kinematic point. For example, it can be GPD of a given type and specific quark flavor at some GPD kinematics.
 *
 * Except the information on the assigned quark flavor, three values can be stored in this container: 1) value of the quark distributions, 2) value of the singlet combination and 3) value of the non-singlet combination. For the definition of singlet and non-singlet combinations see the documentation of QuarkDistribution::m_quarkDistributionPlus and QuarkDistribution::m_quarkDistributionMinus members. Note, that these combinations appear naturally in many GPD-related computations.
 *
 * To see how this class can be used, analyze the following example:
 \code{.cpp}
 //in code, e.g. in one of GPD modules, we calculate values of GPD H for up quarks at x and -x kinematics
 double HuAtX = 2.34;
 double HuAtmX = 1.23;

 //calculate singlet and non-singlet combinations (note: definition is specific for given GPD type!)
 double HuSinglet = HuAtX - HuAtmX;
 double HuNonSinglet = HuAtX + HuAtmX;

 //store my results in QuarkDistribution object
 QuarkDistribution quarkDistribution(QuarkFlavor::UP, HuAtX, HuSinglet, HuNonSinglet);

 //check what is inside
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "My quark distribution contains: " << quarkDistribution.toString());
 \endcode
 which gives via Logger:
 \code
 20-05-2017 09:16:45 [INFO] (example::main) Quark distribution contains: u = 2.34
 u(+) = 1.11
 u(-) = 3.57
 \endcode
 * Check also the documentation of GPDResult and PartonDistribution classes, where QuarkDistribution objects are used extensively.
 */
class QuarkDistribution: public BaseObject {
public:

    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS;

    //QuarkDistribution(); // Not needed because the other constructor uses default values now
    //QuarkDistribution(QuarkFlavor::Type quarkFlavor); // Not needed because the other constructor uses default values now

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    QuarkDistribution(const QuarkDistribution &other);

    /**
     * Assignment constructor.
     * @param quarkFlavor Quark flavor to be assigned.
     * @param quarkDistribution Value of quark distribution.
     * @param quarkDistributionPlus Value of singlet combination, see QuarkDistribution::m_quarkDistributionPlus.
     * @param quarkDistributionMinus Value of non-singlet combination, see QuarkDistribution::m_quarkDistributionMinus.
     */
    QuarkDistribution(QuarkFlavor::Type quarkFlavor = QuarkFlavor::UNDEFINED,
            double quarkDistribution = 0., double quarkDistributionPlus = 0.,
            double quarkDistributionMinus = 0.);

    /**
     * Destructor.
     */
    virtual ~QuarkDistribution();

    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get quark flavor assigned to this object.
     */
    QuarkFlavor getQuarkFlavor() const;

    /**
     * Set quark flavor assigned to this object.
     */
    void setQuarkFlavor(QuarkFlavor quarkFlavorType);

    /**
     * Get value of quark distribution.
     */
    double getQuarkDistribution() const;

    /**
     * Set value of quark distribution.
     */
    void setQuarkDistribution(double quarkDistribution);

    /**
     * Get value of singlet combination.
     */
    double getQuarkDistributionPlus() const;

    /**
     * Set value of singlet combination.
     */
    void setQuarkDistributionPlus(double quarkDistributionPlus);

    /**
     * Get value of non-singlet combination.
     */
    double getQuarkDistributionMinus() const;

    /**
     * Set value of non-singlet combination.
     */
    void setQuarkDistributionMinus(double quarkDistributionMinus);

private:

    /**
     * Quark flavor assigned to this object.
     */
    QuarkFlavor m_quarkFlavor;

    /**
     * Value of quark distribution.
     */
    double m_quarkDistribution;

    /**
     * Value of singlet (plus) combination.
     * In GPD formalism it is C-even combination of single type GPDs, e.g.: <br>
     * \f$H^{q(+)}(x, \xi, t) = H^{q}(x, \xi, t) - H^{q}(-x, \xi, t)\f$ <br>
     * \f$\tilde{H}^{q(+)}(x, \xi, t) = \tilde{H}^{q}(x, \xi, t) + \tilde{H}^{q}(-x, \xi, t)\f$ <br>
     * and analogously for GPDs \f$E\f$ and \f$\tilde{E}\f$. See M. Diehl's https://arxiv.org/abs/hep-ph/0307382 Sec. 3.3.2 for more details.
     */
    double m_quarkDistributionPlus;

    /**
     * Value of non-singlet (minus) combination.
     * In GPD formalism it is C-odd combination of single type GPDs, e.g.: <br>
     * \f$H^{q(-)}(x, \xi, t) = H^{q}(x, \xi, t) + H^{q}(-x, \xi, t)\f$ <br>
     * \f$\tilde{H}^{q(-)}(x, \xi, t) = \tilde{H}^{q}(x, \xi, t) - \tilde{H}^{q}(-x, \xi, t)\f$ <br>
     * and analogously for GPDs \f$E\f$ and \f$\tilde{E}\f$. See M. Diehl's https://arxiv.org/abs/hep-ph/0307382 Sec. 3.3.2 for more details.
     */
    double m_quarkDistributionMinus;
};

} /* namespace PARTONS */

#endif /* QUARK_DISTRIBUTION_H */
