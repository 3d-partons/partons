#ifndef OBSERVABLE_KINEMATIC_H
#define OBSERVABLE_KINEMATIC_H

/**
 * @file ObservableKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 */

#include <string>
#include <ElementaryUtils/parameters/GenericType.h>

#include "../../utils/type/PhysicalType.h"
#include "../Kinematic.h"

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */

/**
 * @class ObservableKinematic
 *
 * @brief Class representing single observable kinematics for DVCS process.
 *
 * This class represents a single observable kinematics for DVCS process (x_{B}, t, \f$Q^{2}\f$, E_{b}, \f$\phi\f$) to be used in all kinds of computations done within PARTONS DVCS/observable layer.
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //evaluate exemplary observable result to be inserted in database

 //retrieve observable service
 ObservableService* pObservableService = Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //load CFF module with the BaseModuleFactory
 DVCSConvolCoeffFunctionModule* pDVCSCFFModule = Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(DVCSCFFModel::classId);

 //configure CFF module
 ElemUtils::Parameters parameters(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PerturbativeQCDOrderType::LO);
 pDVCSCFFModule->configure(parameters);

 pDVCSCFFModule->setGPDModule(pGPDModel);

 //load process module with BaseModuleFactory
 DVCSModule* pDVCSModule = Partons::getInstance()->getModuleObjectFactory()->newProcessModule(GV2008Model::classId);

 //configure process module
 pDVCSModule->setConvolCoeffFunctionModule(pDVCSCFFModule);

 //load observable module with BaseModuleFactory
 Observable* pObservable = Partons::getInstance()->getModuleObjectFactory()->newObservable(All::classId);

 //configure observable module
 pObservable->setProcessModule(pDVCSModule);

 //define observable kinematics used in computation
 ObservableKinematic observableKinematic(0.17, -0.13, 1.36, 5.77, 10.);

 //evaluate
 ObservableResult observableResult = pObservableService->computeObservable(observableKinematic, pObservable);

 //print kinematics and result
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Kinematics:\n" << observableKinematic.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Result:\n" << observableResult.toString());
 \endcode
 which gives via Logger:
 \code
 20-05-2017 05:12:42 [INFO] (example::main) Kinematics:
 [ObservableKinematic]
 m_className = ObservableKinematic - m_objectId = 212 indexId = -1 m_xB = 0.17 m_t = -0.13 (GeV2) m_Q2 = 1.36 (GeV2) m_E = 5.77 (GeV) phi = 10 (degree)
 20-05-2017 05:12:42 [INFO] (example::main) Result:
 [ObservableKinematic]
 m_className = ObservableKinematic - m_objectId = 212 indexId = -1 m_xB = 0.17 m_t = -0.13 (GeV2) m_Q2 = 1.36 (GeV2) m_E = 5.77 (GeV) phi = 10 (degree) observable = -0.341714191542444
 \endcode
 */
class ObservableKinematic: public Kinematic {

public:

    /**
     * Parameter name to set variable \f$x_{B}\f$ via configuration methods.
     */
    static const std::string PARAMETER_NAME_XB;

    /**
     * Parameter name to set variable \f$t\f$ via configuration methods.
     */
    static const std::string PARAMETER_NAME_T;

    /**
     * Parameter name to set variable \f$Q^{2}\f$ via configuration methods.
     */
    static const std::string PARAMETER_NAME_Q2;

    /**
     * Parameter name to set variable \f$E_{b}\f$ via configuration methods.
     */
    static const std::string PARAMETER_NAME_BEAM_ENERGY;

    /**
     * Parameter name to set variable \f$\phi\f$ via configuration methods.
     */
    static const std::string PARAMETER_NAME_PHI;

    /**
     * Default constructor.
     */
    ObservableKinematic();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by ObservableKinematic::PARAMETER_NAME_XB, ObservableKinematic::PARAMETER_NAME_T, ObservableKinematic::PARAMETER_NAME_Q2, ObservableKinematic::PARAMETER_NAME_BEAM_ENERGY and ObservableKinematic::PARAMETER_NAME_PHI.
     */
    ObservableKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     */
    ObservableKinematic(double xB, double t, double Q2, double E, double phi);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     */
    ObservableKinematic(double xB, double t, double Q2, double E);

    /**
     * Assignment constructor.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     */
    ObservableKinematic(const ElemUtils::GenericType& xB,
            const ElemUtils::GenericType& t, const ElemUtils::GenericType& Q2,
            const ElemUtils::GenericType& E, const ElemUtils::GenericType& phi);

    /**
     * Destructor.
     */
    virtual ~ObservableKinematic();

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
     * Get Bjorken variable.
     */
    double getXB() const;

    /**
     * Set Bjorken variable.
     */
    void setXB(double xB);

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
    void setQ2(double Q2);

    /**
     * Get beam energy.
     */
    double getE() const;

    /**
     * Set beam energy.
     */
    void setE(double E);

    /**
     * Get angle between leptonic and hadronic planes.
     */
    PhysicalType<double> getPhi() const;

    /**
     * Set angle between leptonic and hadronic planes.
     */
    void setPhi(double phi);

protected:

    virtual void updateHashSum() const;

private:

    //TODO : What do we do when we consider Fourier harmonics of DVCS observables?

    /**
     * Bjorken variable.
     */
    double m_xB;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Virtual-photon virtuality (in \f$GeV^{2}\f$).
     */
    double m_Q2;

    /**
     * Beam energy (in GeV).
     */
    double m_E;

    /**
     * Angle between leptonic and hadronic planes (in degrees, Trento convention).
     */
    PhysicalType<double> m_phi;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic);

#endif /* OBSERVABLE_KINEMATIC_H */
