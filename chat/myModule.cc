#include <yk.h>
#include "chat_servlet.h"
#include "resource_servlet.h"
#include "myModule.h"
#include "chatCtr.h"
namespace chat {
    

// 定义日志记录器
static yk::Logger::ptr g_logger = YK_LOG_ROOT();

// MyModule 构造函数
MyModule::MyModule()
    :Module("chat_room", "1.0", "") {
}

// 模块加载时执行的函数
bool MyModule::onLoad() {
    YK_LOG_INFO(g_logger) << "onLoad";
    return true;
}

// 模块卸载时执行的函数
bool MyModule::onUnload() {
    YK_LOG_INFO(g_logger) << "onUnload";
    return true;
}

void MyModule::readfile(const std::string& uri,const std::string& path,const std::string& suffix){
    yk::FSUtil::ListAllFile(files, path, suffix);
    size_t usize = uris.size();
    size_t fsize = files.size();
    uris.resize(fsize);
    for(size_t i = usize; i < fsize; ++i){
        uris[i] = uri;
        std::string s;
        for(size_t k = files[i].size()-1; k != 0; --k){
            if(files[i][k] == '/'){
                s = std::string(files[i].begin()+k,files[i].end());
                break;
            }
        }
        uris[i] += s;
    }
    
}

// 服务器准备就绪时执行的函数
bool MyModule::onServerReady() {
    YK_LOG_INFO(g_logger) << "onServerReady";

    // 获取 HTTP 服务器
    std::vector<yk::TcpServer::ptr> svrs;
    if(!yk::Application::GetInstance()->getServer("http", svrs)) {
        YK_LOG_INFO(g_logger) << "no httpserver alive";
        return false;
    }
    //添加html页面下面的
    std::string path = yk::EnvMgr::GetInstance()->getCwd();
    path+="html/";
    readfile("/html",path,".html");
    readfile("/html/css",path+"css/",".css");
    readfile("/html/js",path+"js/",".js");
    path = yk::EnvMgr::GetInstance()->getCwd();
    path+="dist/";
    yk::FSUtil::ListAllFile(files, path,".html");
    uris.push_back("/");
    readfile("",path,".html");
    readfile("",path,".png");
    readfile("",path,".svg");
    readfile("/assets",path+"assets/",".css");
    readfile("/assets",path+"assets/",".js");
    
    std::vector<yk::http::ResourceServlet::ptr> slts(files.size());
    for(size_t i = 0; i < files.size();++i){
        slts[i].reset(new yk::http::ResourceServlet(files[i]));
    }
    // 遍历 HTTP 服务器，为其添加资源处理 Servlet
    for(auto& i : svrs) {
        yk::http::HttpServer::ptr http_server =
            std::dynamic_pointer_cast<yk::http::HttpServer>(i);
        if(!i) {
            continue;
        }
        auto slt_dispatch = http_server->getServletDispatch();
        // 创建资源处理 Servlet 并添加到 ServletDispatch 中
        for(size_t j = 0; j < files.size(); ++j) {
            slt_dispatch->addServlet(uris[j], slts[j]);
            YK_LOG_INFO(g_logger) << "addServlet: " << uris[j];
        }
        
    }

    // 清空服务器列表
    svrs.clear();

    // 获取 WebSocket 服务器
    if(!yk::Application::GetInstance()->getServer("ws", svrs)) {
        YK_LOG_INFO(g_logger) << "no ws alive";
        return false;
    }

    // 遍历 WebSocket 服务器，为其添加聊天室 WebSocket Servlet
    for(auto& i : svrs) {
        yk::http::WSServer::ptr ws_server =
            std::dynamic_pointer_cast<yk::http::WSServer>(i);

        yk::http::ServletDispatch::ptr slt_dispatch = ws_server->getWSServletDispatch();
        ChatWSServlet::ptr slt(new ChatWSServlet);
        slt_dispatch->addServlet("/yk/chat", slt);
    }
    
    chatCtr::GetInstance()->sercive["login_request"] = [](yk::http::HttpRequest::ptr header, yk::http::WSSession::ptr session, chat::ChatMessage::ptr message) {
        return chatCtr::GetInstance()->login_request(header, session, message);
    };
    chatCtr::GetInstance()->sercive["send_request"] = [](yk::http::HttpRequest::ptr header, yk::http::WSSession::ptr session, chat::ChatMessage::ptr message) {
        return chatCtr::GetInstance()->send_request(header, session, message);
    };

    chatCtr::GetInstance()->sercive["login"] = [](yk::http::HttpRequest::ptr header, yk::http::WSSession::ptr session, chat::ChatMessage::ptr message) {
        return chatCtr::GetInstance()->login(header, session, message);
    };

    chatCtr::GetInstance()->sercive["register"] = [](yk::http::HttpRequest::ptr header, yk::http::WSSession::ptr session, chat::ChatMessage::ptr message) {
        return chatCtr::GetInstance()->registe(header, session, message);
    };

    chatCtr::GetInstance()->sercive["message_request"] = [](yk::http::HttpRequest::ptr header, yk::http::WSSession::ptr session, chat::ChatMessage::ptr message) {
        return chatCtr::GetInstance()->message_request(header, session, message);
    };

    chatCtr::GetInstance()->sercive["user_leave"] = [](yk::http::HttpRequest::ptr header, yk::http::WSSession::ptr session, chat::ChatMessage::ptr message) {
        return chatCtr::GetInstance()->user_leave(header, session, message);
    };
    chatCtr::GetInstance()->sercive["erase"] = [](yk::http::HttpRequest::ptr header, yk::http::WSSession::ptr session, chat::ChatMessage::ptr message) {
        return chatCtr::GetInstance()->erase(header, session, message);
    };
    
    return true;
}

// 服务器启动完成时执行的函数
bool MyModule::onServerUp() {
    YK_LOG_INFO(g_logger) << "onServerUp";
    return true;
}

bool MyRockModule::handleRockRequest(yk::RockRequest::ptr request,
                                yk::RockResponse::ptr response,
                                yk::RockStream::ptr stream)  {
    // 处理请求的逻辑

    std::string s = request->getBody();
    size_t pos = s.find(' ');
    std::string name(s.begin(),s.begin()+pos);
    std::string message(s.begin()+pos+1,s.end());
    chatCtr::GetInstance()->send_request(name,message);

    response->setCmd(100);
    response->setResult(1);
    response->setResultStr("yes");
    return true; // 返回处理结果
}

bool MyRockModule::handleRockNotify(yk::RockNotify::ptr notify,
                                yk::RockStream::ptr stream)  {
    // 处理通知的逻辑


    return true; // 返回处理结果
}




} // namespace chat

// 使用 extern "C" 声明 CreateModule 和 DestroyModule 函数，使其能够在动态链接时被正确调用
extern "C" {

// 创建模块实例的函数
yk::Module* CreateModule() {
    yk::Module* module = new chat::MyModule;
    YK_LOG_INFO(chat::g_logger) << "CreateModule " << module;
    return module;
}

// 销毁模块实例的函数
void DestroyModule(yk::Module* module) {
    YK_LOG_INFO(chat::g_logger) << "DestroyModule " << module;
    delete module;
}

} // extern "C"
