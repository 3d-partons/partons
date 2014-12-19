#include <complex>
#include <string>

#include "DVCSCFFModel.h"

class DVCSCFFFourQuarksModel: public DVCSCFFModel {
public:
    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

    DVCSCFFFourQuarksModel(const std::string &className);

    virtual DVCSCFFFourQuarksModel* clone() const;

protected:
    /**
     * Copy constructor
     * @param other
     */
    DVCSCFFFourQuarksModel(const DVCSCFFFourQuarksModel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> KernelGluonNLOV(double x); ///< T^{g, NLO, V}, appendix A, eq. (A2)
private:

    double m_Constant;

};
