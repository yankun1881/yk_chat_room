#include "protocol.h"
#include <yk.h>
#include <jsoncpp/json/json.h>

namespace chat {

ChatMessage::ptr ChatMessage::Create(const std::string& v) {
    Json::Value json;
    Json::Reader reader;
    if (!reader.parse(v, json)) {
        return nullptr;
    }
    ChatMessage::ptr rt(new ChatMessage);

    auto names = json.getMemberNames();
    for (auto& i : names) {
        rt->m_datas[i] = json[i].asString();
    }
    
    return rt;
}


ChatMessage::ChatMessage() {
}

std::string ChatMessage::get(const std::string& name) {
    auto it = m_datas.find(name);
    if(it == m_datas.end()){
        return "";
    }
    auto& v = m_datas[name];
    if(v.isString()) {
        return v.asString();
    }
    return "";
}

void ChatMessage::set(const std::string& name, const std::string& val) {
    Json::Value value(val);
    m_datas[name] = value;
}

std::string ChatMessage::toString() const {
    Json::Value json;
    for(auto& i : m_datas) {
        json[i.first] = i.second;
    }    
    Json::StreamWriterBuilder builder;
    
    builder["indentation"] = ""; // 禁止缩进
    builder["enableYAMLCompatibility"] = true; // 启用 YAML 兼容模式
    builder["emitUTF8"] = true; // 确保输出 UTF-8 编码，这通常是默认设置
    std::ostringstream oss;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(json, &oss);
    
    return oss.str();
}
void ChatMessage::addDataArray(const std::vector<std::map<std::string, std::string>>& dataArray,const std::string& data) {
    Json::Value dataJson(Json::arrayValue); // 创建一个JSON数组

    for (const auto& dataItem : dataArray) {
        Json::Value itemJson; // 创建一个JSON对象用于存放单个数据项
        for (const auto& kv : dataItem) {
            itemJson[kv.first] = kv.second; // 添加键值对到对象
        }
        dataJson.append(itemJson); // 将对象添加到数组
    }
    m_datas[data] = dataJson;
}

std::string ChatMessage::toDataArray(const std::vector<std::map<std::string, std::string>>& dataArray){
    Json::Value dataJson(Json::arrayValue); // 创建一个JSON数组
    for (const auto& dataItem : dataArray) {
        Json::Value itemJson; // 创建一个JSON对象用于存放单个数据项
        for (const auto& kv : dataItem) {
            itemJson[kv.first] = kv.second; // 添加键值对到对象
        }
        dataJson.append(itemJson); // 将对象添加到数组
    }
    Json::StreamWriterBuilder builder;
    
    builder["indentation"] = ""; // 禁止缩进
    builder["enableYAMLCompatibility"] = true; // 启用 YAML 兼容模式
    builder["emitUTF8"] = true; // 确保输出 UTF-8 编码，这通常是默认设置
    std::ostringstream oss;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(dataJson, &oss);
    
    return oss.str();
}


}
