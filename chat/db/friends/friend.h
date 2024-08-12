#include <string>
#include <memory>
namespace chat{

class Friend  {
public:
    typedef std::shared_ptr<Friend> ptr;
    Friend(){}
    Friend(int id1,int id2, std::string time) : m_user_id(id1), m_friend_id(id2), m_time(time) {}
    void setUserId(int id) { m_user_id = id; }
    void setFriendId(int id) { m_friend_id = id; }

    int getUserId() const { return m_user_id; }
    int getFriendId() const { return m_friend_id; }
    std::string getTime const{return m_time;}
private:
    
    int m_user_id;
    int m_friend_id;
    int m_chatType;
    std::string m_headImg;
    std::string m_msg;
    std::string m_time;
};


}