#include <iostream>

#include "runtime/application.hpp"
#include "runtime/runtime.hpp"

int main(int argc, char *argv[]) {
    // instance main application
    std::cout << "start of program" << std::endl;
    ApplicationConfig appCfg;
    appCfg.name = "Sprinter";
    appCfg.cwd = "~/source/repos/hobby/SceneGL/sample_project";

    Application* app = new Application(appCfg);
    app->SetOperator(new Runtime());
    app->Run();
    delete app;
    
    std::cout << "end of program" << std::endl;
    return 0;
}
