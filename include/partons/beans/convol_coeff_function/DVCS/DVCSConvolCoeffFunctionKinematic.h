#ifndef DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H
#define DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H

/**
 * @file DVCSConvolCoeffFunctionKinematic.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Kinematic.h"

namespace ElemUtils {
class Packet;
class Parameters;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionKinematic
 *
 * @brief Class representing single CFF kinematics for DVCS process.
 *
 * This class represents a single Compton Form Factor (CFF) kinematics for DVCS process (xi, t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$) to be used in all kinds of computations done within PARTONS DVCS/CFF layer.
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //evaluate exemplary CFF result

 //retrieve CFF service
 ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService = Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //load CFF module with the BaseModuleFactory
 DVCSConvolCoeffFunctionModule* pDVCSCFFModule = Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(DVCSCFFModel::classId);

 //configure CFF module
 ElemUtils::Parameters parameters(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PerturbativeQCDOrderType::LO);
 pDVCSCFFModule->configure(parameters);

 pDVCSCFFModule->setGPDModule(pGPDModel);

 //define CFF kinematics used in computation
 DVCSConvolCoeffFunctionKinematic cffKinematic(0.05, -0.1, 4., 4., 4.);

 //evaluate
 DVCSConvolCoeffFunctionResult cffResult = pDVCSConvolCoeffFunctionService->computeForOneCCFModel(cffKinematic, pDVCSCFFModule);

 //print kinematics and result
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Kinematics:\n" << cffKinematic.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Result:\n" << cffResult.toString());
 \endcode
 which gives via Logger:
 \code
 20-05-2017 04:28:28 [INFO] (example::main) Kinematics:
 [DVCSConvolCoeffFunctionKinematic]
 m_className = DVCSConvolCoeffFunctionKinematic - m_objectId = 204 indexId = -1 m_binId = 0 m_xi = 0.05 m_t = -0.1 m_Q2 = 4 m_MuF2 = 4 m_MuR2 = 4
 20-05-2017 04:28:28 [INFO] (example::main) Result:
 [DVCSConvolCoeffFunctionResult] computed by DVCSCFFModel
 kinematic([DVCSConvolCoeffFunctionKinematic]
 m_className = DVCSConvolCoeffFunctionKinematic - m_objectId = 204 indexId = -1 m_binId = 0 m_xi = 0.05 m_t = -0.1 m_Q2 = 4 m_MuF2 = 4 m_MuR2 = 4)
 CFF_H = -0.487734416992033 ; 2.07452159003312
 CFF_E = -9.01663758571869 ; 13.716547623976
 \endcode
 */
class DVCSConvolCoeffFunctionKinematic: public Kinematic {
public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionKinematic();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, ObservableKinematic::PARAMETER_NAME_T, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2.
     */
    DVCSConvolCoeffFunctionKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DVCSConvolCoeffFunctionKinematic(double xi, double t, double Q2,
            double MuF2, double MuR2);

    /**
     * Assignment constructor.
     * @param binId Id of bin.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DVCSConvolCoeffFunctionKinematic(unsigned int binId, double xi, double t,
            double Q2, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    DVCSConvolCoeffFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t, const ElemUtils::GenericType &Q2,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionKinematic();

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
     * Get id of bin.
     * @return Retrieved value.
     */
    unsigned int getBinId() const;

    /**
     * Set id of bin.
     */
    void setBinId(unsigned int binId);

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
     * Get virtual-photon virtuality.
     */
    double getQ2() const;

    /**
     * Set virtual-photon virtuality.
     */
    void setQ2(double q2);

    /**
     * Set factorization scale squared.
     */
    void setMuF2(double muF2);

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(double muR2);

    /**
     * Get factorization scale squared.
     */
    double getMuF2() const;

    /**
     * Get renormalization scale squared.
     */
    double getMuR2() const;

protected:
    virtual void updateHashSum() const;

private:

    /**
     * Id of bin.
     * This information is not set and used by typical calculations performed in PARTONS. However, it can associated to experimental or pseudo data stored in the database.
     */
    unsigned int m_binId;

    /**
     * Skewness variable.
     */
    double m_xi;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    double m_Q2;

    /**
     * Factorization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuF2;

    /**
     * Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuR2;
};

} /* namespace PARTONS */

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PARTONS::DVCSConvolCoeffFunctionKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        PARTONS::DVCSConvolCoeffFunctionKinematic& kinematic);

#endif /* DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_H */
