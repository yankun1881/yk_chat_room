#ifndef __CHAT_PROTOCOL_H__
#define __CHAT_PROTOCOL_H__

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <jsoncpp/json/json.h>


namespace chat {

class ChatMessage {
public:
    typedef std::shared_ptr<ChatMessage> ptr;

    static ChatMessage::ptr Create(const std::string& v);
    ChatMessage();
    std::string get(const std::string& name);
    void set(const std::string& name, const std::string& val);
    std::string toString() const;
    void addDataArray(const std::vector<std::map<std::string, std::string>>& dataArray,const std::string & data = "data");
private:
    std::map<std::string, Json::Value> m_datas;
};

}

#endif
