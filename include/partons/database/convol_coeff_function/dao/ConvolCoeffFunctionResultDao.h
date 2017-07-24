#ifndef CONVOL_COEFF_FUNCTION_RESULT_DAO_H
#define CONVOL_COEFF_FUNCTION_RESULT_DAO_H

/**
 * @file ConvolCoeffFunctionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/List.h"
#include "ConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionResultDao
 *
 * @brief Compton form factor (CFF) result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete CFF results from the database. This class in not intended to be used by regular users who should deal with ConvolCoeffFunctionResultDaoService instead.
 */
class ConvolCoeffFunctionResultDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionResultDao();

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionResultDao();

    //TODO remove ; unused
    /**
     * Insert CFF result into database.
     * @param computationModuleName Name of module used to evaluate this result.
     * @param kinematicId Unique id of row in the database containing CFF kinematics associated to this result.
     * @param computationId Unique id of row in the database containing computation information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insert(const std::string &computationModuleName, const int kinematicId,
            const int computationId) const;

    /**
     * Insert and associate row in the database representing complex number (result for given GPD type) to that representing DVCSConvolCoeffFunctionResult.
     * @param realPart Real part of CFF value.
     * @param imgPart Imaginary part of CFF value.
     * @param gpdTypeId GPD type, see GPDType::Type.
     * @param ccfResultId Unique id of row in the database representing DVCSConvolCoeffFunctionResult object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoCCFResultComplex(const int realPart, const int imgPart,
            const int gpdTypeId, const int ccfResultId) const;

    /**
     * Retrieve list of CFF results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of DVCSConvolCoeffFunctionResult objects containing CFF results retrieved from the database.
     */
    List<DVCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    /**
     * ConvolCoeffFunctionKinematicDao object to perform database queries.
     */
    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao;

    /**
     * Fill List of DVCSConvolCoeffFunctionResult objects from given query.
     * @param resultList List of DVCSConvolCoeffFunctionResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillConvolCoeffFunctionResultList(
            List<DVCSConvolCoeffFunctionResult> &resultList,
            QSqlQuery& query) const;
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
