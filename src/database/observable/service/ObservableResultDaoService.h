#ifndef OBSERVABLE_RESULT_DAO_SERVICE_H
#define OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file ObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ObservableResultDaoService
 *
 * @brief
 */

#include "../../common/service/CommonDaoService.h"
#include "../dao/ObservableResultDao.h"
#include "ObservableKinematicDaoService.h"

class ObservableResultList;
class ObservableResult;

class ObservableResultDaoService: public BaseObject {
public:
    ObservableResultDaoService();
    virtual ~ObservableResultDaoService();

    int insert(const ObservableResult &observableResult) const;
    int insert(const ObservableResultList &observableResultList);

private:
    ObservableResultDao m_observableResultDao;

    ObservableKinematicDaoService m_observableKinematicDaoService;
    CommonDaoService m_commonDaoService;
};

#endif /* OBSERVABLE_RESULT_DAO_SERVICE_H */
