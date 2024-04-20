#include <yk.h>
#include "chat/myModule.h"


int main(int argc, char** argv) {
    yk::ModuleMgr::GetInstance()->add(yk::Module::ptr(new chat::MyModule));
    yk::Application app;
    if(app.init(argc, argv)) {
        return app.run();
    }
    
    return 0;
}
