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

#include <string>

class CommonDao {
public:
    static int insertComputation(const std::string &computationDate);
};

#endif /* COMMON_DAO_H */
