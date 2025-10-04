#include "AppLayer.h"
#include <engine/Application.h>
#include <engine/Log.h>

using namespace std;

int main() {
    se::ApplicationSpec appSpec;
    appSpec.Name = "Architecture";
    appSpec.WindowWidth = 800;
    appSpec.WindowHeight = 600;

    se::LogInit(true);

    se::Application application(appSpec);
    application.PushLayer<AppLayer>();
    application.Run();
}
