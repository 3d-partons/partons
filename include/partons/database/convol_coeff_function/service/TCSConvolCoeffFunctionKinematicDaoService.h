#ifndef TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file TCSConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../dao/TCSConvolCoeffFunctionKinematicDao.h"
#include "ConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class TCSConvolCoeffFunctionKinematicDaoService
 *
 * @brief TCS Compton form factor (CFF) kinematics Data Access Object (DAO) service.
 *
 * It deals with ConvolCoeffFunctionKinematic C++ object and related tables from the database.
 */
class TCSConvolCoeffFunctionKinematicDaoService: public ConvolCoeffFunctionKinematicDaoService {

public:

    /**
     * Default constructor.
     */
    TCSConvolCoeffFunctionKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~TCSConvolCoeffFunctionKinematicDaoService();

    /**
     * Insert into database a new TCSConvolCoeffFunctionKinematic object with transactions mechanisms.
     *
     * @param ccfKinematic Reference to TCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const TCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Insert into database a list of TCSConvolCoeffFunctionKinematic objects with transactions mechanisms.
     *
     * @param ccfKinematicList Reference to List of TCSConvolCoeffFunctionKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(
            const List<TCSConvolCoeffFunctionKinematic> &ccfKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given TCSConvolCoeffFunctionKinematic kinematics.
     *
     * @param ccfKinematic Reference to TCSConvolCoeffFunctionKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with TCSConvolCoeffFunctionKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const TCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Try to find an entry in the database containing CFF kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return TCSConvolCoeffFunctionKinematic object filled with the retrieved data or a default GPDKinematic object if couldn't find it.
     */
    TCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Try to find all entries in the database containing CFF kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return  List of TCSConvolCoeffFunctionKinematic objects that matches with computation id.
     */
    List<TCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Try to find unique id of database entry containing CFF kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    virtual int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow TCSConvolCoeffFunctionResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class TCSConvolCoeffFunctionResultDaoService;

    /**
     *TCS ConvolCoeffFunctionKinematicDao object to perform database queries.
     */
    TCSConvolCoeffFunctionKinematicDao m_tcsConvolCoeffFunctionKinematicDao;

    /**
     * Insert into the database a new TCSConvolCoeffFunctionKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<TCSConvolCoeffFunctionKinematic> object, because transactions are already performed earlier.
     *
     * @param ccfKinematic Reference to TCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(
            const TCSConvolCoeffFunctionKinematic &ccfKinematic) const;
};

} /* namespace PARTONS */

#endif /* TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
