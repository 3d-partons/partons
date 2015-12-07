//#ifndef VINNIKOV_EVOL_QCD_H
//#define VINNIKOV_EVOL_QCD_H
//
///**
// * @file VinnikovEvolQCD.h
// * @author Bryan BERTHOU (SPhN / CEA Saclay)
// * @author C. Mezrag (SPhN / CEA Saclay)
// * @author H. MOUTARDE (SPhN / CEA Saclay)
// * @date 22 October 2014
// * @version 1.0
// *
// * @class VinnikovEvolQCD
// *
// * @brief
// */
//
//#include <string>
//#include <vector>
//
//#include "../GPDEvolutionModule.h"
//
//class VinnikovEvolQCDModel: public GPDEvolutionModule {
//public:
//    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.
//
//    VinnikovEvolQCDModel(const std::string &className);
//    virtual ~VinnikovEvolQCDModel();
//    virtual VinnikovEvolQCDModel* clone() const;
//
//    void setNbMuFPoints(unsigned int nbMuFPoints);
//    void setNbXPoints(unsigned int nbXPoints);
//
////    virtual PartonDistribution compute(double x, double xi, double t,
////            double MuF, double MuR, GPDModule* pGPDModule);
//
//protected:
//    /**
//     * Copy constructor
//     */
//    VinnikovEvolQCDModel(const VinnikovEvolQCDModel &other);
//
//    virtual void isModuleWellConfigured();
//    virtual void initModule();
//
//private:
//
//    double m_nbColor;
//    double m_CF;
//    double m_CA;
//    unsigned int m_nbActiveFlavor;
//    double m_b0;
//    double m_TR;
//    double m_LnMuF2_ref; //lnQ2Ini
//    double m_LnMuF2; //lnQ2Fin
//    std::vector<double> m_NonSingletGpd;
//    std::vector<double> m_SingletGpd;
//    std::vector<double> m_GluonGpd;
//    std::vector<double> m_Singletx;
//    std::vector<double> m_NonSingletx;
//
//    void evolution();
//
//    void evolutionNonSingletGPD(unsigned int iNS);
//    void evolutionSingletGPD();
//
//    double interpolate(std::vector<double> xGrid, std::vector<double> yGrid,
//            double x);
//
//    void buildNonSingletxGrid(); // Build the logarithm grid for computation
//    void buildSingletxGrid(); // Build Grid for singlet case
//    void initNonSingletGPD(unsigned int iNS); // Compute initial values of GPD for non singlet quark
//    void initSingletGPD(); // Compute initial values of GPD for singlet quark and gluons
//    double alpha_s(double t); // Compute strong coupling constant
//
//    double convolNonSingletKernel(unsigned int const ix, double const xi,
//            int const n, double const * const gpd, double const * const x);
//    double convolSingletqqKernel(unsigned int const ix, double const xi,
//            int const n, double const * const gpd, double const * const x);
//    double convolSingletggKernel(unsigned int const ix, double const xi,
//            int const n, double const * const gpd, double const * const x);
//    double convolSingletgqKernel(unsigned int const ix, double const xi,
//            int const n, double const * const gpd, double const * const x);
//    double convolSingletqgKernel(unsigned int const ix, double const xi,
//            int const n, double const * const gpd, double const * const x);
//
//};
//
//#endif /* VINNIKOV_EVOL_QCD_H */
