#ifndef OBSERVABLE_MODULE_H
#define OBSERVABLE_MODULE_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 19 november 2014
 * @version 1.0
 */

#include <string>

#include "ModuleObject.h"

class ObservableModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    ObservableModule(const std::string &className);

    virtual ObservableModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~ObservableModule();

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    ObservableModule(const ObservableModule &other);

    // Invariant scalars
    double m_xB;
    double m_t;
    double m_Q2;

    // Frame dependent scalars

    // 4-vectors defined in the CM frame

private:

};

#endif /* OBSERVABLE_MODULE_H */
