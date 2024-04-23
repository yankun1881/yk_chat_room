#include <yk.h>
#include "chat_servlet.h"
#include "resource_servlet.h"
#include "myModule.h"

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

// 服务器准备就绪时执行的函数
bool MyModule::onServerReady() {
    YK_LOG_INFO(g_logger) << "onServerReady";

    // 获取 HTTP 服务器
    std::vector<yk::TcpServer::ptr> svrs;
    if(!yk::Application::GetInstance()->getServer("http", svrs)) {
        YK_LOG_INFO(g_logger) << "no httpserver alive";
        return false;
    }
    std::string path = yk::EnvMgr::GetInstance()->getCwd();
    path+="html/";
    std::vector<std::string> files;
    yk::FSUtil::ListAllFile(files, path, ".html");
    size_t fsize = files.size();
    std::vector<std::string> uri;
    uri.resize(files.size());
    for(size_t i = 0; i < fsize; ++i){
        uri[i] = "/html";
        std::string s;
        for(size_t k = files[i].size()-1; k != 0; --k){
            if(files[i][k] == '/'){
                s = std::string(files[i].begin()+k,files[i].end());
                break;
            }
        }
        uri[i] += s;
    }
    yk::FSUtil::ListAllFile(files, path+"css/", ".css");
    uri.resize(files.size());
    for(size_t i = fsize; i < files.size(); ++i){
        uri[i] = "/html/css";
        std::string s;
        for(size_t k = files[i].size()-1; k != 0; --k){
            if(files[i][k] == '/'){
                s = std::string(files[i].begin()+k,files[i].end());
                break;
            }
        }
        uri[i] += s;
    }
    fsize = files.size();
    yk::FSUtil::ListAllFile(files, path+"js/", ".js");
    for(size_t i = fsize; i < files.size(); ++i){
        uri[i] = "/html/js";
        std::string s;
        for(size_t k = files[i].size()-1; k != 0; --k){
            if(files[i][k] == '/'){
                s = std::string(files[i].begin()+k,files[i].end());
                break;
            }
        }
        uri[i] += s;
    }
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
            slt_dispatch->addServlet(uri[j], slts[j]);
            YK_LOG_INFO(g_logger) << "addServlet";
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
    return true;
}

// 服务器启动完成时执行的函数
bool MyModule::onServerUp() {
    YK_LOG_INFO(g_logger) << "onServerUp";
    return true;
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
