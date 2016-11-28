#ifndef MODULE_OBJECT_FACTORY_H
#define MODULE_OBJECT_FACTORY_H

/**
 * @file ModuleObjectFactory.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 02, 2015
 * @version 1.0
 */

#include <map>
#include <string>

#include "ModuleObjectReference.h"

class ActiveFlavorsModule;
class BaseObjectFactory;
class DoubleDistributionModule;
class DVCSConvolCoeffFunctionModule;
class DVCSModule;
class GapEquationSolverModule;
class GPDBorderFunctionModule;
class GPDEvolutionModule;
class GPDModule;
class GPDSubtractionConstantModule;
class IncompleteGPDModule;
class ModuleObject;
class Observable;
class ProcessModule;
class RadonInverseModule;
class RunningAlphaStrongModule;
class ScaleModule;
class XiConverterModule;

/**
 * @class ModuleObjectFactory
 *
 * @brief It's a specialization of the BaseObjectFactory to get intermediate abstract module class specialization to be able to use directly more sophisticated method (like compute(...)).
 * It only casts BaseObject pointer to desired abstract module class type.
 */
class ModuleObjectFactory: public BaseObject {
public:
    /**
     * Default destructor
     */
    virtual ~ModuleObjectFactory();

    ModuleObject* newModuleObject(const std::string& className);
    ModuleObject* newModuleObject(unsigned int classId);

    IncompleteGPDModule* newIncompleteGPDModule(unsigned int classId);
    IncompleteGPDModule* newIncompleteGPDModule(const std::string & className);

    RadonInverseModule* newRadonMatrixModule(unsigned int classId);
    RadonInverseModule* newRadonMatrixModule(const std::string & className);

    DoubleDistributionModule* newDoubleDistributionModule(unsigned int classId);
    DoubleDistributionModule* newDoubleDistributionModule(
            const std::string & className);

    GPDEvolutionModule* newGPDEvolutionModule(unsigned int classId);
    GPDEvolutionModule* newGPDEvolutionModule(const std::string & className);

    GPDModule* newGPDModule(unsigned int classId);
    GPDModule* newGPDModule(const std::string &className);

    GPDBorderFunctionModule* newGPDBorderFunctionModule(unsigned int classId);
    GPDBorderFunctionModule* newGPDBorderFunctionModule(
            const std::string &className);

    GPDSubtractionConstantModule* newGPDSubtractionConstantModule(
            unsigned int classId);
    GPDSubtractionConstantModule* newGPDSubtractionConstantModule(
            const std::string &className);

    DVCSConvolCoeffFunctionModule* newDVCSConvolCoeffFunctionModule(
            unsigned int classId);
    DVCSConvolCoeffFunctionModule* newDVCSConvolCoeffFunctionModule(
            const std::string &className);

    DVCSModule* newProcessModule(unsigned int classId);
    DVCSModule* newProcessModule(const std::string & className);

    RunningAlphaStrongModule* newRunningAlphaStrongModule(unsigned int classId);
    RunningAlphaStrongModule* newRunningAlphaStrongModule(
            const std::string & className);

    ProcessModule* newObservableModule(unsigned int classId);
    ProcessModule* newObservableModule(const std::string &className);

    ActiveFlavorsModule* newActiveFlavorsModule(unsigned int classId);
    ActiveFlavorsModule* newActiveFlavorsModule(const std::string &className);

    ScaleModule* newScaleModule(unsigned int classId);
    ScaleModule* newScaleModule(const std::string &className);

    XiConverterModule* newXiConverterModule(unsigned int classId);
    XiConverterModule* newXiConverterModule(const std::string &className);

    GapEquationSolverModule* newGapEquationSolverModule(unsigned int classId);
    GapEquationSolverModule* newGapEquationSolverModule(
            const std::string &className);

    Observable* newObservable(unsigned int classId);
    Observable* newObservable(const std::string & className);

    void updateModulePointerReference(ModuleObject* pModuleObjectTarget,
            ModuleObject* pModuleObjectSource);

    virtual std::string toString() const;

private:
    // To allow only Partons class to create a new instance of this class.
    // Used to avoid multiple singleton class and to avoid multithreading problem especially when getInstance() is called.
    // There is a bad behaviour with first instance initialization and mutex.
    friend class Partons;

    /**
     * Private default constructor to ensure the creation of a single instance of the class, managed by Parton's class.
     *
     * @param pBaseObjectFactory
     */
    ModuleObjectFactory(BaseObjectFactory* pBaseObjectFactory);

    BaseObjectFactory* m_pBaseObjectFactory; ///< Pointer to BaseObjectFactory to get cloned object's pointer.

    /**
     * std::map<BaseObject::m_objectId, ModuleObjectReference>
     * Store ModuleObject pointer created by the ModuleObjectFactory; used to handle references and life cycle of module pointer during execution of the program.
     */
    std::map<unsigned int, ModuleObjectReference*> m_instantiatedModuleObject;

    void store(ModuleObject* pModuleObject);

    //void remove(ModuleObject* pModuleObject);
};

#endif /* MODULE_OBJECT_FACTORY_H */
