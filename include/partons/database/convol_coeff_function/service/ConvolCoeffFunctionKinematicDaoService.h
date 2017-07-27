#ifndef CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file ConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../dao/ConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionKinematicDaoService
 *
 * @brief Compton form factor (CFF) kinematics Data Access Object (DAO) service.
 *
 * It deals with ConvolCoeffFunctionKinematic C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove GPD kinematics from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 *
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //define GPD kinematics to be inserted in database
 DVCSConvolCoeffFunctionKinematic dvcsConvolCoeffFunctionKinematicInserted(0.1, -0.1, 2., 6., 10.);

 //get ConvolCoeffFunctionKinematicDaoService
 ConvolCoeffFunctionKinematicDaoService convolCoeffFunctionKinematicDaoService;

 //insert kinematics into database
 int id = convolCoeffFunctionKinematicDaoService.insert(dvcsConvolCoeffFunctionKinematicInserted);

 //retrieve kinematics from database
 DVCSConvolCoeffFunctionKinematic dvcsConvolCoeffFunctionKinematicExtracted;
 convolCoeffFunctionKinematicDaoService.select(id, dvcsConvolCoeffFunctionKinematicExtracted);

 //compare
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Inserted: " << dvcsConvolCoeffFunctionKinematicInserted.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Extracted: " << dvcsConvolCoeffFunctionKinematicExtracted.toString());
 \endcode
 */
class ConvolCoeffFunctionKinematicDaoService: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionKinematicDaoService();

    /**
     * Insert into database a new DVCSConvolCoeffFunctionKinematic object with transactions mechanisms.
     *
     * @param ccfKinematic Reference to DVCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Insert into database a list of DVCSConvolCoeffFunctionKinematic objects with transactions mechanisms.
     *
     * @param ccfKinematicList Reference to List of DVCSConvolCoeffFunctionKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(
            const List<DVCSConvolCoeffFunctionKinematic> &ccfKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given DVCSConvolCoeffFunctionKinematic kinematics.
     *
     * @param ccfKinematic Reference to DVCSConvolCoeffFunctionKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with DVCSConvolCoeffFunctionKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Try to find an entry in the database containing CFF kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return DVCSConvolCoeffFunctionKinematic object filled with the retrieved data or a default GPDKinematic object if couldn't find it.
     */
    DVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Try to find all entries in the database containing CFF kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return  List of DVCSConvolCoeffFunctionKinematic objects that matches with computation id.
     */
    List<DVCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Try to find unique id of database entry containing CFF kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow ConvolCoeffFunctionResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class ConvolCoeffFunctionResultDaoService;

    /**
     * ConvolCoeffFunctionKinematicDao object to perform database queries.
     */
    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao;

    /**
     * Insert into the database a new DVCSConvolCoeffFunctionKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<DVCSConvolCoeffFunctionKinematic> object, because transactions are already performed earlier.
     *
     * @param ccfKinematic Reference to DVCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(
            const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
