#include "resource_servlet.h"
#include <yk.h>
#include <iostream>
#include <fstream>
#include "db/user/userCtr.h"
#include "db/chat_message/chat_messageCtr.h"

#include "protocol.h"
namespace yk {
namespace http {

static yk::Logger::ptr g_logger = YK_LOG_ROOT();

ResourceServlet::ResourceServlet(const std::string& path)
    :Servlet("ResourceServlet")
    ,m_path(path) {
}

int32_t ResourceServlet::handle(yk::http::HttpRequest::ptr request
                           , yk::http::HttpResponse::ptr response
                           , yk::http::HttpSession::ptr session) {
    std::string path;
    if(m_path[m_path.size()-1] == '/'){
        path = std::string(m_path.begin(),m_path.begin()+m_path.size()-1);
    }else{
        path = m_path;
    }
    YK_LOG_INFO(g_logger) << "handle path=" << path;
    if(path.find("..") != std::string::npos) {
        response->setBody("invalid path");
        response->setStatus(yk::http::HttpStatus::NOT_FOUND);
        return 0;
    } 
    std::ifstream ifs(path);
    if(!ifs) {
        response->setBody("invalid file");
        response->setStatus(yk::http::HttpStatus::NOT_FOUND);
        return 0;
    }

    std::stringstream ss;
    std::string line;
    while(std::getline(ifs, line)) {
        ss << line << std::endl;
    }
    ifs.close();
    response->setBody(ss.str());
    std::string s;
    for(size_t k = path.size()-1; k != 0; --k){
        if(path[k] == '.'){
            s = std::string(path.begin()+k+1,path.end());
            break;
        }
    }
    if(s == "js"){
        response->setHeader("content-type", "text/javascript;charset=utf-8");
    }else if(s == "png"){
        response->setHeader("content-type", "image/png");
    }else if(s == "svg"){
        response->setHeader("content-type", "image/svg+xml");
    }else{
        response->setHeader("content-type", "text/"+s+";charset=utf-8");
    }
    response->setHeader("X-Content-Type-Options", "nosniff");
    response->setHeader("Cache-Control", "public, max-age=3600");

    return 0;
}
getFriendListServlet::getFriendListServlet():
    Servlet("getFriendListServlet"){

}
int32_t getFriendListServlet::handle(yk::http::HttpRequest::ptr request
                   , yk::http::HttpResponse::ptr response
                   , yk::http::HttpSession::ptr session) {
        
        // 解析请求体
        auto msg = request->getHeader("id");
        // TODO: 根据你的应用逻辑解析requestBody
        if(msg == ""){
            response->setStatus(yk::http::HttpStatus::OK);
            response->setHeader("content-type", "application/json");
            response->setHeader("id","1");
            response->setBody("{参数错误}");

            // 发送响应
            session->sendResponse(response);
            return -2;
        }
        
        int id =  std::stoi(msg);
        
        std::vector<chat::User> vt;
        chat::UserController::getFriendList(vt,id);

        std::vector<std::map<std::string, std::string>> vm(vt.size());
        for(size_t i = 0 ; i < vt.size(); ++i){
            vm[i]["img"] = vt[i].getImg();
            vm[i]["name"] = vt[i].getName();
            vm[i]["detail"] = vt[i].getDetail();
            vm[i]["lastMsg"] = vt[i].getLastMsg();
            vm[i]["id"] = std::to_string(vt[i].getId());
        }
        
        auto js =  chat::ChatMessage::toDataArray(vm);
        
        std::cout << js <<std::endl;
        // 设置响应状态码和响应体
        response->setStatus(yk::http::HttpStatus::OK);
        response->setHeader("content-type", "application/json");
        
        response->setHeader("Access-Control-Allow-Origin", "*");
        response->setBody(js);

        // 发送响应
        session->sendResponse(response);

        return 0; // 成功处理请求
        
        
        


}

getFriendMsgServlet::getFriendMsgServlet():
    Servlet("getFriendMsgServlet"){

}
int32_t getFriendMsgServlet::handle(yk::http::HttpRequest::ptr request
                   , yk::http::HttpResponse::ptr response
                   , yk::http::HttpSession::ptr session) {
        // 解析请求体
        auto msg = request->getHeader("id");
        auto msg2 = request->getHeader("fid");
        // TODO: 根据你的应用逻辑解析requestBody
        
        if(msg == ""){
            response->setStatus(yk::http::HttpStatus::OK);
            response->setHeader("content-type", "application/json");
            response->setHeader("id","1");
            response->setBody("{参数错误}");

            // 发送响应
            session->sendResponse(response);
            return -2;
        }
        
        int id =  std::stoi(msg);
        int fid =  std::stoi(msg2);
        std::vector<std::map<std::string, std::string>> vm;
        chat::ChatMessageController::getChatMessageMsgList(vm,id,fid);
        
        auto js =  chat::ChatMessage::toDataArray(vm);
        
        std::cout << js <<std::endl;
        // 设置响应状态码和响应体
        response->setStatus(yk::http::HttpStatus::OK);
        response->setHeader("content-type", "application/json");
        
        response->setHeader("Access-Control-Allow-Origin", "*");
        response->setBody(js);

        // 发送响应
        session->sendResponse(response);

        return 0; // 成功处理请求
        


}







}
}
