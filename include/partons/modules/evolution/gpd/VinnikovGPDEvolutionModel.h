#ifndef VINNIKOV_GPD_EVOLUTION_MODEL_H
#define VINNIKOV_GPD_EVOLUTION_MODEL_H

/**
 * @file VinnikovGPDEvolutionModel.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author C. Mezrag (SPhN / CEA Saclay)
 * @author H. MOUTARDE (SPhN / CEA Saclay)
 * @date 26 October 2015
 * @version 1.0
 *
 * @class VinnikovGPDEvolutionModel
 *
 * @brief See Vinnikov paper arXiv:hep-ph/0604248v1 for details on the implementation.
 */

#include <string>
#include <vector>

#include "../GPDEvolutionModule.h"

class VinnikovGPDEvolutionModel: public GPDEvolutionModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    VinnikovGPDEvolutionModel(const std::string &className);
    virtual ~VinnikovGPDEvolutionModel();
    virtual VinnikovGPDEvolutionModel* clone() const;

    virtual void configure(ParameterList parameters);

    unsigned int getSizeMuF2Grid() const;
    void setSizeMuF2Grid(unsigned int sizeMuF2Grid);
    unsigned int getSizeXGrid() const;
    void setSizeXGrid(unsigned int sizeXGrid);

    static const std::string PARAMETER_NAME_SIZE_MUF2_GRID;
    static const std::string PARAMETER_NAME_SIZE_X_GRID;

protected:
    /**
     * Copy constructor
     */
    VinnikovGPDEvolutionModel(const VinnikovGPDEvolutionModel &other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual double integratedNonSingletMuFDerivative(
            const NfInterval &nfInterval);
    virtual double integratedSingletMuFDerivative(const NfInterval &nfInterval);
    virtual double integratedGluonMuFDerivative(const NfInterval &nfInterval);

private:

    double m_nbColor;
    double m_CF;
    double m_CA;
    unsigned int m_nbActiveFlavor;
    double m_b0;
    double m_TR;
    double m_LnMuF2_ref; //lnQ2Ini
    double m_LnMuF2; //lnQ2Fin
    unsigned int m_sizeMuF2Grid; // m in Vinnikov paper
    unsigned int m_sizeXGrid; // n in Vinnikov paper, so total grid size is 4n+1!!
    std::vector<double> m_nonSingletGpd;
    std::vector<double> m_singletGpd;
    std::vector<double> m_gluonGpd;
    std::vector<double> m_singletx;
    std::vector<double> m_nonSingletx;

    void evolution();

//    void evolutionNonSingletGPD(
//            unsigned short nonSingletIndex, unsigned short currentNf,
//            double MuF2Initial, double MuF2Final); // Use protected member variables instead
    void evolutionNonSingletGPD(double MuF2Initial, double MuF2Final);
    void evolutionSingletGPD();

    double interpolate(std::vector<double> xGrid, std::vector<double> yGrid,
            double x);

    void buildNonSingletxGrid(); // Build the logarithm grid for computation
    void buildSingletxGrid(); // Build Grid for singlet case
    // Compute initial values of GPD for non singlet quark
    // void initNonSingletGPD(unsigned short nonSingletIndex, unsigned short currentNf, double currentMuF2);
    void initNonSingletGPD(); // Use protected member variables instead
    void initSingletGPD(); // Compute initial values of GPD for singlet quark and gluons
    double alpha_s(double t); // Compute strong coupling constant

    double discretizedIntegratedNonSingletMuFDerivative(
            const NfInterval &nfInterval);
    double discretizedIntegratedSingletMuFDerivative(const NfInterval &nfInterval);
    double discretizedIntegratedGluonMuFDerivative(const NfInterval &nfInterval);

    double convolNonSingletKernel(unsigned int const ix, double const xi,
            int const n, double const * const gpd, double const * const x);
    double convolSingletqqKernel(unsigned int const ix, double const xi,
            int const n, double const * const gpd, double const * const x);
    double convolSingletggKernel(unsigned int const ix, double const xi,
            int const n, double const * const gpd, double const * const x);
    double convolSingletgqKernel(unsigned int const ix, double const xi,
            int const n, double const * const gpd, double const * const x);
    double convolSingletqgKernel(unsigned int const ix, double const xi,
            int const n, double const * const gpd, double const * const x);

};

#endif /* VINNIKOV_GPD_EVOLUTION_H */
