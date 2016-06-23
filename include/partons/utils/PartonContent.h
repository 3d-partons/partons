#ifndef PARTON_CONTENT_H
#define PARTON_CONTENT_H

/**
 * @file PartonContent.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 21, 2016
 * @version 1.0
 */

#include "../BaseObject.h"

/**
 * @class PartonContent
 *
 * @brief
 */
class PartonContent: public BaseObject {
public:
    PartonContent();
    virtual ~PartonContent();

    double getBbar() const;
    void setBbar(double bbar);
    double getBot() const;
    void setBot(double bot);
    double getCbar() const;
    void setCbar(double cbar);
    double getChm() const;
    void setChm(double chm);
    double getDnv() const;
    void setDnv(double dnv);
    double getDsea() const;
    void setDsea(double dsea);
    double getGlu() const;
    void setGlu(double glu);
    double getPhot() const;
    void setPhot(double phot);
    double getSbar() const;
    void setSbar(double sbar);
    double getStr() const;
    void setStr(double str);
    double getUpv() const;
    void setUpv(double upv);
    double getUsea() const;
    void setUsea(double usea);

private:
    double upv;
    double dnv;
    double usea;
    double dsea;
    double str;
    double sbar;
    double chm;
    double cbar;
    double bot;
    double bbar;
    double glu;
    double phot;
};

#endif /* PARTON_CONTENT_H */
