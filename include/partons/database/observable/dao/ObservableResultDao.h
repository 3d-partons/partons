#ifndef OBSERVABLE_RESULT_DAO_H
#define OBSERVABLE_RESULT_DAO_H

/**
 * @file ObservableResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../BaseObject.h"

namespace PARTONS {

/**
 * @class ObservableResultDao
 *
 * @brief Observable result Data Access Object (DAO).
 */
class ObservableResultDao: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ObservableResultDao(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ObservableResultDao();
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_RESULT_DAO_H */
