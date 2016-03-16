/*
 * MD5.h
 *
 *  Created on: Mar 10, 2016
 *      Author: debian
 */

#ifndef MD5_H_
#define MD5_H_

#include <string>
#include <vector>

class MD5 {
public:
    MD5();
    virtual ~MD5();

    std::string compute(const std::string &string) const;

    std::string convertToBitsString(const std::string &string) const; /// convert each characters to ASCII values.

    std::string convertIntToBitRepresentation(int x) const;
private:
    static const int INIT_A;
    static const int INIT_B;
    static const int INIT_C;
    static const int INIT_D;

    static std::vector<int> makeShiftAmts();
    static std::vector<int> makeTableT();

    std::vector<int> m_shiftAmountsPerRound;
    std::vector<int> m_sinusOfInteger;


};

#endif /* MD5_H_ */
