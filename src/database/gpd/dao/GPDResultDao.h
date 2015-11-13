#ifndef GPD_RESULT_DAO
#define GPD_RESULT_DAO

/**
 * @file GPDResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 13 November 2015
 * @version 1.0
 *
 * @class GPDResultDao
 *
 * @brief
 */

#include <string>

#include "../../../BaseObject.h"

class GPDResultDao: public BaseObject {
public:
    GPDResultDao();
    virtual ~GPDResultDao();

    int insert(const std::string &computationModuleName, int gpdKinematicId,
            int computationId) const;
};

#endif /* GPD_RESULT_DAO */
