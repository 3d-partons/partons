#ifndef TCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_H
#define TCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_H

/**
 * @file TCSConvolCoeffFunctionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>

#include "../../../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "ConvolCoeffFunctionResultDao.h"
#include "TCSConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class TCSConvolCoeffFunctionResultDao
 *
 * @brief TCS Compton form factor (CFF) result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete CFF results from the database.
 */
class TCSConvolCoeffFunctionResultDao: public ConvolCoeffFunctionResultDao {

public:

    /**
     * Default constructor.
     */
    TCSConvolCoeffFunctionResultDao();

    /**
     * Destructor.
     */
    virtual ~TCSConvolCoeffFunctionResultDao();

    /**
     * Insert and associate row in the database representing complex number (result for given GPD type) to that representing TCSConvolCoeffFunctionResult.
     * @param realPart Real part of CFF value.
     * @param imgPart Imaginary part of CFF value.
     * @param gpdType GPD type, see GPDType::Type.
     * @param ccfResultId Unique id of row in the database representing TCSConvolCoeffFunctionResult object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoTCSCCFResultComplex(const double realPart,
            const double imgPart, const GPDType::Type gpdType,
            const int ccfResultId) const;

    /**
     * Retrieve list of CFF results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of TCSConvolCoeffFunctionResult objects containing CFF results retrieved from the database.
     */
    List<TCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    /**
     * Fill List of TCSConvolCoeffFunctionResult objects from given query.
     * @param resultList List of TCSConvolCoeffFunctionResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillConvolCoeffFunctionResultList(
            List<TCSConvolCoeffFunctionResult> &resultList,
            QSqlQuery& query) const;

    /**
     * Kinematic DAO.
     */
    TCSConvolCoeffFunctionKinematicDao m_tcsConvolCoeffFunctionKinematicDao;
};

} /* namespace PARTONS */

#endif /* TCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
