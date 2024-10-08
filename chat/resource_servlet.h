#ifndef __YK_HTTP_RESOURCE_SERVLET_H__
#define __YK_HTTP_RESOURCE_SERVLET_H__

#include <yk.h>

namespace yk {
namespace http {

class ResourceServlet :public yk::http::Servlet {
public:
    typedef std::shared_ptr<ResourceServlet> ptr;
    ResourceServlet(const std::string& path);
    virtual int32_t handle(yk::http::HttpRequest::ptr request
                   , yk::http::HttpResponse::ptr response
                   , yk::http::HttpSession::ptr session) override;

private:
    std::string m_path;
};

class getFriendListServlet :public yk::http::Servlet {
public:
    typedef std::shared_ptr<getFriendListServlet> ptr;
    getFriendListServlet();
    virtual int32_t handle(yk::http::HttpRequest::ptr request
                   , yk::http::HttpResponse::ptr response
                   , yk::http::HttpSession::ptr session) override;

};

class getFriendMsgServlet :public yk::http::Servlet {
public:
    typedef std::shared_ptr<getFriendMsgServlet> ptr;
    getFriendMsgServlet();
    virtual int32_t handle(yk::http::HttpRequest::ptr request
                   , yk::http::HttpResponse::ptr response
                   , yk::http::HttpSession::ptr session) override;

};

}
}

#endif
