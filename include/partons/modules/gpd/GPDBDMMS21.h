#ifndef GPD_BDMMS21_H
#define GPD_BDMMS21_H

/**
 * @file GPDBDMMS21.h
 * @author  Hervé Dutrieux (CEA Saclay)
 * @date January 28, 2021
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <include/partons/modules/gpd/GPDModule.h>
#include <string>

namespace PARTONS {

/**
 * @class GPDBDMMS21
 *
 * @brief GPD module containing example of NLO ghost GPD.
 *
 * This module provides an example of NLO ghost GPD, i.e.
 * GPD that gives a vanishing contribution to DVCS CFFs evaluated with NLO formulae.
 *
 * The example is for GPD H^u sea contribution only.
 *
 * Reference: 	"The deconvolution problem of deeply virtual Compton scattering",
 * 				V. Bertone, H. Dutrieux, C. Mezrag, H. Moutarde, P. Sznajder,
 * 				to be published.
 */
class GPDBDMMS21: public GPDModule {

public:

	static const unsigned int classId; ///< ID assigned by BaseObjectRegistry.

	/**
	 * Constructor.
	 */
	GPDBDMMS21(const std::string &className);

	/**
	 * Destructor.
	 */
	virtual ~GPDBDMMS21();

	virtual GPDBDMMS21* clone() const;
	virtual void resolveObjectDependencies();
	virtual void configure(const ElemUtils::Parameters &parameters);

protected:

	/**
	 * Copy constructor.
	 */
	GPDBDMMS21(const GPDBDMMS21& other);

	virtual void isModuleWellConfigured();
	virtual void initModule();

	virtual PartonDistribution computeH();
};

} /* namespace PARTONS */

#endif /* GPD_BDMMS21_H */
