#include"chat_messageCtr.h"
#include<sstream>
static yk::Logger::ptr g_logger = YK_LOG_ROOT();
namespace chat{

void getdate(sql::ResultSet* res,std::map<std::string, std::string>& t){
        t["headImg"] = res->getString("headImg"); 
        t["uid"] = res->getString("sender_id"); 
        t["time"] = res->getString("time"); 
        t["chatType"] = res->getString("chatType"); 
        t["name"] = res->getString("name"); 
        t["msg"] = res->getString("msg"); 
}



void ChatMessageController::getChatMessageMsgList(std::vector<std::map<std::string, std::string>>& vt,int uid, int fid){
    std::stringstream ss;
    ss << "SELECT cm.sender_id ,u.headImg, u.name,cm.msg,cm.chatType,cm.time FROM chat_messages cm inner join user u on cm.sender_id =  u.id WHERE cm.chat_room_id IN (SELECT chat_room_id FROM chat_room_members WHERE id IN (";
    ss << uid <<","<<fid;
    ss << ") GROUP BY chat_room_id HAVING COUNT(*) = 2";
    ss << ");";

    YK_LOG_INFO(g_logger) << "sql : " << ss.str();
    yk::ConnPoolMgr::GetInstance()->getConn()->processQueryResult(vt,ss.str(),getdate);
}

}
