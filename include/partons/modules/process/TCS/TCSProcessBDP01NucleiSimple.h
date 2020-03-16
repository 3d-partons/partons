/*
 * TCSProcessBDP01NucleiSimple.h
 *
 *  Created on: Mar 11, 2020
 *      Author: partons
 */

#ifndef INCLUDE_PARTONS_MODULES_PROCESS_TCS_TCSPROCESSBDP01NUCLEISIMPLE_H_
#define INCLUDE_PARTONS_MODULES_PROCESS_TCS_TCSPROCESSBDP01NUCLEISIMPLE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../beans/nuclei/Nuclei.h"
#include "../../../utils/type/PhysicalType.h"
#include "TCSProcessBDP01.h"

namespace PARTONS {

class TCSProcessBDP01NucleiSimple: public TCSProcessBDP01, public Nuclei {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    TCSProcessBDP01NucleiSimple(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSProcessBDP01NucleiSimple();

    virtual TCSProcessBDP01NucleiSimple* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other
     */
    TCSProcessBDP01NucleiSimple(const TCSProcessBDP01NucleiSimple &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSectionBH();
    virtual PhysicalType<double> CrossSectionInterf();
    virtual PhysicalType<double> CrossSectionVCS();
};

} /* namespace PARTONS */

#endif /* INCLUDE_PARTONS_MODULES_PROCESS_TCS_TCSPROCESSBDP01NUCLEISIMPLE_H_ */
