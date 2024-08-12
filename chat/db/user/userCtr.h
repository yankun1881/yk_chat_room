#include "user.h"
#include <yk.h>

#include <vector>
namespace yk{
    void getdata(sql::ResultSet* res,chat::User& T);
}

namespace chat
{
class UserController {
public:

    static int CreateUser(const std::string & name);
    //通过用户id获取用户好友信息
    static void getFriendList(std::vector<User>& vt,int id);
    
};
} // namespace chat


