#include "../../../include/partons/utils/PartonContent.h"

PartonContent::PartonContent() :
        BaseObject("PartonContent") {

    upv = 0.;
    dnv = 0.;
    usea = 0.;
    dsea = 0.;
    str = 0.;
    sbar = 0.;
    chm = 0.;
    cbar = 0.;
    bot = 0.;
    bbar = 0.;
    top = 0.;
    tbar = 0.;
    glu = 0.;
    phot = 0.;
}

PartonContent::PartonContent(const PartonContent& other) :
        BaseObject(other) {

    upv = other.upv;
    dnv = other.dnv;
    usea = other.usea;
    dsea = other.dsea;
    str = other.str;
    sbar = other.sbar;
    chm = other.chm;
    cbar = other.cbar;
    bot = other.bot;
    bbar = other.bbar;
    top = other.top;
    tbar = other.tbar;
    glu = other.glu;
    phot = other.phot;
}

PartonContent* PartonContent::clone() const{
    return new PartonContent(*this);
}

PartonContent::~PartonContent() {
}

double PartonContent::getBbar() const {
    return bbar;
}

void PartonContent::setBbar(double bbar) {
    this->bbar = bbar;
}

double PartonContent::getBot() const {
    return bot;
}

void PartonContent::setBot(double bot) {
    this->bot = bot;
}

double PartonContent::getCbar() const {
    return cbar;
}

void PartonContent::setCbar(double cbar) {
    this->cbar = cbar;
}

double PartonContent::getChm() const {
    return chm;
}

void PartonContent::setChm(double chm) {
    this->chm = chm;
}

double PartonContent::getDnv() const {
    return dnv;
}

void PartonContent::setDnv(double dnv) {
    this->dnv = dnv;
}

double PartonContent::getDsea() const {
    return dsea;
}

void PartonContent::setDsea(double dsea) {
    this->dsea = dsea;
}

double PartonContent::getGlu() const {
    return glu;
}

void PartonContent::setGlu(double glu) {
    this->glu = glu;
}

double PartonContent::getPhot() const {
    return phot;
}

void PartonContent::setPhot(double phot) {
    this->phot = phot;
}

double PartonContent::getSbar() const {
    return sbar;
}

void PartonContent::setSbar(double sbar) {
    this->sbar = sbar;
}

double PartonContent::getStr() const {
    return str;
}

void PartonContent::setStr(double str) {
    this->str = str;
}

double PartonContent::getUpv() const {
    return upv;
}

void PartonContent::setUpv(double upv) {
    this->upv = upv;
}

double PartonContent::getUsea() const {
    return usea;
}

void PartonContent::setUsea(double usea) {
    this->usea = usea;
}

double PartonContent::getTbar() const {
    return tbar;
}

void PartonContent::setTbar(double tbar) {
    this->tbar = tbar;
}

double PartonContent::getTop() const {
    return top;
}

void PartonContent::setTop(double top) {
    this->top = top;
}
