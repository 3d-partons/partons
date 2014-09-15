#include <string>

class BaseObject {
public:
    BaseObject(std::string className);
    ~BaseObject();
    const std::string getClassName();

private:
    std::string m_className;
};
