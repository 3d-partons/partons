#ifndef OBSERVABLE_RESULT_DAO_H
#define OBSERVABLE_RESULT_DAO_H

/**
 * @file ObservableResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ObservableResultDao
 *
 * @brief
 */

#include <string>

#include "../../../BaseObject.h"

class ObservableResultDao: public BaseObject {
public:
    ObservableResultDao();
    virtual ~ObservableResultDao();

    int insert(const std::string &observableName, double observableValue,
            double phi, double statErrorLB, double statErrorUB,
            double systErrorLB, double systErrorUB, double errorTotal,
            int kinematicId, int computationId) const;
};

#endif /* OBSERVABLE_RESULT_DAO_H */
