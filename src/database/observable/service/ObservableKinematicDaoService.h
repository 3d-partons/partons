#ifndef OBSERVABLE_KINEMATIC_DAO_SERVICE_H
#define OBSERVABLE_KINEMATIC_DAO_SERVICE_H

/**
 * @file ObservableKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ObservableKinematicDaoService
 *
 * @brief
 */

#include "../../../beans/observable/ObservableKinematic.h"
#include "../dao/ObservableKinematicDao.h"

class ObservableKinematicList;

class ObservableKinematicDaoService: public BaseObject {
public:
    ObservableKinematicDaoService();
    virtual ~ObservableKinematicDaoService();

    int insert(const ObservableKinematic &observableKinematic) const;

    int getKinematicId(const ObservableKinematic &observableKinematic) const;

    ObservableKinematicList getKinematicListByComputationId(
            int computationId) const;

private:
    ObservableKinematicDao m_observableKinematicDao;
};

#endif /* OBSERVABLE_KINEMATIC_DAO_SERVICE_H */
