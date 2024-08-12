#include "friend.h"
#include <yk.h>

#include <vector>
namespace yk{
    void getdata(sql::ResultSet* res,chat::Friend& T);
}

namespace chat
{
class FriendController {
public:


    void  getFriendMsgList(std::vector<Friend>& vt,int id);
    
};
} // namespace chat


