#include"chatCtr.h"

#include "db/users/usersCtr.h"
#include "db/messages/messagesCtr.h"
namespace chat{
static yk::Logger::ptr g_logger = YK_LOG_ROOT();

bool ChatCtr::session_exists(const std::string& id) {
    YK_LOG_INFO(g_logger) << "session_exists id=" << id;
    yk::RWMutex::ReadLock lock(m_mutex);
    auto it = m_sessions.find(id);
    return it != m_sessions.end();
}

void ChatCtr::session_add(const std::string& id, yk::http::WSSession::ptr session) {
    YK_LOG_INFO(g_logger) << "session_add id=" << id;
    yk::RWMutex::WriteLock lock(m_mutex);
    m_sessions[id] = session;
}

void ChatCtr::session_del(const std::string& id) {
    YK_LOG_INFO(g_logger) << "session_add del=" << id;
    yk::RWMutex::WriteLock lock(m_mutex);
    m_sessions.erase(id);
}

int32_t ChatCtr::SendMessage(yk::http::WSSession::ptr session
                    , ChatMessage::ptr msg) {
    YK_LOG_INFO(g_logger) << msg->toString() << " - " << session;
    return session->sendMessage(msg->toString()) > 0 ? 0: 1;
}

void ChatCtr::session_notify(ChatMessage::ptr msg, yk::http::WSSession::ptr session = nullptr) {
    yk::RWMutex::ReadLock lock(m_mutex);
    auto sessions = m_sessions;
    lock.unlock();

    for(auto& i : sessions) {
        if(i.second == session) {
            continue;
        }
        SendMessage(i.second, msg);
    }
}

int32_t ChatCtr::login_request(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session,ChatMessage::ptr msg)
{
    ChatMessage::ptr rsp(new ChatMessage);
    auto id = header->getHeader("$id");

    rsp->set("type", "login_response");
    auto name = msg->get("name");
    if(name.empty()) {
        rsp->set("result", "400");
        rsp->set("msg", "name is null");
        return SendMessage(session, rsp);
    }
    if(!id.empty()) {
        rsp->set("result", "401");
        rsp->set("msg", "logined");
        return SendMessage(session, rsp);
    }
    if(session_exists(id)) {
        rsp->set("result", "402");
        rsp->set("msg", "name exists");
        return SendMessage(session, rsp);
    }
    id = name;
    header->setHeader("$id", id);
    rsp->set("result", "200");
    rsp->set("msg", "ok");
    session_add(id, session);

    ChatMessage::ptr nty(new ChatMessage);
    nty->set("type", "user_enter");
    nty->set("time", yk::TimeToStr());
    nty->set("name", name);
    nty->set("number",std::to_string(m_sessions.size()));        

    session_notify(nty, session);
    return SendMessage(session, rsp);
}

int32_t ChatCtr::send_request(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session,ChatMessage::ptr msg){
    auto id = header->getHeader("$id");
    ChatMessage::ptr rsp(new ChatMessage);
    rsp->set("type", "send_response");
    auto m = msg->get("msg");
    if(m.empty()) {
        rsp->set("result", "500");
        rsp->set("msg", "msg is null");
        return SendMessage(session, rsp);
    }
    if(id.empty()) {
        rsp->set("result", "501");
        rsp->set("msg", "not login");
        return SendMessage(session, rsp);
    }

    rsp->set("result", "200");
    rsp->set("msg", "ok");

    ChatMessage::ptr nty(new ChatMessage);
    nty->set("type", "msg");
    nty->set("time", yk::TimeToStr());
    nty->set("name", id);
    nty->set("msg", m);
    nty->set("id",std::to_string(++lid));
    MessagesController::addMessage(id,m);
    session_notify(nty, nullptr);
    return SendMessage(session, rsp);
}
int32_t ChatCtr::login(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session,ChatMessage::ptr msg){
    auto id = header->getHeader("$id");
    chat::UserController u;
    ChatMessage::ptr rsp(new ChatMessage);
    if(u.NameByPassword(msg->get("name"),msg->get("password"))){
        header->setHeader("$id", id);
        rsp->set("result", "200");
        rsp->set("msg", "ok");
    }else{
        header->setHeader("$id", id);
        rsp->set("result", "201");
        rsp->set("msg", "error");
    }
    return SendMessage(session, rsp);                            
}
int32_t ChatCtr::registe(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session,ChatMessage::ptr msg){
    auto id = header->getHeader("$id");
    ChatMessage::ptr rsp(new ChatMessage);
    if(UserController::createUsers(msg->get("name"),msg->get("password"))){
        header->setHeader("$id", id);
        rsp->set("result", "200");
        rsp->set("msg", "ok");
    }else{
        header->setHeader("$id", id);
        rsp->set("result", "201");
        rsp->set("msg", "昵称已被注册");
    }
    return SendMessage(session, rsp);                            
}
int32_t ChatCtr::message_request(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session,ChatMessage::ptr msg){
    auto id = header->getHeader("$id");
    ChatMessage::ptr rsp(new ChatMessage);
    std::vector<Message>vt;
    MessagesController::getMessageQuery(vt,"",msg->get("day"));
    rsp->set("type", "messageData");
    rsp->set("day", msg->get("day"));
    std::vector<std::map<std::string, std::string>> vm(vt.size());
    for(size_t i = 0 ; i < vt.size(); ++i){
        vm[i]["msg"] = vt[i].getMessage();
        vm[i]["send_time"] = vt[i].getSendTime();
        vm[i]["name"] = vt[i].getUsername();
    }
    rsp->addDataArray(vm);
    return SendMessage(session, rsp);                            
}
int32_t ChatCtr::user_leave(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session,ChatMessage::ptr msg){
    auto id = header->getHeader("$id");
    ChatMessage::ptr rsp(new ChatMessage);
    if(!id.empty()) {
        session_del(id);
        ChatMessage::ptr nty(new ChatMessage);
        nty->set("type", "user_leave");
        nty->set("number", std::to_string(m_sessions.size()));
        nty->set("time", yk::TimeToStr());
        nty->set("name", id);
        session_notify(nty);
    }
    return 0;
}

int32_t ChatCtr::erase(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session,ChatMessage::ptr msg){
    auto id = header->getHeader("$id");
    if(!id.empty()) {
        yk::RWMutex::WriteLock lock(m_mutex);
        m_sessions.erase(id);
    }
    return 1;
}

};