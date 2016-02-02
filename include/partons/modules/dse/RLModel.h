/**
 * @file RLModel.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 26, 2016
 * @version 1.0
 *
 * @class RLModel
 */

#ifndef RLMODEL_H_
#define RLMODEL_H_

#include <string>

#include "GapEquationSolverModule.h"

class RLModel: public GapEquationSolverModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    RLModel(const std::string &className);
    virtual ~RLModel();

    virtual RLModel* clone() const;

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    RLModel(const RLModel& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* RLMODEL_H_ */
