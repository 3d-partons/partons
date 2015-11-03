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

class CommonDao {
public:
    static int insertComputation(const time_t &dateTime);
    static int getComputationIdByDateTime(const time_t &dateTime);
};

#endif /* COMMON_DAO_H */
