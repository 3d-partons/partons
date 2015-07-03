/*
 * BaseObjectFactory.h
 *
 *  Created on: Jun 26, 2015
 *      Author: debian
 */

#ifndef BASEOBJECTFACTORY_H_
#define BASEOBJECTFACTORY_H_

#include <string>

class DVCSModule;
class EvolQCDModule;
class MathIntegratorModule;
class RunningAlphaStrongModule;
class BaseObjectRegistry;
class BaseObject;
class GPDModule;
class CoefficientFunctionModule;
class ObservableModule;

/*
 *
 */
class BaseObjectFactory {
public:
    /**
     * Share a unique pointer of this class
     */
    static BaseObjectFactory* getInstance();

    /**
     * Default destructor
     */
    virtual ~BaseObjectFactory();

    BaseObject* newBaseObject(unsigned int classId);
    BaseObject* newBaseObject(const std::string &className);

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static BaseObjectFactory* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    BaseObjectFactory();

    BaseObjectRegistry* m_pBaseObjectregistry;
};

#endif /* BASEOBJECTFACTORY_H_ */
