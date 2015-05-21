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

#include <string>

#include "KinematicType.h"

class GPDKinematic {
public:
    static const std::string GPD_KINEMATIC_DB_COLUMN_NAME_X;
    static const std::string GPD_KINEMATIC_DB_COLUMN_NAME_XI;
    static const std::string GPD_KINEMATIC_DB_COLUMN_NAME_T;
    static const std::string GPD_KINEMATIC_DB_COLUMN_NAME_MUF;
    static const std::string GPD_KINEMATIC_DB_COLUMN_NAME_MUR;

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
    GPDKinematic(double x, double xi, double t, double MuF, double MuR);

    /**
     * Default destructor
     */
    virtual ~GPDKinematic();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    double getMuF() const;
    void setMuF(double muF);
    double getMuR() const;
    void setMuR(double muR);
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

private:
    KinematicType::Type m_kinematicType;

    double m_x;		///< Longitudinal momentum fraction of the active parton
    double m_xi;	///< Skewness
    double m_t;	///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
    double m_MuF;	///< Factorization scale (in GeV)
    double m_MuR;	///< Renormalization scale (in GeV)
};

//sf::Packet& operator <<(sf::Packet& packet, const GPDKinematic& object);
//sf::Packet& operator >>(sf::Packet& packet, GPDKinematic& object);

#endif /* GPD_KINEMATIC_H */
