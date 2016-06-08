#ifndef GPD_KINEMATIC_H
#define GPD_KINEMATIC_H

/**
 * @file GPDKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class GPDKinematic
 *
 * @brief Class representing the kinematic variables for a GPD model.
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <string>

#include "../kinematic/KinematicType.h"
#include "../Kinematic.h"

namespace ElemUtils {
class Packet;
class Parameters;
} /* namespace ElemUtils */

class GPDKinematic: public Kinematic {
public:
    //TODO why missing T ?
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_X;
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_XI;
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_MUF2;
    static const std::string GPD_KINEMATIC_PARAMETER_NAME_MUR2;

    /**
     * Default constructor
     */
    GPDKinematic();

    /**
     * Constructor
     *
     * @param x Longitudinal momentum fraction of the active parton
     * @param xi Skewness
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param MuF Factorization scale (in GeV)
     * @param MuR Renormalization scale (in GeV)
     */
    GPDKinematic(double x, double xi, double t, double MuF2, double MuR2);

//    GPDKinematic(const std::string &x, const std::string &xi,
//            const std::string &t, const std::string &MuF2,
//            const std::string &MuR2);

    GPDKinematic(const ElemUtils::GenericType &x,
            const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    GPDKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Default destructor
     */
    virtual ~GPDKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

//    ComparisonReport compare(const GPDKinematic &referenceObject,
//            const NumA::Tolerances &tolerances) const;

    void serialize(ElemUtils::Packet &packet) const;
    void unserialize(ElemUtils::Packet &packet);

    // ##### GETTERS & SETTERS #####

    double getMuF2() const;
    void setMuF2(double muF2);
    double getMuR2() const;
    void setMuR2(double muR2);
    double getT() const;
    void setT(double t);
    double getX() const;
    void setX(double x);
    double getXi() const;
    void setXi(double xi);
    KinematicType::Type getKinematicType() const;
    void setKinematicType(KinematicType::Type kinematicType);

//    friend sf::Packet& operator <<(sf::Packet& packet,
//            const GPDKinematic& object);
//    friend sf::Packet& operator >>(sf::Packet& packet, GPDKinematic& object);

protected:
    virtual void updateHashSum() const;

private:
    KinematicType::Type m_kinematicType;

    double m_x;		///< Longitudinal momentum fraction of the active parton
    double m_xi;	///< Skewness
    double m_t;	///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_MuF2;	///< Factorization scale (in GeV^2)
    double m_MuR2;	///< Renormalization scale (in GeV^2)
};

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GPDKinematic& kinematic);
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GPDKinematic& kinematic);

#endif /* GPD_KINEMATIC_H */
