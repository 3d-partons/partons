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
 * @brief Time format utilities used by database services.
 *
 * Utilities dealing with time formats used by database services. In particular tools to retrieve current time in SQL format are availible.
 */

#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <string>

class DateTimeUtils {

public:

    /**
     * Get current time in format recognizable by SQL.
     * @return String containing time in "yyyy-MM-dd hh:mm:ss" format.
     */
    static std::string getCurrentSQLDateTime() {
        QDateTime dateTime = QDateTime::currentDateTime();
        return dateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString();
    }
};

#endif /* DATE_TIME_UTILS_H */
