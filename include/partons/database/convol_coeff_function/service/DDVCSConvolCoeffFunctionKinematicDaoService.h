#ifndef DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file DDVCSConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../dao/DDVCSConvolCoeffFunctionKinematicDao.h"
#include "ConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionKinematicDaoService
 *
 * @brief DDVCS Compton form factor (CFF) kinematics Data Access Object (DAO) service.
 *
 * It deals with ConvolCoeffFunctionKinematic C++ object and related tables from the database.
 */
class DDVCSConvolCoeffFunctionKinematicDaoService: public ConvolCoeffFunctionKinematicDaoService {

public:

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionKinematicDaoService();

    /**
     * Insert into database a new DDVCSConvolCoeffFunctionKinematic object with transactions mechanisms.
     *
     * @param ccfKinematic Reference to DDVCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DDVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Insert into database a list of DDVCSConvolCoeffFunctionKinematic objects with transactions mechanisms.
     *
     * @param ccfKinematicList Reference to List of DDVCSConvolCoeffFunctionKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(
            const List<DDVCSConvolCoeffFunctionKinematic> &ccfKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given DDVCSConvolCoeffFunctionKinematic kinematics.
     *
     * @param ccfKinematic Reference to DDVCSConvolCoeffFunctionKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with DDVCSConvolCoeffFunctionKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const DDVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Try to find an entry in the database containing CFF kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return DDVCSConvolCoeffFunctionKinematic object filled with the retrieved data or a default GPDKinematic object if couldn't find it.
     */
    DDVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Try to find all entries in the database containing CFF kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return  List of DDVCSConvolCoeffFunctionKinematic objects that matches with computation id.
     */
    List<DDVCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Try to find unique id of database entry containing CFF kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    virtual int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow DDVCSConvolCoeffFunctionResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class DDVCSConvolCoeffFunctionResultDaoService;

    /**
     *DDVCS ConvolCoeffFunctionKinematicDao object to perform database queries.
     */
    DDVCSConvolCoeffFunctionKinematicDao m_ddvcsConvolCoeffFunctionKinematicDao;

    /**
     * Insert into the database a new DDVCSConvolCoeffFunctionKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<DDVCSConvolCoeffFunctionKinematic> object, because transactions are already performed earlier.
     *
     * @param ccfKinematic Reference to DDVCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(
            const DDVCSConvolCoeffFunctionKinematic &ccfKinematic) const;
};

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
