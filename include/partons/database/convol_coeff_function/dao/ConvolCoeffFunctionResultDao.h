#ifndef CONVOL_COEFF_FUNCTION_RESULT_DAO_H
#define CONVOL_COEFF_FUNCTION_RESULT_DAO_H

/**
 * @file ConvolCoeffFunctionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/channel/ChannelType.h"
#include "../../../BaseObject.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionResultDao
 *
 * @brief Compton form factor (CFF) result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete CFF results from the database.
 */
class ConvolCoeffFunctionResultDao: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionResultDao(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionResultDao();

    /**
     * Insert CFF result into database.
     * @param computationModuleName Name of module used to evaluate this result.
     * @param channelId Channel type.
     * @param kinematicId Kinematics id.
     * @param computationId Unique id of row in the database containing computation information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoCCFResult(const std::string &computationModuleName,
            const ChannelType::Type channelType, const int kinematicId,
            const int computationId) const;
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
