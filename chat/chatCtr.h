#ifndef __CHAT_CHAT_CTR_H__
#define __CHAT_CHAT_CTR_H__

#include <yk.h>

#include "protocol.h"
#include <map>
namespace chat {

class ChatCtr{
public:
    typedef std::shared_ptr<ChatCtr> ptr;
    ChatCtr(){}
    void session_del(const std::string& id) ;
    bool session_exists(const std::string& id) ;
    void session_add(const std::string& id, yk::http::WSSession::ptr session);
    int32_t SendMessage(yk::http::WSSession::ptr session
                , ChatMessage::ptr msg);
    void session_notify(ChatMessage::ptr msg, yk::http::WSSession::ptr session) ;                
    int32_t login_request(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg);
    int32_t send_request(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg);
    int32_t login(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg);
    int32_t registe(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg);
    int32_t message_request(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg);
    int32_t user_leave(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg);    
    int32_t erase(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg);
    int32_t send_request(std::string name,std::string msg);
public:
    std::map<std::string,std::function<int32_t(yk::http::HttpRequest::ptr header
                            ,yk::http::WSSession::ptr session,ChatMessage::ptr msg)>> sercive;
    yk::RWMutex m_mutex;
    std::map<std::string, yk::http::WSSession::ptr> m_sessions;
    std::atomic<long long> lid;
};

typedef yk::Singleton<ChatCtr> chatCtr;
}

#endif
