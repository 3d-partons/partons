#ifndef DATE_TIME_UTILS_H
#define DATE_TIME_UTILS_H

/**
 * @file DateTimeUtils.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 May 2015
 * @version 1.0
 *
 * @class DateTimeUtils
 *
 * @brief
 */

#include <QDateTime>
#include <QString>
#include <string>

class DateTimeUtils {

public:
    static std::string getCurrentSQLDateTime() {
        QDateTime dateTime = QDateTime::currentDateTime();
        return dateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString();
    }
};

#endif /* DATE_TIME_UTILS_H */
