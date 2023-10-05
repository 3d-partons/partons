#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessBMP14.h"

#include <cmath>
#include <complex>
#include <vector>

#include <partons/FundamentalPhysicalConstants.h>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {

const unsigned int DVCSProcessBMP14::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSProcessBMP14("DVCSProcessBMP14"));

/*--------------------------------------- Constructors ---------------------------------*/

DVCSProcessBMP14::DVCSProcessBMP14(const std::string &className) :
        DVCSProcessBMJ12(className) {
}

/*-------------------------------------- Destructor ------------------------------------*/

DVCSProcessBMP14::~DVCSProcessBMP14() {
}

DVCSProcessBMP14::DVCSProcessBMP14(const DVCSProcessBMP14& other) :
        DVCSProcessBMJ12(other) {
}

DVCSProcessBMP14* DVCSProcessBMP14::clone() const {
    return new DVCSProcessBMP14(*this);
}

void DVCSProcessBMP14::computeCFFs() {

    // Compute coefficients in BMJ12's CFFs F+b and F0+ by means of BMP14's leading twist (LT)

    /*
     * cff_F: BMP14's CFFs. The convolution between T0 and GPDs is given in eqs 39, 40. The LT is given in eqs 69a, 69b.
     * 1st index: 0 H, 1 E, 2 Ht, 3 Et GPDs
     * 2nd index: 0 F++, 1 F+-, 2 F0+
     */
    std::vector<std::vector<std::complex<double> > > cff_F;
    cff_F.assign(4, std::vector<std::complex<double> >(3, (0., 0.)));
    int h, g;

//BMP14's LT eqs 69a in BMP14:
    cff_F[0][0] = getConvolCoeffFunctionValue(GPDType::H);
    cff_F[1][0] = getConvolCoeffFunctionValue(GPDType::E);
    cff_F[2][0] = getConvolCoeffFunctionValue(GPDType::Ht);
    cff_F[3][0] = (1. + m_t / m_Q2) * getConvolCoeffFunctionValue(GPDType::Et)
            + (4. * pow(Constant::PROTON_MASS, 2.) / m_Q2) * cff_F[2][0];

    double gamma2 = pow(2. * Constant::PROTON_MASS * m_xB, 2.) / m_Q2; //squared of eq 5 in BMP14

    double kappa = (m_Q2 - m_t + 2. * m_xB * m_t)
            / (sqrt(1. + gamma2) * (m_Q2 + m_t)) - 1.; //eq 48 in BMP14
    double kappaZero = sqrt(2. / (1. + gamma2)) * sqrt(m_Q2) * m_Kt[0]
            / (m_Q2 + m_t); //eq 48 in BMP14

    //m_CFF: BMJ12's CFFs by means of BMP14's CFFs:
//    m_CFF.assign(4, std::vector<std::complex<double> >(3, 0.));
    for (g = 0; g < 4; g++) {
        //Eq 47 in BMP14 (general transformation from BMP14's to BMJ12's CFFs):
        m_CFF[g][0] = cff_F[g][0] + 0.5 * kappa * (cff_F[g][0] + cff_F[g][1])
                - kappaZero * cff_F[g][2];
        m_CFF[g][1] = cff_F[g][1] + 0.5 * kappa * (cff_F[g][0] + cff_F[g][1])
                - kappaZero * cff_F[g][2];
        m_CFF[g][2] = -(1. + kappa) * cff_F[g][2]
                + kappaZero * (cff_F[g][0] + cff_F[g][1]);

    }


}

} /* namespace PARTONS */
