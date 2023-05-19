#ifndef DDVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_H
#define DDVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_H

/**
 * @file DDVCSConvolCoeffFunctionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>

#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "ConvolCoeffFunctionResultDao.h"
#include "DDVCSConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionResultDao
 *
 * @brief DDVCS Compton form factor (CFF) result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete CFF results from the database.
 */
class DDVCSConvolCoeffFunctionResultDao: public ConvolCoeffFunctionResultDao {

public:

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionResultDao();

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionResultDao();

    /**
     * Insert and associate row in the database representing complex number (result for given GPD type) to that representing DDVCSConvolCoeffFunctionResult.
     * @param realPart Real part of CFF value.
     * @param imgPart Imaginary part of CFF value.
     * @param gpdType GPD type, see GPDType::Type.
     * @param ccfResultId Unique id of row in the database representing DDVCSConvolCoeffFunctionResult object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoDDVCSCCFResultComplex(const double realPart,
            const double imgPart, const GPDType::Type gpdType,
            const int ccfResultId) const;

    /**
     * Retrieve list of CFF results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of DDVCSConvolCoeffFunctionResult objects containing CFF results retrieved from the database.
     */
    List<DDVCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    /**
     * Fill List of DDVCSConvolCoeffFunctionResult objects from given query.
     * @param resultList List of DDVCSConvolCoeffFunctionResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillConvolCoeffFunctionResultList(
            List<DDVCSConvolCoeffFunctionResult> &resultList,
            QSqlQuery& query) const;

    /**
     * Kinematic DAO.
     */
    DDVCSConvolCoeffFunctionKinematicDao m_ddvcsConvolCoeffFunctionKinematicDao;
};

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
