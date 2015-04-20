#ifndef ATTRIBUTS_H
#define ATTRIBUTS_H

/**
 * @file Attributs.h
 * @author Adrien KIELB, Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 January 2014
 * @version 1.0
 *
 * @class Attributs
 * @brief
 */

#include <string>
#include <utility>
#include <vector>

class Attributs {
private:
    std::vector<std::pair<std::string, std::string> > sesAtt;
    std::vector<std::pair<std::string, std::string> >::iterator m_it;

public:

    void insertAtt(std::pair<std::string, std::string>);
    void affiche();
    bool verifInsert(std::pair<std::string, std::string>);
    std::vector<std::pair<std::string, std::string> > getAtt();
    std::string getStringValueOf(std::string);
    int getIntValueOf(std::string);
    double getDoubleValueOf(std::string);

    bool isInt(std::string);
};
/*! \class Attributs Attributs.h
 *  \brief Classe pour les attributs des balises.
 *
 * Cette classe contient tous les attributs des balises dans un std::vector<std::pair<std::string, std::string> > qui simule une map.
 * Elle contient egalement des methodes pour chercher des attributs a l'interieur du vector.
 */
/*! \var Attributs::sesAtt
 *	Ce vector contient des paires qui sont composees du nom de l'attribut comme cle, et la valeur de l'attibut en valeur.
 *	\brief vector contenant tous les attributs d'une balise.
 */
/*! \fn void Attributs::insertAtt(std::pair<std::string, std::string> p)
 *  \brief Function permettant l'insertion dans le vector qui est prive.
 *  \param p une paire de string representant la cle et sa valeur.
 */
/*! \fn void Attributs::affiche()
 *  \brief Function permettant l'affichage des attributs contenus dans le vector.
 */
/*! \fn bool Attributs::verifInsert(std::pair<std::string, std::string> p)
 *  \brief Function permettant de verifier si la cle n'existe pas deja dans le vector, cle unique comme dans la map.
 *  \param p une paire de string representant la cle et sa valeur.
 *  \return retourne un booleen, true si la cle n'existe pas, faux si la cle existe.
 */
/*! \fn std::vector<std::pair<std::string, std::string> > Attributs::getAtt()
 *  \brief Function permettant de recuperer le vector qui est prive.
 *  \return retourne un std::vector<std::pair<std::string, std::string> > qui contient tous les parametres.
 */
/*! \fn std::string Attributs::getValueOf(std::string s);
 *  \brief Function permettant de verifier si la cle n'existe pas deja dans le vector, cle unique comme dans la map.
 *  \param s une chaine de characteres qui sera cherchee comme cle.
 *  \return retourne une chaine de characteres qui sera la valeur de la cle passee en parametre, "none" si la fonction ne trouve pas.
 */

#endif /* ATTRIBUTS_H */
