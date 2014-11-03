
#ifndef VINNIKOV_EVOL_QCD_H
#define VINNIKOV_EVOL_QCD_H

/**
 * @file VinnikovEvolQCD.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 22 October 2014
 *
 * @class VinnikovEvolQCD
 */

#include "../EvolQCDModule.h"

class VinnikovEvolQCDModel: public EvolQCDModule {
public:
	static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

	VinnikovEvolQCDModel();
	virtual ~VinnikovEvolQCDModel();
	virtual VinnikovEvolQCDModel* clone() const;

	virtual GPDResultData compute(const double &x, const double &xi,
			const double &t, const double &MuF, const double &MuR,
			const GPDResultData &gpdResultData);
protected:
	/**
	 * Copy constructor
	 */
	VinnikovEvolQCDModel(const VinnikovEvolQCDModel &other);

	virtual void isModuleWellConfigured();
	virtual void initModule();

private:
	void evolution();
};

#endif /* VINNIKOV_EVOL_QCD_H */
