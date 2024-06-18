#include"messagesCtr.h"
#include<sstream>
static yk::Logger::ptr g_logger = YK_LOG_ROOT();
namespace chat{

void getdate(sql::ResultSet* res,Message& t){
    t.setUsername(res->getString("username")); 
    t.setMessage(res->getString("message"));
    t.setSendTime(res->getString("send_time"));  
}

void MessagesController::getMessageQuery(std::vector<Message>& dates,const std::string & name,const std::string & day){
    std::stringstream ss;
    ss << "select * from messages where 1=1 "; 
    if(name.size() != 0){
        ss << "and name = '" << name <<"'";
    }
    if(day.size() != 0){
        ss <<" and send_time > NOW() - INTERVAL "<< day << "  DAY";
    }
    ss << ";";
    YK_LOG_INFO(g_logger) << "sql : " << ss.str();
    yk::ConnPoolMgr::GetInstance()->getConn()->processQueryResult<chat::Message>(dates,ss.str(),getdate);
}

void MessagesController::addMessage(const std::string & name,const std::string & message,const std::string & Time){
    std::stringstream ss;
    ss << "INSERT INTO messages (username, message, send_time) VALUES ("; 
    ss << " '" << name <<"' ,'" <<message<<"'";
    if(Time.size() != 0){
        ss << ",'"<<Time<<"'";
    }else{
        ss << ", NOW()";
    }
    ss << ");";
    YK_LOG_INFO(g_logger) << "sql : " << ss.str();
    yk::ConnPoolMgr::GetInstance()->getConn()->addData(ss.str());
}


}
