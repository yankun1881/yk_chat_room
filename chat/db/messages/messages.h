#include <string>
#include <memory>
namespace chat{
class Message {
private:
    std::string username;
    std::string message;
    std::string send_time; // 使用字符串存储日期时间，实际应用中可能需要使用专门的日期时间库

public:
    // 构造函数
    Message(){
    }
    Message( const std::string& username, const std::string& message, const std::string& send_time)
        : username(username), message(message), send_time(send_time) {}
    const std::string& getUsername() const { return username; }
    void setUsername(const std::string& newUser) { username = newUser; }

    const std::string& getMessage() const { return message; }
    void setMessage(const std::string& newMessage) { message = newMessage; }

    const std::string& getSendTime() const { return send_time; }
    void setSendTime(const std::string& newSendTime) { send_time = newSendTime; }
};

}