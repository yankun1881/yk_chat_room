#include "resource_servlet.h"
#include <yk.h>
#include <iostream>
#include <fstream>

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

}
}
