#include <string>

#include "ModuleObject.h"

class RunningAlphaStrongModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    RunningAlphaStrongModule(const std::string &className);

    virtual RunningAlphaStrongModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~RunningAlphaStrongModule();

    virtual double compute(double Mu) = 0;

    unsigned int getNf() const;
    void setNf(unsigned int nf);

protected:

    /**
     * Copy constructor
     *
     * @param other
     */
    RunningAlphaStrongModule(const RunningAlphaStrongModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    double m_Mu;            ///< Current renormalization scale (in GeV)
    unsigned int m_nf;      ///< Number of active flavours
};
