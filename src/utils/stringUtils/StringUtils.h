#ifndef STRING_UTILS_H
#define STRING_UTILS_H

/*!
 * \file StringUtils.h
 * \brief Manipulations et traitements sur les chaines de caracteres
 * \author Bryan BERTHOU
 * \version 0.1
 * \date Creation : 28/04/2009
 * \date Derniere maj : /09/2014
 */

//#include <map>
#include <string>
#include <utility>
#include <vector>

//TODO uniformiser le fichier, corriger les parametres des fonctions (const, &, ...) refaire la documentation

/*!
 * \class StringUtils
 */
class StringUtils {

public:

    static std::string EMPTY;

    /*!
     * \fn static bool isEmpty(const std::string & str)
     * \brief Test si la chaine de caracteres cible est NULL ou vide ""
     * \param _chaine : chaine de caracteres a tester
     * \return bool
     */
    static bool isEmpty(const std::string & str);

    /*!
     * \fn static bool equals(const std::string & str, const std::string & str2)
     * \brief Applique une comparaison entre la chaine 1 et 2 en tenant compte de la casse
     * \param _chaine1 : chaine de caracteres a comparer
     * \param _chaine2 : chaine de caracteres a comparer
     * \return bool : true si les deux chaines de caracteres sont egales
     */
    static bool equals(const std::string & str, const std::string & str2);

    /*!
     * \fn static bool equalsIgnoreCase( std::string _chaine1, std::string _chaine2, bool toUpperCase )
     * \brief Applique une comparaison entre la chaine 1 et 2 sans tenir compte de la casse
     * \param _chaine1 : chaine de caracteres a comparer
     * \param _chaine2 : chaine de caracteres a comparer
     * \return bool : true si les deux chaines de caracteres sont egales
     */
    static bool equalsIgnoreCase(const std::string &chaine,
            const std::string &chaine2, const bool &toUpperCase = true);

//    /*!
//     * \fn static std::string fromIntToString( int _nombre )
//     * \brief Convertie un nombre en chaine de caracteres
//     * \param _nombre : nombre a convertir en chaine de caracteres
//     * \return std::string
//     */
//    static std::string fromIntToString(int number);

    /*!
     * \fn static int fromStringToInt( std::string _chaine )
     * \brief Convertie une chaine de caracteres en entier
     * \param _chaine : chaine a convertir en entier
     * \return int
     */
    static int fromStringToInt(std::string chaine);

    //static unsigned int fromStringToUnsignedInt(const std::string & chaine);

    static float fromStringToFloat(std::string chaine);

    static double fromStringToDouble(const std::string & str);

    static bool fromStringToBool(std::string chaine);

    static bool contains(const std::string & str,
            const std::string & searchString);

    static bool containsIgnoreCase(std::string* chaine,
            std::string* searchString, bool toUpperCase);

    static void replaceAll(std::string & str, const std::string & searchString,
            const std::string & replaceString);

    static void replaceAllIgnoreCase(std::string* chaine,
            std::string* searchString, std::string* replaceString,
            bool toUpperCase);

    // Remove leading and trailing spaces from a string
    static void trim(std::string &str, const std::string& whitespace = " \t");

    static unsigned int count(std::string str,
            const std::string & searchString);

    /*!
     * \fn static std::vector<std::string> stringToVector( std::string _chaine, char splitCharacter )
     * \brief Transforme une chaine de caracteres en un vector de string
     * \param _chaine : chaine a splitter
     * \param splitCharacter : caractere de split
     * \return std::vector<std::string>
     */
    static std::vector<std::string> split(const std::string &chaine,
            char splitCharacter);

    /*!
     * \fn static std::string vectorToString( std::vector<std::string> vector )
     * \brief Transforme un vector de string en une chaine de caracteres
     * \param vector : vector a transformer en chaine de caracteres
     * \return std::string
     */
    static std::string vectorToString(std::vector<std::string>* vector);

    static void to_upperCase(std::string &chaine);

    static void to_lowerCase(std::string* chaine);

    static std::pair<std::string, std::vector<char> > fromStringToArrayOfChar(
            const std::string &string);

   static std::string removeAfterLast(const std::string &chaine,
            const char lastCharacter);
};

#endif /* STRING_UTILS_H */
