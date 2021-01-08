/*
 * evolution.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Paweł Sznajder and Arkadiusz P. Trawiński
 */

#include "../../../../include/partons/modules/gpd_subtraction_constant/GPDSubtractionConstantDLMSTW21Evolution.h"

#include <cstdlib>
#include <iostream>
#include <iterator>

namespace PARTONS {
namespace GPDSubtractionConstantDLMSTW21Evolution {

size_t getNActiveFlavors(double mu2) {

	if (mu2 >= c_m_u2 && mu2 < c_m_d2) {
		return 1;
	} else if (mu2 >= c_m_d2 && mu2 < c_m_s2) {
		return 2;
	} else if (mu2 >= c_m_s2 && mu2 < c_m_c2) {
		return 3;
	} else if (mu2 >= c_m_c2 && mu2 < c_m_b2) {
		return 4;
	} else if (mu2 >= c_m_b2 && mu2 < c_m_t2) {
		return 5;
	} else if (mu2 >= c_m_t2) {
		return 6;
	}

	return 0;
}

double getThreshold2(size_t nf) {

	if (nf == 1) {
		return c_m_u2;
	} else if (nf == 2) {
		return c_m_d2;
	} else if (nf == 3) {
		return c_m_s2;
	} else if (nf == 4) {
		return c_m_c2;
	} else if (nf == 5) {
		return c_m_b2;
	} else if (nf == 6) {
		return c_m_t2;
	}

	return 0.;
}

double lambdaQCD(size_t nf) {

	if (nf < 2) {
		std::cout << "lambdaQCD(): nf cann't be smaller than 2" << std::endl;
		exit(0);
	} else if (nf > 6) {
		std::cout << "lambdaQCD(): nf cann't be larger than 6" << std::endl;
		exit(0);
	} else if (nf == 2) {
		return 0.14442;
	} else {
		double m = sqrt(getThreshold2(nf));
		double lamdaQCDsmall = lambdaQCD(nf - 1);
		return lamdaQCDsmall * pow(lamdaQCDsmall / m, 2. / (33. - 2. * nf));
	}

	return 0;
}

double lambdaQCD(double mu2) {

	size_t nf = getNActiveFlavors(mu2);

	if (nf < 2) {
		std::cout << "lambdaQCD(): mu2 smaller than u quark mass" << std::endl;
		exit(0);
	} else
		return lambdaQCD(nf);

	return 0.;
}

double beta(size_t i, size_t nf) {

	switch (i) {
	case 0:
		return 11. - 2. * nf / 3.;
		break;

	default:
		std::cout << "beta(): beta_" << i << " undefined" << std::endl;
		exit(0);
		break;
	}

	return 0.;
}

double alphaS(double mu2) {

	if (mu2 <= 0.) {
		std::cout << "alphaS(): illegal mu2 = " << mu2 << std::endl;
		exit(0);
	}

	size_t nf = getNActiveFlavors(mu2);

	return 4 * M_PI / (beta(0, nf) * log(mu2 / (pow(lambdaQCD(mu2), 2))));
}

double evolutionGamma(size_t n, int sign, size_t nf) {

	if (abs(sign) > 1) {
		std::cout << "evolutionGamma(): illegal sign = " << sign << std::endl;
		exit(0);
	}

	double sum = 0.;

	for (size_t k = 2; k <= (n + 1); k++) {
		sum += 1. / k;
	}

	double gammaQQ = c_cf * (0.5 - (1. / ((n + 1.) * (n + 2.))) + 2 * sum);

	if (sign == 0)
		return gammaQQ;

	double gammaQG = -1. * nf * c_tf * (n * n + 3 * n + 4.)
			/ (n * (n + 1.) * (n + 2.));
	double gammaGQ = -2. * c_cf * (n * n + 3 * n + 4.)
			/ ((n + 1.) * (n + 2.) * (n + 3.));
	double gammaGG =
			c_ca
					* (1. / 6. - 2. / (n * (n + 1)) - 2. / ((n + 2) * (n + 3))
							+ 2 * sum) + (2. / 3.) * nf * c_tf;
	return 0.5
			* (gammaQQ + gammaGG
					+ sign
							* sqrt(
									pow(gammaQQ - gammaGG, 2)
											+ 4 * gammaQG * gammaGQ));
}

double evolutionA(size_t n, int sign, size_t nf) {

	if (abs(sign) != 1) {
		std::cout << "evolutionA(): illegal sign = " << sign << std::endl;
		exit(0);
	}

	double sum = 0.;

	for (size_t k = 2; k <= (n + 1); k++) {
		sum += 1. / k;
	}

	double gammaQQ = c_cf * (0.5 - (1. / ((n + 1.) * (n + 2.))) + 2 * sum);
	double gammaQG = -1. * nf * c_tf * (n * n + 3 * n + 4.)
			/ (n * (n + 1.) * (n + 2.));

	return 2. * nf / n * (evolutionGamma(n, sign, nf) - gammaQQ) / gammaQG;
}

double evolveQuarkDiff(double mu2, double mu20, double dQuarkDiff, size_t n,
		size_t nf) {

	return dQuarkDiff
			* pow(alphaS(mu2) / alphaS(mu20),
					2 * evolutionGamma(n, 0, nf) / beta(0, nf));
}

double evolveQuarkSum(double mu2, double mu20, double dQuarkSum, double dGluon,
		size_t n, size_t nf) {

	double aPlus = evolutionA(n, +1, nf);
	double aMinus = evolutionA(n, -1, nf);

	double dPlus = (dGluon - aMinus * dQuarkSum / nf) / (aPlus - aMinus);
	double dMinus = (-dGluon + aPlus * dQuarkSum / nf) / (aPlus - aMinus);

	return dPlus * nf
			* pow(alphaS(mu2) / alphaS(mu20),
					2 * evolutionGamma(n, +1, nf) / beta(0, nf))
			+ dMinus * nf
					* pow(alphaS(mu2) / alphaS(mu20),
							2 * evolutionGamma(n, -1, nf) / beta(0, nf));
}

double evolveGluon(double mu2, double mu20, double dQuarkSum, double dGluon,
		size_t n, size_t nf) {

	double aPlus = evolutionA(n, +1, nf);
	double aMinus = evolutionA(n, -1, nf);

	double dPlus = (dGluon - aMinus * dQuarkSum / nf) / (aPlus - aMinus);
	double dMinus = (-dGluon + aPlus * dQuarkSum / nf) / (aPlus - aMinus);

	return aPlus * dPlus
			* pow(alphaS(mu2) / alphaS(mu20),
					2 * evolutionGamma(n, +1, nf) / beta(0, nf))
			+ aMinus * dMinus
					* pow(alphaS(mu2) / alphaS(mu20),
							2 * evolutionGamma(n, -1, nf) / beta(0, nf));
}

std::vector<double> evolveQuarkGluon(double mu2, double mu20,
		std::vector<double> dPartons, size_t n, size_t nf) {

	if (nf < 2) {
		std::cout << "evolveQuarkGluon(): nf cann't be smaller than 2"
				<< std::endl;
		exit(0);
	} else if (nf > 6) {
		std::cout << "evolveQuarkGluon(): nf cann't be larger than 6"
				<< std::endl;
		exit(0);
	} else if (dPartons.size() != nf + 1) {
		std::cout << "evolveQuarkGluon(): nf does't match size of dPartons "
				<< std::endl;
		exit(0);
	}

	/* Gluons are the first parton */
	double dGluon = dPartons.front();

	/* Computing sum over quarks */
	double dQuarkSum = 0;
	for (std::vector<double>::iterator it = dPartons.begin() + 1;
			it != dPartons.end(); it++)
		dQuarkSum += *it;

	/* Vector to return */
	std::vector<double> dResult;

	/* Computing evolved gluons and returing it */
	double dGluonEvolved = evolveGluon(mu2, mu20, dQuarkSum, dGluon, n, nf);
	dResult.push_back(dGluonEvolved);

	/* Computing evolved sum over all quarks */
	double dQuarkSumEvolved = evolveQuarkSum(mu2, mu20, dQuarkSum, dGluon, n,
			nf);

	/* Computing evolved difference of quarks in respect to the first one */
	std::vector<double> dQuarkDiffEvolved;
	double dQuark1 = *(dPartons.begin() + 1);

	for (std::vector<double>::iterator it = dPartons.begin() + 2;
			it != dPartons.end(); it++)
		dQuarkDiffEvolved.push_back(
				evolveQuarkDiff(mu2, mu20, *it - dQuark1, n, nf));

	/* Computing evolved first quark and returing it*/
	double dQuark1Evolved = dQuarkSumEvolved;
	for (std::vector<double>::iterator it = dQuarkDiffEvolved.begin();
			it != dQuarkDiffEvolved.end(); it++)
		dQuark1Evolved -= *it;
	dQuark1Evolved /= nf;

	dResult.push_back(dQuark1Evolved);

	/* Returning of other quarks */
	for (std::vector<double>::iterator it = dQuarkDiffEvolved.begin();
			it != dQuarkDiffEvolved.end(); it++)
		dResult.push_back(dQuark1Evolved + *it);

	return dResult;
}

std::vector<double> evolveQuarkGluon(double mu2, double mu20,
		std::vector<double> dPartons, size_t n) {
	size_t nf_mu2 = getNActiveFlavors(mu2);
	size_t nf_mu20 = getNActiveFlavors(mu20);

	if (dPartons.size() != nf_mu20 + 1) {
		std::cout << "evolveQuarkGluon(): At scale mu20 = " << mu20
				<< " it is required of " << nf_mu20
				<< " active quarks, but provided " << dPartons.size() - 1
				<< " in dPartons." << std::endl;
		exit(0);
	}

	if (n % 2 == 0) {
		std::cout << "evolveQuarkGluon(): n has to be odd" << std::endl;
		exit(0);
	}

	if (nf_mu2 > nf_mu20) {
		double m2 = getThreshold2(nf_mu20 + 1);
		std::vector<double> dPartonsEvolved = evolveQuarkGluon(m2, mu20,
				dPartons, n, nf_mu20);
		dPartonsEvolved.push_back(0); /* contribution form heavier quark is generated radiatively */
		return evolveQuarkGluon(mu2, m2, dPartonsEvolved, n);
	}

	if (nf_mu2 < nf_mu20) {
		double m2 = getThreshold2(nf_mu2 + 1);
		std::vector<double> dPartonsEvolved = evolveQuarkGluon(m2, mu20,
				dPartons, n);
		dPartonsEvolved.pop_back(); /* removing contribution from the heaviest quark */
		return evolveQuarkGluon(mu2, m2, dPartonsEvolved, n, nf_mu2);
	}
	/* nf_mu2 ==  nf_mu20 */
	return evolveQuarkGluon(mu2, mu20, dPartons, n, nf_mu20);
}

}
}
