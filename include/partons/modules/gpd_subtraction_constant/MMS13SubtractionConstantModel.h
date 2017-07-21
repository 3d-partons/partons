/*
 * MMS13SubtractionConstantModel.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef MMS13SUBTRACTIONCONSTANTMODEL_H_
#define MMS13SUBTRACTIONCONSTANTMODEL_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>
#include <vector>

#include "../../beans/automation/BaseObjectData.h"
#include "../GPDSubtractionConstantModule.h"
#include "../MathIntegratorModule.h"

class MMS13Model;

/**
 * Subtraction constant model based on MM13Model D-term
 */
class MMS13SubtractionConstantModel: public GPDSubtractionConstantModule,
		public MathIntegratorModule {

public:

	static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

	/**
	 * Default constructor.
	 * @param className Class name.
	 */
	MMS13SubtractionConstantModel(const std::string &className);

	/**
	 * Destructor.
	 */
	virtual ~MMS13SubtractionConstantModel();

	virtual MMS13SubtractionConstantModel* clone() const;
	virtual void configure(const ElemUtils::Parameters &parameters);
	virtual std::string toString() const;
	virtual void resolveObjectDependencies();
	virtual void prepareSubModules(
			const std::map<std::string, BaseObjectData>& subModulesData);

	virtual double computeH();
	virtual double computeE();

protected:

	/** Copy constructor.
	 @param other Object to be copied.
	 */
	MMS13SubtractionConstantModel(const MMS13SubtractionConstantModel& other);

	virtual void initModule();
	virtual void isModuleWellConfigured();

private:

	/**
	 * Integral of D-term.
	 * @param zeta GPD x/xi variable.
	 * @param par Additional parameters (not used here).
	 */
	double dTermIntegral(double zeta, std::vector<double> par);

	/**
	 * Initialize functors.
	 */
	void initFunctorsForIntegrations();

	/**
	 * Functor related to dTermIntegral() function.
	 */
	NumA::FunctionType1D* m_p_int_dTermIntegral;

	/**
	 * Pointer to MMS13Model model.
	 */
	MMS13Model* m_pMMS13Model;
};

#endif /* MMS13SUBTRACTIONCONSTANTMODEL_H_ */
