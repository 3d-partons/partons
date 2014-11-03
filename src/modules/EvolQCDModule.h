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

	QCDOrderType::Type getQcdOrderType() const;
	void setQcdOrderType(QCDOrderType::Type qcdOrderType);

	static MatrixD conversionMatrix1;
		static MatrixD conversionMatrix2;
		static MatrixD conversionMatrix3;
		static MatrixD conversionMatrix4;
		static MatrixD conversionMatrix5;
		static MatrixD conversionMatrix6;

		static MatrixD invertMatrix1;
		static MatrixD invertMatrix2;
		static MatrixD invertMatrix3;
		static MatrixD invertMatrix4;
		static MatrixD invertMatrix5;
		static MatrixD invertMatrix6;

protected:
	/**
	 * Copy constructor
	 */
	EvolQCDModule(const EvolQCDModule &other);

	double m_x;
	double m_xi;
	double m_t;
	double m_MuF;
	double m_MuR;

	//RunningAlphaStrong m_runningAlphaS;
	double m_alphaS;
	double m_scaleDistinction;

	int m_nfEvol;
	int m_nfMin;		///< nf_min donne la taille de la matrice
	double m_epsilon;
	double m_alpha;

	QCDOrderType::Type m_qcdOrderType;

	GPDResultData m_gpdResultData;

	MatrixD m_currentConvertMatrix;
	MatrixD m_currentInvertMatrix;

	std::vector<double> m_vectorOfGPDCombination;
	//Matrix<double> m_matrixOfUnknownPartonDistribution; ///< HuPlus, HuMinus, ...

	void preCompute(const double &x, const double &xi, const double &t,
			const double &MuF, const double &MuR,
			const GPDResultData &gpdResultData);

	virtual void initModule();
	virtual void isModuleWellConfigured();

	virtual void convertBasis();
	virtual void invertBasis();

	GPDResultData makeGPDResultData();

private:
	/*
	static MatrixD conversionMatrix1;
	static MatrixD conversionMatrix2;
	static MatrixD conversionMatrix3;
	static MatrixD conversionMatrix4;
	static MatrixD conversionMatrix5;
	static MatrixD conversionMatrix6;

	static MatrixD invertMatrix1;
	static MatrixD invertMatrix2;
	static MatrixD invertMatrix3;
	static MatrixD invertMatrix4;
	static MatrixD invertMatrix5;
	static MatrixD invertMatrix6;
	*/

	bool isRelativeTest(const double &MuF, const double &MuF_ref);
	bool isAbsoluteTest(const double &MuF, const double &MuF_ref);

	void initNfMin();
	void initMatrixValue();
	void initVectorOfGPDCombinations();

	double calculateFq(double FMinus, double FPlus);
};

#endif /* EVOL_QCD_MODULE_H */
