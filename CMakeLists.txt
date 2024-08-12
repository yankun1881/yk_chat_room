cmake_minimum_required(VERSION 3.0)

project(chat)
set(CMAKE_VERBOSE_MAKEFILE NO)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-deprecated-declarations")

set(MySQLCppCONN_INCLUDE_DIRS "/usr/include/cppconn")
set(MySQLCppCONN_LIBRARIES "/usr/lib/x86_64-linux-gnu/libmysqlcppconn.so")
include_directories(${MySQLCppCONN_INCLUDE_DIRS})

# hiredis的动态库
set(hiredis "/usr/lib/x86_64-linux-gnu/libhiredis.so")

# 添加redis++的动态库
set(redis++ "/home/ubuntu/redis-plus-plus/build/libredis++.so")

# 添加 redis++ 库的包含目录
include_directories("/home/ubuntu/redis-plus-plus/src")

# zookeeper
set(zookeeper "/usr/lib/x86_64-linux-gnu/libzookeeper_mt.so")

find_package(yaml-cpp REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(OpenSSL REQUIRED)

# 将所有与 chat 相关的源文件列出
set(CHAT_SRC
    chat/chat_servlet.cc
    chat/myModule.cc
    chat/protocol.cc
    chat/resource_servlet.cc
    chat/db/users/usersCtr.cc
    chat/db/messages/messagesCtr.cc
    chat/chatCtr.cc
    chat/db/user/userCtr.cc
    chat/db/chat_message/chat_messageCtr.cc   
)

# 创建包含所有功能的 chat 库
add_library(chat SHARED ${CHAT_SRC})

set(YK_LIBS "/home/ubuntu/myServer2/build/lib/libyk.so")

# 添加 chat 库的头文件路径，包括 /home/ubuntu/myServer2 目录
target_include_directories(chat PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    /home/ubuntu/myServer2
    ${JSONCPP_INCLUDE_DIRS}
   
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(chat_room chat_room.cc)

add_executable(test test.cc)

target_link_libraries(chat_room PUBLIC chat)
target_link_libraries(test PUBLIC chat)
# 链接所有依赖项到 chat 库中
target_link_libraries(chat PUBLIC 
    
    ${redis++}
    ${YK_LIBS}
    ${MySQLCppCONN_LIBRARIES} 
    yaml-cpp 
    ${zookeeper}
    jsoncpp_lib 
    OpenSSL::SSL 
    OpenSSL::Crypto
)

