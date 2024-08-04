#include <yk.h>
namespace chat
{
class MyModule : public yk::Module {
public:
    typedef std::shared_ptr<MyModule> ptr;
    MyModule();
    bool onLoad() override;
    bool onUnload() override;
    bool onServerReady() override;
    bool onServerUp() override;

    //读取该目录下，前端所需要的元素
    void readfile(const std::string& uri,const std::string& path,const std::string& suffix);
    std::vector<std::string> files;
    std::vector<std::string> uris;

};

class MyRockModule : public yk::RockModule {
public:
    MyRockModule(): yk::RockModule("","1.0",""){}
    MyRockModule(const std::string& name, const std::string& version, const std::string& filename)
        : yk::RockModule(name, version, filename) {}

    virtual bool handleRockRequest(yk::RockRequest::ptr request,
                                   yk::RockResponse::ptr response,
                                   yk::RockStream::ptr stream) override;

    virtual bool handleRockNotify(yk::RockNotify::ptr notify,
                                  yk::RockStream::ptr stream) override ;
};


} // namespace chat 