#ifndef OBSERVABLE_KINEMATIC_H
#define OBSERVABLE_KINEMATIC_H

/**
 * @file ObservableKinematic.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class ObservableKinematic
 *
 * @brief Class representing the kinematic variables.
 */

#include <vector>

#include "FormFactorKinematic.h"

class ObservableKinematic: public FormFactorKinematic {
public:
    ObservableKinematic(double xB, double t, double Q2,
            std::vector<double> listOfPhi);
    ~ObservableKinematic();

    // ##### GETTERS & SETTERS #####

    const std::vector<double>& getListOfPhi() const;
    void setListOfPhi(const std::vector<double>& listOfPhi);

private:
    std::vector<double> m_listOfPhi; ///<  Angles between leptonic and hadronic planes
};

#endif /* OBSERVABLE_KINEMATIC_H */
