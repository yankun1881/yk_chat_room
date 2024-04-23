#include"usersCtr.h"
#include<sstream>
static yk::Logger::ptr g_logger = YK_LOG_ROOT();
namespace chat{

void getdate(sql::ResultSet* res,Users& t){
    t.setId(res->getInt("id")); 
    t.setName(res->getString("name")); 
    t.setStatus(res->getInt("status"));
    t.setPassword(res->getString("password"));  
}
//根据name，或者状态查询Users
void UserController::getUsersQuery(std::vector<Users>& dates,const std::string& name,int m_status){
    std::stringstream ss;
    ss << "select * from users where 1=1 "; 
    if(name.size() != 0){
        ss << "and name = '" << name <<"'";
    }
    if(m_status != -1){
        ss <<"' and status = "<< m_status;
    }
    ss << ";";
    YK_LOG_INFO(g_logger) << "sql : " << ss.str();
    yk::SQLMgr::GetInstance()->processQueryResult<chat::Users>(dates,ss.str(),getdate);
}
//验证name和password是否正确
bool UserController::NameByPassword(const std::string& name,const std::string& password){
    std::vector<Users> uvt;
    getUsersQuery(uvt,name);
    if(uvt.size() == 0) return false;
    return uvt[0].getPassword().compare(password) == 0;    
}
//通过name获取UsersId 没有该用户返回-1
int UserController::getUsersByname(const std::string& name){
    std::vector<Users> uvt;
    getUsersQuery(uvt,name);
    if(uvt.size() > 0){
        return uvt[0].getId();
    }
    else {
        return -1;
    } 
}
//创建账号
bool UserController::createUsers(const std::string& name,const std::string& password){
    if(getUsersByname(name) != -1){
        return false;
    }
    std::stringstream ss;
    ss <<"INSERT INTO users (name, status, password) VALUES ('" << name << "', 1, '"<<password<<"');";
    yk::SQLMgr::GetInstance()->addData(ss.str());
    return true;
}


}
