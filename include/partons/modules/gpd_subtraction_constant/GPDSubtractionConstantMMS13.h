/*
 * GPDSubtractionConstantMMS13.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef GPDSUBTRACTIONCONSTANTMMS13MODEL_H_
#define GPDSUBTRACTIONCONSTANTMMS13MODEL_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>
#include <vector>

#include "../../beans/automation/BaseObjectData.h"
#include "../GPDSubtractionConstantModule.h"
#include "../MathIntegratorModule.h"

class GPDMMS13;

/**
 * Subtraction constant model based on MM13Model D-term
 */
class GPDSubtractionConstantMMS13: public GPDSubtractionConstantModule,
		public MathIntegratorModule {

public:

	static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

	/**
	 * Default constructor.
	 * @param className Class name.
	 */
	GPDSubtractionConstantMMS13(const std::string &className);

	/**
	 * Destructor.
	 */
	virtual ~GPDSubtractionConstantMMS13();

	virtual GPDSubtractionConstantMMS13* clone() const;
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
	GPDSubtractionConstantMMS13(const GPDSubtractionConstantMMS13& other);

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
	 * Pointer to GPDMMS13 model.
	 */
	GPDMMS13* m_pMMS13Model;
};

#endif /* GPDSUBTRACTIONCONSTANTMMS13MODEL_H_ */
