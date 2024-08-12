#include <string>
#include <memory>
namespace chat{

class FriendMessage  {
public:
    typedef std::shared_ptr<FriendMessage> ptr;
    FriendMessage(){}

    void setMessageId(int msgid){m_message_id = msgid;}
    void setChatRoomId(int chatroomid){m_chat_room_id = chatroomid;}
    void setSenderId(int senderId){m_sender_id = senderId;}
    
    void setMessage(std::string msg){m_time = m_msg;}
    void setChatType(int cty){m_chat_type = cty;}
    void setTime(std::string  time){m_time = time;}
    
private:
    
    int m_message_id;
    int m_chat_room_id;
    int m_sender_id;    //发送人id
    int m_chat_type;
    std::string m_msg;
    std::string m_time;
};


}