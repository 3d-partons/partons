#ifndef PARTON_CONTENT_H
#define PARTON_CONTENT_H

/**
 * @file PartonContent.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 21, 2016
 * @version 1.0
 */

#include "../BaseObject.h"

namespace PARTONS {

/**
 * @class PartonContent
 *
 * @brief Container to store single numbers for photons, gluons and all quark flavors.
 *
 * This class acts as a container to store single double precision numbers for photons, gluons and all quark flavors.
 * It can be used in particular to store an output of a given PDF parameterization.
 */
class PartonContent: public BaseObject {

public:

    /**
     * Default constructor.
     */
    PartonContent();

    /**
     * Destructor.
     */
    virtual ~PartonContent();

    PartonContent* clone() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    double getUpv() const;      ///< Get value for \f$u_v\f$ quarks.
    void setUpv(double upv);    ///< Set value for \f$u_v\f$ quarks.
    double getDnv() const;      ///< Get value for \f$d_v\f$ quarks.
    void setDnv(double dnv);    ///< Set value for \f$d_v\f$ quarks.
    double getUsea() const;     ///< Get value for \f$u_{\mathrm{sea}}\f$ quarks.
    void setUsea(double usea);  ///< Set value for \f$u_{\mathrm{sea}}\f$ quarks.
    double getDsea() const;     ///< Get value for \f$d_{\mathrm{sea}}\f$ quarks.
    void setDsea(double dsea);  ///< Set value for \f$d_{\mathrm{sea}}\f$ quarks.
    double getStr() const;      ///< Get value for \f$s\f$ quarks.
    void setStr(double str);    ///< Set value for \f$s\f$ quarks.
    double getSbar() const;     ///< Get value for \f$\bar{s}\f$ quarks.
    void setSbar(double sbar);  ///< Set value for \f$\bar{s}\f$ quarks.
    double getChm() const;      ///< Get value for \f$c\f$ quarks.
    void setChm(double chm);    ///< Set value for \f$c\f$ quarks.
    double getCbar() const;     ///< Get value for \f$\bar{c}\f$ quarks.
    void setCbar(double cbar);  ///< Set value for \f$\bar{c}\f$ quarks.
    double getBot() const;      ///< Get value for \f$b\f$ quarks.
    void setBot(double bot);    ///< Set value for \f$b\f$ quarks.
    double getBbar() const;     ///< Get value for \f$\bar{b}\f$ quarks.
    void setBbar(double bbar);  ///< Set value for \f$\bar{b}\f$ quarks.
    double getTop() const;      ///< Get value for \f$t\f$ quarks.
    void setTop(double top);    ///< Set value for \f$t\f$ quarks.
    double getTbar() const;     ///< Get value for \f$\bar{t}\f$ quarks.
    void setTbar(double tbar);  ///< Set value for \f$\bar{t}\f$ quarks.
    double getGlu() const;      ///< Get value for gluons.
    void setGlu(double glu);    ///< Set value for gluons.
    double getPhot() const;     ///< Get value for photons.
    void setPhot(double phot);  ///< Set value for photons.

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    PartonContent(const PartonContent& other);

private:

    double upv;     ///< Value for \f$u_v\f$ quarks.
    double dnv;     ///< Value for \f$d_v\f$ quarks.
    double usea;    ///< Value for \f$u_{\mathrm{sea}}\f$ quarks.
    double dsea;    ///< Value for \f$d_{\mathrm{sea}}\f$ quarks.
    double str;     ///< Value for \f$s\f$ quarks.
    double sbar;    ///< Value for \f$\bar{s}\f$ quarks.
    double chm;     ///< Value for \f$c\f$ quarks.
    double cbar;    ///< Value for \f$\bar{c}\f$ quarks.
    double bot;     ///< Value for \f$b\f$ quarks.
    double bbar;    ///< Value for \f$\bar{b}\f$ quarks.
    double top;     ///< Value for \f$t\f$ quarks.
    double tbar;    ///< Value for \f$\bar{t}\f$ quarks.
    double glu;     ///< Value for gluons.
    double phot;    ///< Value for photons.
};

} /* namespace PARTONS */

#endif /* PARTON_CONTENT_H */
