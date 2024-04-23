#include <string>
#include <memory>
namespace chat{

class Users  {
public:
    typedef std::shared_ptr<Users> ptr;
    Users(){}
    Users(std::string _name, int _status, std::string _password) : m_name(_name), m_status(_status), m_password(_password) {}

    int getId() const { return m_id; }
    
    std::string getName() const { return m_name; }
    int getStatus() const { return m_status; }
    std::string getPassword() const { return m_password; }
    void setId(int id) { m_id = id; }
    void setName(std::string name)  {  m_name = name; }
    void setStatus(int _status) { m_status = _status; }
    void setPassword(std::string _password) { m_password = _password; }
private:
    int m_id;
    std::string m_name;
    int m_status;
    std::string m_password;
};


}