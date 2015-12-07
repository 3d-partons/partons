#ifndef COMMON_DAO_H
#define COMMON_DAO_H

/**
 * @file CommonDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class CommonDao
 *
 * @brief
 */

#include <ctime>

#include "../../../BaseObject.h"

class CommonDao: public BaseObject {
public:
    CommonDao();
    virtual ~CommonDao();

    int insertComputation(const time_t &dateTime) const;
    int getComputationIdByDateTime(const time_t &dateTime) const;
};

#endif /* COMMON_DAO_H */
