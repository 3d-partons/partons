/*
 * GPDSubtractionConstantModule.h
 *
 *  Created on: Oct 18, 2016
 *      Author: debian
 */

#ifndef GPDSUBTRACTIONCONSTANTMODULE_H_
#define GPDSUBTRACTIONCONSTANTMODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../beans/automation/BaseObjectData.h"
#include "../beans/gpd/GPDBorderFunctionKinematic.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../ModuleObject.h"

/**
 * @class GPDSubtractionConstantModule
 *
 * @brief Abstract class for modeling of GPD subtraction constant.
 *
 * This class acts as an abstract (mother) class for modules used in the modeling of the GPD subtraction constants.
 * The usage of such kind of modules is illustrated by the following example:
 \code{.cpp}
 // Retrieve GPD service
 GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 // Load GPDBorderFunction module with the BaseModuleFactory
 GPDSubtractionConstantModule* pGPDSubtractionConstantModule = Partons::getInstance()->getModuleObjectFactory()->newGPDSubtractionConstantModule(KM10SubtractionConstantModel::classId);

 // Create border function kinematics
 GPDBorderFunctionKinematic borderFunctionKinematic(0.001, -0.3, 2., 2.);

 // Compute
 double subConst = pGPDSubtractionConstantModule->compute(borderFunctionKinematic, GPDType::H);

 // Print results
 Partons::getInstance()->getLoggerManager()->info("main", __func__, ElemUtils::Formatter() << "subtraction constant for kinematics" << borderFunctionKinematic.toString() << " is: " << subConst);
 \endcode
 which gives via Logger:
 \code
 06-06-2017 07:59:46 [INFO] (main::main) subtraction constant for kinematics[GPDKinematic]
 m_className = GPDKinematic - m_objectId = 195 indexId = -1
 m_x = 0.001 m_xi = 0.001 m_t = -0.3 m_MuF2 = 2(Gev2) m_MuR2 = 2(Gev2) is: 4.67128027681661
 \endcode
 */
class GPDSubtractionConstantModule: public ModuleObject {

public:

	/**
	 * Type of module name used by the automatization.
	 */
	static const std::string GPD_SUBTRACTION_CONSTANT_MODULE_CLASS_NAME;

	/**
	 * Constructor.
	 * @param className Name of class.
	 */
	GPDSubtractionConstantModule(const std::string &className);

	/**
	 * Destructor.
	 */
	virtual ~GPDSubtractionConstantModule();

	virtual GPDSubtractionConstantModule* clone() const;
	virtual void configure(const ElemUtils::Parameters &parameters);
	virtual std::string toString() const;
	virtual void resolveObjectDependencies();
	virtual void prepareSubModules(
			const std::map<std::string, BaseObjectData>& subModulesData);

	/**
	 * Evaluate specific GPD subtraction constant for a given kinematics.
	 * @param xi Skewness variable.
	 * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
	 * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
	 * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
	 * @param gpdType Type of GPD.
	 */
	virtual double compute(double xi, double t, double MuF2, double MuR2,
			GPDType::Type gpdType);

	/**
	 * Evaluate specific GPD subtraction constant for a given kinematics.
	 * @param kinematic Reference to the object containing kinematics to be used in the evaluation.
	 * @param gpdType Type of GPD .
	 */
	virtual double compute(const GPDBorderFunctionKinematic &kinematic,
			GPDType gpdType);

	/**
	 * Evaluate for GPD \f$H\f$.
	 */
	virtual double computeH();

	/**
	 * Evaluate for GPD \f$E\f$.
	 */
	virtual double computeE();

	/**
	 * Evaluate for GPD \f$\widetilde{H}\f$.
	 */
	virtual double computeHt();

	/**
	 * Evaluate for GPD \f$\widetilde{E}\f$.
	 */
	virtual double computeEt();

	/**
	 * Get list of available GPD types to be computed.
	 */
	List<GPDType> getListOfAvailableGPDTypeForComputation() const;

protected:

	/**
	 * Copy constructor.
	 * @param other Object to be copied.
	 */
	GPDSubtractionConstantModule(const GPDSubtractionConstantModule &other);

	virtual void initModule();
	virtual void isModuleWellConfigured();

	/**
	 * Evaluate specific GPD subtraction constant for a given kinematics.
	 * @param xi Skewness variable.
	 * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
	 * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
	 * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
	 * @param gpdType Type of GPD.
	 */
	void preCompute(double xi, double t, double MuF2, double MuR2,
			GPDType::Type gpdType);

	/**
	 * Skewness variable.
	 */
	double m_xi;

	/**
	 * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
	 */
	double m_t;

	/**
	 * Factorization scale squared (in \f$GeV^{2}\f$).
	 */
	double m_MuF2;

	/**
	 * Renormalization scale squared (in \f$GeV^{2}\f$).
	 */
	double m_MuR2;

	/**
	 * GPD type.
	 */
	GPDType::Type m_gpdType;

	/**
	 * List of available GPDs to be computed.
	 */
	std::map<GPDType::Type, double (GPDSubtractionConstantModule::*)()> m_listGPDComputeTypeAvailable;

	/**
	 *  Iterator of list of available GPDs to be computed
	 */
	std::map<GPDType::Type, double (GPDSubtractionConstantModule::*)()>::iterator m_it;
};

#endif /* GPDSUBTRACTIONCONSTANTMODULE_H_ */
