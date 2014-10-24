#ifndef GPD_QUARK_FLAVOR_DATA
#define GPD_QUARK_FLAVOR_DATA

/**
 * @file GPDQuarkFlavorData.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 30 Juil 2014
 * @version 1.0
 */

#include <string>

#include "../QuarkFlavor.h"
#include "GPDComputeType.h"

//TODO GPD -> PartonDistributionQuarkFlavorData
class GPDQuarkFlavorData {

public:
	GPDQuarkFlavorData(GPDComputeType &_gpdComputeType,
			QuarkFlavor::Type _quarkFlavorType);

	virtual ~GPDQuarkFlavorData();

	std::string toString();

	double getHq() const;

	void setHq(double hq);

	double getSea() const;

	void setSea(double hqSea);

	double getSinglet() const;

	void setSinglet(double hqSinglet);

	double getValence() const;

	void setValence(double hqVal);

	QuarkFlavor* getQuarkFlavor();

	void setQuarkFlavor(QuarkFlavor &_quarkFlavor);

	double getPartonDistributionMinus() const;
	void setPartonDistributionMinus(double partonDistributionMinus);
	double getPartonDistributionPlus() const;
	void setPartonDistributionPlus(double partonDistributionPlus);

private:
	GPDComputeType m_gpdComputeType;
	QuarkFlavor m_quarkFlavor;

	//TODO changer le nom de cette variable; Hq -> partonDistribution
	double Hq;          ///< GPD(q), flavour singlet
	double valence;     ///< GPD(q), valence part
	double sea;         ///< GPD(q), sea part

	//TODO singlet -> devient pdPlus
	double pdSinglet;     ///< GPD(q), charge singlet part

	//TODO faire référence à la revue paraph 3.3.2 de Markus Diehl
	double m_partonDistributionPlus;		///<
	double m_partonDistributionMinus;		///<

	//TODO a voir
	double quark;
	double antiQuark;
};

#endif /* GPD_QUARK_FLAVOR_DATA */
