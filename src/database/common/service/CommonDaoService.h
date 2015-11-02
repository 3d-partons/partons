#ifndef COMMON_DAO_SERVICE_H
#define COMMON_DAO_SERVICE_H

/**
 * @file CommonDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class CommonDaoService
 *
 * @brief
 */

#include "../../../BaseObject.h"

class CommonDaoService: public BaseObject {
public:
    CommonDaoService();
    virtual ~CommonDaoService();

    int insertComputation() const;
};

#endif /* COMMON_DAO_SERVICE_H */
