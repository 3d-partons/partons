#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 09 September 2014
 * Last update : 17 September 2014
 */

#include <string>

class LoggerManager;

class BaseObject {
public:
    BaseObject(std::string className);
    ~BaseObject();
    const std::string getClassName();

protected:
    LoggerManager* m_pLoggerManager; ///< Logger's pointer

private:
    std::string m_className; ///< String that represents name of child class used by the logger for know the source of the trace
};

#endif /* BASE_OBJECT_H */
