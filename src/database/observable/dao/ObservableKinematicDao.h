#ifndef OBSERVABLE_KINEMATIC_DAO_H
#define OBSERVABLE_KINEMATIC_DAO_H

/**
 * @file ObservableKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ObservableKinematicDao
 *
 * @brief
 */

#include <string>

#include "../../../BaseObject.h"

class ObservableKinematicList;

class ObservableKinematicDao: public BaseObject {
public:
    ObservableKinematicDao();
    virtual ~ObservableKinematicDao();

    int insert(double xB, double t, double Q2,
            const std::string &listOfPhi) const;

    int select(double xB, double t, double Q2,
            const std::string &listOfPhi_str) const;

    ObservableKinematicList getKinematicListByComputationId(
            int computationId) const;
};

#endif /* OBSERVABLE_KINEMATIC_DAO_H */
