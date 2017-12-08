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

namespace PARTONS {

class ActiveFlavorsThresholdsModule;
class BaseObjectFactory;
class DVCSConvolCoeffFunctionModule;
class DVCSProcessModule;
class GPDBorderFunctionModule;
class GPDEvolutionModule;
class GPDModule;
class GPDSubtractionConstantModule;
class ModuleObject;
class Observable;
class ProcessModule;
class RunningAlphaStrongModule;
class ScalesModule;
class XiConverterModule;

/**
 * @class ModuleObjectFactory
 *
 * @brief It's a specialization of the BaseObjectFactory to get intermediate abstract module class specialization to be able to use directly more sophisticated methods (like compute(...)).
 * It only casts BaseObject pointer to desired abstract module class type.
 */
class ModuleObjectFactory: public BaseObject {
public:
    /**
     * Default destructor.
     */
    virtual ~ModuleObjectFactory();

    /**
     * Specialization of BaseObjectFactory::newBaseObject into a ModuleObject.
     * @param className Name of last child class.
     * @return ModuleObject pointer.
     */
    ModuleObject* newModuleObject(const std::string& className);
    /**
     * Specialization of BaseObjectFactory::newBaseObject into a ModuleObject.
     * @param classId Unique identifier of last child class.
     * @return ModuleObject pointer.
     */
    ModuleObject* newModuleObject(unsigned int classId);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDEvolutionModule.
     * @param classId Unique identifier of last child class.
     * @return GPDEvolutionModule pointer.
     */
    GPDEvolutionModule* newGPDEvolutionModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDEvolutionModule.
     * @param className Name of last child class.
     * @return GPDEvolutionModule pointer.
     */
    GPDEvolutionModule* newGPDEvolutionModule(const std::string & className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDModule.
     * @param classId Unique identifier of last child class.
     * @return GPDModule pointer.
     */
    GPDModule* newGPDModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDModule.
     * @param className Name of last child class.
     * @return GPDModule pointer.
     */
    GPDModule* newGPDModule(const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDBorderFunctionModule.
     * @param classId Unique identifier of last child class.
     * @return GPDBorderFunctionModule pointer.
     */
    GPDBorderFunctionModule* newGPDBorderFunctionModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDBorderFunctionModule.
     * @param className Name of last child class.
     * @return GPDBorderFunctionModule pointer.
     */
    GPDBorderFunctionModule* newGPDBorderFunctionModule(
            const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDSubtractionConstantModule.
     * @param classId Unique identifier of last child class.
     * @return GPDSubtractionConstantModule pointer.
     */
    GPDSubtractionConstantModule* newGPDSubtractionConstantModule(
            unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a GPDSubtractionConstantModule.
     * @param className Name of last child class.
     * @return GPDSubtractionConstantModule pointer.
     */
    GPDSubtractionConstantModule* newGPDSubtractionConstantModule(
            const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a DVCSConvolCoeffFunctionModule.
     * @param classId Unique identifier of last child class.
     * @return DVCSConvolCoeffFunctionModule pointer.
     */
    DVCSConvolCoeffFunctionModule* newDVCSConvolCoeffFunctionModule(
            unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a DVCSConvolCoeffFunctionModule.
     * @param className Name of last child class.
     * @return DVCSConvolCoeffFunctionModule pointer.
     */
    DVCSConvolCoeffFunctionModule* newDVCSConvolCoeffFunctionModule(
            const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a DVCSProcessModule.
     * @param classId Unique identifier of last child class.
     * @return DVCSProcessModule pointer.
     */
    DVCSProcessModule* newDVCSProcessModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a DVCSProcessModule.
     * @param className Name of last child class.
     * @return DVCSProcessModule pointer.
     */
    DVCSProcessModule* newDVCSProcessModule(const std::string & className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a RunningAlphaStrongModule.
     * @param classId Unique identifier of last child class.
     * @return RunningAlphaStrongModule pointer.
     */
    RunningAlphaStrongModule* newRunningAlphaStrongModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a RunningAlphaStrongModule.
     * @param className Name of last child class.
     * @return RunningAlphaStrongModule pointer.
     */
    RunningAlphaStrongModule* newRunningAlphaStrongModule(
            const std::string & className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a ProcessModule.
     * @param classId Unique identifier of last child class.
     * @return ProcessModule pointer.
     */
    ProcessModule* newProcessModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a ProcessModule.
     * @param className Name of last child class.
     * @return ProcessModule pointer.
     */
    ProcessModule* newProcessModule(const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a ActiveFlavorsThresholdsModule.
     * @param classId Unique identifier of last child class.
     * @return ActiveFlavorsThresholdsModule pointer.
     */
    ActiveFlavorsThresholdsModule* newActiveFlavorsThresholdsModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a ActiveFlavorsModule.
     * @param className Name of last child class.
     * @return ActiveFlavorsModule pointer.
     */
    ActiveFlavorsThresholdsModule* newActiveFlavorsThresholdsModule(const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a ScalesModule.
     * @param classId Unique identifier of last child class.
     * @return ScalesModule pointer.
     */
    ScalesModule* newScalesModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a ScaleModule.
     * @param className Name of last child class.
     * @return ScaleModule pointer.
     */
    ScalesModule* newScalesModule(const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a XiConverterModule.
     * @param classId Unique identifier of last child class.
     * @return XiConverterModule pointer.
     */
    XiConverterModule* newXiConverterModule(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a XiConverterModule.
     * @param className Name of last child class.
     * @return XiConverterModule pointer.
     */
    XiConverterModule* newXiConverterModule(const std::string &className);

    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a Observable.
     * @param classId Unique identifier of last child class.
     * @return Observable pointer.
     */
    Observable* newObservable(unsigned int classId);
    /**
     * Specialization of ModuleObjectFactory::newModuleObject into a Observable.
     * @param className Name of last child class.
     * @return Observable pointer.
     */
    Observable* newObservable(const std::string & className);

    /**
     * Method to update a pointer. Used to keep track of the modules and remove them when they become orphans (i.e. no pointer points to them).
     * @param pModuleObjectTarget The pointer to be modified.
     * @param pModuleObjectSource The pointer to be copied into the other. Or 0 if the target pointer is to be cleared.
     */
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
     * Store ModuleObject pointer created by the ModuleObjectFactory; used to handle references and life cycle of module pointer during execution of the program.
     */
    std::map<unsigned int, ModuleObjectReference*> m_instantiatedModuleObject;

    /**
     * Store a new ModuleObjectReference from the provided ModuleObject pointer.
     *
     * @param pModuleObject
     */
    void store(ModuleObject* pModuleObject);
};

} /* namespace PARTONS */

#endif /* MODULE_OBJECT_FACTORY_H */
