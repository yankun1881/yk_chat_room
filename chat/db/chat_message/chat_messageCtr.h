#include "chat_message.h"
#include <yk.h>

#include <vector>

namespace chat
{
class ChatMessageController {
public:


    static void  getChatMessageMsgList(std::vector<std::map<std::string, std::string>>& vt,int uid,int fid);
    
};
} // namespace chat


