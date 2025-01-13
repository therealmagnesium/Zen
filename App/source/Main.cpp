#include "ZenEditor.h"

int main(s32 argc, char** argv)
{
    ApplicationSpecification appInfo;
    appInfo.windowWidth = 1600;
    appInfo.windowHeight = 900;

    ZenEditor editor(appInfo);
    editor.Run();

    return 0;
}
