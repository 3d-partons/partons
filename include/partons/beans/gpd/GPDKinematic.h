#ifndef GPD_KINEMATIC_H
#define GPD_KINEMATIC_H

/**
 * @file GPDKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 26, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <string>

#include "../kinematic/KinematicType.h"
#include "../Kinematic.h"

namespace ElemUtils {
class Packet;
class Parameters;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class GPDKinematic
 *
 * @brief Class representing single GPD kinematics.
 *
 * This class represents a single GPD kinematics (x, xi, t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$) to be used in all kinds of computations done within PARTONS GPD layer.
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //evaluate exemplary GPD result

 //retrieve GPD service
 GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //define GPD kinematics to be used in computation
 //here we are using the constructor that allows us to assign values of x, xi, t, muF2 and muR2 immediately
 GPDKinematic gpdKinematic(-0.1, 0.05, 0., 2., 2.);

 //define list of GPD types to be computed
 List<GPDType> gpdTypeList;
 gpdTypeList.add(GPDType::H);

 //evaluate
 GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel, gpdTypeList);

 //print kinematics and result
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Kinematics:\n" << gpdKinematic.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Result:\n" << gpdResult.toString());
 \endcode
 which gives via Logger:
 \code
 19-05-2017 02:10:26 [INFO] (example::main) Kinematics:
 [GPDKinematic]
 m_className = GPDKinematic - m_objectId = 199 indexId = -1
 m_x = -0.1 m_xi = 0.05 m_t = 0 m_MuF2 = 2(Gev2) m_MuR2 = 2(Gev2)
 19-05-2017 02:10:26 [INFO] (example::main) Result:
 [GPDResult]
 m_className = GPDResult - m_objectId = 201 indexId = -1
 [GPDKinematic]
 m_className = GPDKinematic - m_objectId = 199 indexId = -1
 m_x = -0.1 m_xi = 0.05 m_t = 0 m_MuF2 = 2(Gev2) m_MuR2 = 2(Gev2)
 [PartonDistributionList]
 GPD_H
 [PartonDistribution]
 m_className = PartonDistribution - m_objectId = 241 indexId = -1
 GluonDistribution = 0
 u = 0
 u(+) = -4.80937
 u(-) = 4.80937
 d = 0
 d(+) = -3.34827
 d(-) = 3.34827
 s = 0
 s(+) = 0
 s(-) = 0
 \endcode
 */
class GPDKinematic: public Kinematic {

public:

    //TODO The alias of T is defined in another class (Observable)
    /**
     * Parameter name to set variable \f$x\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_X;

    /**
     * Parameter name to set variable \f$\xi\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_XI;

    /**
     * Parameter name to set variable \f$\mu_{F}^{2}\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_MUF2;

    /**
     * Parameter name to set variable \f$\mu_{R}^{2}\f$ via configuration methods.
     */
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_MUR2;

    /**
     * Default constructor.
     */
    GPDKinematic();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDKinematic(const GPDKinematic &other);

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, ObservableKinematic::PARAMETER_NAME_T, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2.
     */
    GPDKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDKinematic(double x, double xi, double t, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     *
     * @param x Longitudinal momentum fraction of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDKinematic(const ElemUtils::GenericType &x,
            const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Destructor.
     */
    virtual ~GPDKinematic();

    virtual std::string toString() const;

    /**
     * Serialize into given Packet.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Retrieve data from given Packet.
     * @param packet Input Packet.
     */
    void unserialize(ElemUtils::Packet &packet);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get kinematics type.
     */
    KinematicType::Type getKinematicType() const;

    /**
     * Set kinematics type.
     */
    void setKinematicType(KinematicType::Type kinematicType);

    /**
     * Get longitudinal momentum fraction of active parton.
     */
    double getX() const;

    /**
     * Set longitudinal momentum fraction of active parton.
     */
    void setX(double x);

    /**
     * Get skewness variable.
     */
    double getXi() const;

    /**
     * Set skewness variable.
     */
    void setXi(double xi);

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    double getT() const;

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t);

    /**
     * Get factorization scale squared.
     */
    double getMuF2() const;

    /**
     * Set factorization scale squared.
     */
    void setMuF2(double muF2);

    /**
     * Get renormalization scale squared.
     */
    double getMuR2() const;

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(double muR2);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Kinematics type.
     */
    KinematicType::Type m_kinematicType;

    /**
     * Longitudinal momentum fraction of active parton.
     */
    double m_x;

    /**
     * Skewness variable.
     */
    double m_xi;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Factorization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuF2;

    /**
     * Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuR2;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GPDKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GPDKinematic& kinematic);

} /* namespace PARTONS */

#endif /* GPD_KINEMATIC_H */
