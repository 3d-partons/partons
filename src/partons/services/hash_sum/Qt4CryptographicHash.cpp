#include "../../../../include/partons/services/hash_sum/Qt4CryptographicHash.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qcryptographichash.h>
#include <QtCore/qstring.h>

namespace PARTONS {


Qt4CryptographicHash::Qt4CryptographicHash() :
        CryptographicHashI("Qt4CryptographicHash") {
}

Qt4CryptographicHash::~Qt4CryptographicHash() {
    // TODO Auto-generated destructor stub
}

std::string Qt4CryptographicHash::generateSHA1HashSum(
        const std::string& string) const {
    QByteArray hashSum = QCryptographicHash::hash(QByteArray(string.c_str()),
            QCryptographicHash::Sha1);

    return QString(hashSum.toHex()).toStdString();
}

} /* namespace PARTONS */
