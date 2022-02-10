#ifndef DDVCS_PROCESS_GV08_H
#define DDVCS_PROCESS_GV08_H

#include <complex>
#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DDVCSProcessModule.h"

namespace PARTONS {

/**
 * @class DDVCSProcessTEST
 *
 * @brief TODO
 *
 * TODO
 */
class DDVCSProcessTEST: public DDVCSProcessModule {
public:

	static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

	/**
	 * Constructor.
	 * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
	 *
	 * @param className name of child class.
	 */
	DDVCSProcessTEST(const std::string &className);

	/**
	 * Destructor.
	 */
	virtual ~DDVCSProcessTEST();

	virtual DDVCSProcessTEST* clone() const;

protected:

	/**
	 * Copy constructor.
	 */
	DDVCSProcessTEST(const DDVCSProcessTEST& other);

	virtual void initModule();
	virtual void isModuleWellConfigured();

	virtual PhysicalType<double> CrossSectionBH();
	virtual PhysicalType<double> CrossSectionVCS();
	virtual PhysicalType<double> CrossSectionInterf();

private:

	//TODO: description
	double bh_squared(double Mnucleon, double t, double phi, double phil,
			double Q2, double eta, double xi, double y, double ytilde,
			double F1, double F2, double tmin, double tmax, double edgeFactor,
			double P1, double P2, double P3, double P4, int beamSign,
			double charge_e) const;

	//TODO: description
	double crossSectionBH(double ml, double Ebeam, double Mnucleon, double xB,
			double t, double Qcal2, double Mll2, double phi, double phil,
			double thetal, int beamSign, int polariz) const;

	double vcs_squared(double Mnucleon, double t, double phi, double phil,
			double Q2, double eta, double xi, double y, double ytilde,
			double edgeFactor, const std::complex<double>& cffH,
			const std::complex<double>& cffE, const std::complex<double>& cffHL,
			const std::complex<double>& cffEL,
			const std::complex<double>& cffHtilde,
			const std::complex<double>& cffEtilde, double charge_e) const;

	double crossSectionVCS(double ml, double Ebeam, double Mnucleon, double xB,
			double t, double Qcal2, double Mll2, double phi, double phil,
			double thetal, int beamSign, int polariz) const;

	double int_squared(double Mnucleon, double t, double phi, double phil,
			double Q2, double eta, double xi, double y, double ytilde,
			double F1, double F2, double edgeFactor, double Kcap,
			double Ktildecap, const std::complex<double>& cffH,
			const std::complex<double>& cffE, const std::complex<double>& cffHL,
			const std::complex<double>& cffEL,
			const std::complex<double>& cffHtilde, double P1, double P2,
			double P3, double P4, double charge_e, int beamSign, int polariz,
			double lambda) const;

	double crossSectionInterf(double ml, double Ebeam, double Mnucleon,
			double xB, double t, double Qcal2, double Mll2, double phi,
			double phil, double thetal, int beamSign, int polariz,
			double lambda) const;

	void computeInternalVariables(double ml, double Ebeam, double Mnucleon,
			double xB, double t, double Qcal2, double Mll2, double phi,
			double phil, double thetal);

	double m_BM_Q2;
	double m_BM_eta;
	double m_BM_xi;
	double m_BM_y;
	double m_BM_ytilde;
	double m_BM_ytildePlus;
	double m_BM_F1;
	double m_BM_F2;
	double m_BM_charge_e;

	double m_BM_tmin;
	double m_BM_tmax;

	double m_BM_edgeFactor;

	double m_BM_Kcap;
	double m_BM_Ktildecap;
	double m_BM_KtildecapPlus;

	double m_BM_kDelta;
	double m_BM_lminusDelta;
	double m_BM_lplusDelta;

	double m_BM_P1;
	double m_BM_P2;
	double m_BM_P3;
	double m_BM_P4;

	//beta variable
	double m_BM_beta;

	double m_BM_xBmin;
	double m_BM_xBmax;
	double m_BM_Qcal2min;
	double m_BM_Qcal2max;
	double m_BM_Mll2min;
	double m_BM_Mll2max;

	//CFFs
	std::complex<double> m_cffH;
	std::complex<double> m_cffE;
	std::complex<double> m_cffHt;
	std::complex<double> m_cffEt;
	std::complex<double> m_cffHL;
	std::complex<double> m_cffEL;
};

} /* namespace PARTONS */

#endif /* DDVCS_PROCESS_GV08_H */

