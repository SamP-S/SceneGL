#include <iostream>
#include "editor.hpp"

int main(int argc, char *argv[]) {
    // instance main application
    std::cout << "start of program" << std::endl;
    ApplicationConfig appCfg;
    appCfg.name = "Spectator";
    appCfg.cwd = "~/source/repos/hobby/SceneGL/sample_project";

    Application& app = Application::Create(appCfg);
    app.SetOperator(new Editor());
    app.Run();
    Application::Destroy();
    
    std::cout << "end of program" << std::endl;
    return 0;
}
