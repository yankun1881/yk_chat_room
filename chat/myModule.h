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

} // namespace chat 