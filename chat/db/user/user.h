#include <string>
#include <memory>
namespace chat{

class User  {
public:
    typedef std::shared_ptr<User> ptr;
    User(){}
    User(std::string _name, std::string _password) : m_name(_name), m_password(_password) {}
    void setId(int id) { m_id = id; }
    void setName(const std::string& name)  {  m_name = name; }
    void setStatus(int _status) { m_status = _status; }
    void setPassword(const std::string& _password) { m_password = _password; }
    void setImg(const std::string& img) { m_img = img; }
    void setDetail(const std::string& detail) { m_detail = detail; }
    void setLastMsg(const std::string& lastMsg){ m_lastMsg = lastMsg;}
    void setHeadImg(const std::string& headImg){ m_headImg = headImg;}

    int getId() const { return m_id; }
    std::string getName() const { return m_name; }
    int getStatus() const { return m_status; }
    std::string getPassword() const { return m_password; }

    std::string getImg() const { return m_img; }
    std::string getDetail() const { return m_detail; }
    std::string getLastMsg() const { return m_lastMsg; }
    std::string getHeadImg() const { return m_headImg; }
    

private:
    int m_id;
    int m_status;
    std::string m_name;
    std::string m_img;
    std::string m_detail;
    std::string m_lastMsg;
    std::string m_headImg;
    std::string m_password;
};


}