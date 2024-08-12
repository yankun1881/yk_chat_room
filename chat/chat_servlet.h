#ifndef __CHAT_CHAT_SERVLET_H__
#define __CHAT_CHAT_SERVLET_H__

#include <yk.h>
namespace chat {
    
class ChatWSServlet : public yk::http::WSServlet {
public:
    typedef std::shared_ptr<ChatWSServlet> ptr;
    ChatWSServlet();
    virtual int32_t onConnect(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session) override;
    virtual int32_t onClose(yk::http::HttpRequest::ptr header
                             ,yk::http::WSSession::ptr session) override;
    virtual int32_t handle(yk::http::HttpRequest::ptr header
                           ,yk::http::WSFrameMessage::ptr msg
                           ,yk::http::WSSession::ptr session) override;
};


class CatChatWSServlet : public yk::http::WSServlet {
public:
    typedef std::shared_ptr<CatChatWSServlet> ptr;
    CatChatWSServlet();
    virtual int32_t onConnect(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session) override;
    virtual int32_t onClose(yk::http::HttpRequest::ptr header
                             ,yk::http::WSSession::ptr session) override;
    virtual int32_t handle(yk::http::HttpRequest::ptr header
                           ,yk::http::WSFrameMessage::ptr msg
                           ,yk::http::WSSession::ptr session) override;
};

}

#endif
