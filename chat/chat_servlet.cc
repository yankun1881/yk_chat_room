#include "chat_servlet.h"
#include "chatCtr.h"
#include "yk/log.h"
#include "protocol.h"
#include "db/users/usersCtr.h"
#include "db/messages/messagesCtr.h"
namespace chat {

static yk::Logger::ptr g_logger = YK_LOG_ROOT();


ChatWSServlet::ChatWSServlet()
    :yk::http::WSServlet("chat_servlet") {
}

int32_t ChatWSServlet::onConnect(yk::http::HttpRequest::ptr header
                              ,yk::http::WSSession::ptr session) {
    YK_LOG_INFO(g_logger) << "onConnect ";
    return 0;
}

int32_t ChatWSServlet::onClose(yk::http::HttpRequest::ptr header
                             ,yk::http::WSSession::ptr session) {
    auto id = header->getHeader("$id");
    YK_LOG_INFO(g_logger) << "onClose "<< " id=" << id;
    if(!id.empty()) {
        return chatCtr::GetInstance()->sercive["user_leave"](header,session,nullptr);
    }
    return 0;
}

int32_t ChatWSServlet::handle(yk::http::HttpRequest::ptr header
                           ,yk::http::WSFrameMessage::ptr msgx
                           ,yk::http::WSSession::ptr session) {
    YK_LOG_INFO(g_logger) << "handle " << session
            << " opcode=" << msgx->getOpcode()
            << " data=" << msgx->getData();

    auto msg = ChatMessage::Create(msgx->getData());
    auto id = header->getHeader("$id");
    if(!msg) {
        return chatCtr::GetInstance()->sercive["erase"](header,session,msg);
    }
    auto type = msg->get("type");
    if(type == "login_request") {
        return chatCtr::GetInstance()->sercive["login_request"](header,session,msg);
    } else if(type == "send_request") {
        return chatCtr::GetInstance()->sercive["send_request"](header,session,msg);
    }else if(type == "login"){
        return chatCtr::GetInstance()->sercive["login"](header,session,msg);
    }else if(type == "register"){
        return chatCtr::GetInstance()->sercive["register"](header,session,msg);
    }else if(type == "message_request"){
        return chatCtr::GetInstance()->sercive["message_request"](header,session,msg);
    }
    return 0;
}

}
