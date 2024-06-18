#include "messages.h"
#include <yk.h>

#include <vector>
namespace yk{
    void getdata(sql::ResultSet* res,chat::Message& T);
}

namespace chat
{
class MessagesController {
public:

    //获取day天的消息
    static void getMessageQuery(std::vector<Message>& vt,const std::string & name ="",const std::string & day = "1");
    static void addMessage(const std::string & name,const std::string & message,const std::string & Time = "");
};
} // namespace chat


