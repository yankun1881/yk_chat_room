#include"userCtr.h"
#include<sstream>
static yk::Logger::ptr g_logger = YK_LOG_ROOT();
namespace chat{

void getdate(sql::ResultSet* res,User& t){
        t.setId(res->getInt("id")); 
        t.setName(res->getString("name")); 
        t.setStatus(res->getInt("status"));
        t.setPassword(res->getString("password"));
        t.setImg(res->getString("img"));
        t.setDetail(res->getString("detail"));
        t.setLastMsg(res->getString("lastMsg"));
        t.setHeadImg(res->getString("headImg"));
}

bool UserController::CreateUsers(const std::string& name,const std::string& password){
    if(getUsersByname(name) != -1){
        return false;
    }
    std::stringstream ss;
    ss <<"INSERT INTO user (name, status, password) VALUES ('" << name << "', 1, '"<<password<<"');";
    yk::ConnPoolMgr::GetInstance()->getConn()->addData(ss.str());
    return true;
}



void UserController::getFriendList(std::vector<User>& vt,int id){
    std::stringstream ss;
    ss << "SELECT * FROM user u WHERE u.id IN ( select friend_id  from friend ";
    ss << " where friend.user_id = " + std::to_string(id);
    ss << " );";

    YK_LOG_INFO(g_logger) << "sql : " << ss.str();
    yk::ConnPoolMgr::GetInstance()->getConn()->processQueryResult<chat::User>(vt,ss.str(),getdate);
}


}
