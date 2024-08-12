#include "users.h"
#include <yk.h>

#include <vector>
namespace yk{
    void getdata(sql::ResultSet* res,chat::Users& T);
}

namespace chat
{
class UserController {
public:
    //根据name，或者状态查询Users
    static void getUsersQuery(std::vector<Users>& vt,const std::string& name = "",int m_status = -1);
    //验证name和password是否正确
    static bool NameByPassword(const std::string& name,const std::string& password);
    //通过name获取UsersId
    static int getUsersByname(const std::string& name);
    //创建用户
    static bool createUsers(const std::string& name,const std::string& password);
};
} // namespace chat


