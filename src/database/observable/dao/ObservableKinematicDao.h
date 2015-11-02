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

class ObservableKinematicDao {
public:
    static int insert(double xB, double t, double Q2,
            const std::string &listOfPhi);

    static int select(double xB, double t, double Q2);
};

#endif /* OBSERVABLE_KINEMATIC_DAO_H */
