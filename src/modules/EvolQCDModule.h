#ifndef EVOL_QCD_MODULE_H
#define EVOL_QCD_MODULE_H

/**
 * @file EvolQCDModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 21 October 2014
 *
 * @class EvolQCDModule
 */

#include "ModuleObject.h"
#include "../beans/QCDOrderType.h"
//#include "alphaS/RunningAlphaStrong.h"
#include "../beans/gpd/GPDResultData.h"
#include "../utils/MatrixD.h"

class GPDModule;

class EvolQCDModule: public ModuleObject {
public:
	enum Type {
		RELATIVE, ABSOLUTE, BOTH
	};

	EvolQCDModule(const std::string &moduleID);
	virtual ~EvolQCDModule();
	virtual EvolQCDModule* clone() const = 0;

	virtual GPDResultData compute(const double &x, const double &xi,
			const double &t, const double &MuF, const double &MuR,
			const GPDResultData &gpdResultData) = 0;

	/**
	 * Checks if MuF (from compute parameters) is revelant
	 */
	bool isRunnable(const double &MuF, const double &MuF_ref,
			EvolQCDModule::Type testType);

	// ##### GETTERS & SETTERS #####

protected:
	/**
	 * Copy constructor
	 */
	EvolQCDModule(const EvolQCDModule &other);

	GPDModule* m_pGPDModule;

	double m_x;
	double m_xi;
	double m_t;
	double m_MuF;
	double m_MuR;
	//GPDResultData m_gpdResultData;

	QCDOrderType::Type m_qcdOrderType;
	//RunningAlphaStrong m_runningAlphaS;
	double m_alphaS;
	double m_scaleDistinction;

	double m_singletQuark;
	double m_gluon;

	int m_nfEvol;
	unsigned int m_nfMin;		///< nf_min donne la taille de la matrice
	double m_epsilon;
	double m_alpha;

	GPDResultData m_gpdResultData;

	MatrixD m_currentMatrix;
	std::vector<double> m_vectorOfUnknownData;
	//Matrix<double> m_matrixOfUnknownPartonDistribution; ///< HuPlus, HuMinus, ...

	void preCompute(const GPDResultData &gpdResultData);

	virtual void convertBasis();
	virtual void invertBasis();

private:
	static MatrixD conversionMatrix1;
	static MatrixD conversionMatrix2;
	static MatrixD conversionMatrix3;
	static MatrixD conversionMatrix4;
	static MatrixD conversionMatrix5;
	static MatrixD conversionMatrix6;

	bool isRelativeTest(const double &MuF, const double &MuF_ref);
	bool isAbsoluteTest(const double &MuF, const double &MuF_ref);

	virtual void isModuleConfigured();

	virtual void updateVariables();
	void updateNfMin();
	void updateMatrixValue();
	void initVectorOfGPDCombinations();
};

#endif /* EVOL_QCD_MODULE_H */
