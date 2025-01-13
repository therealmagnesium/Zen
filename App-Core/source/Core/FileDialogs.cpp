#include "Core/FileDialogs.h"
#include "Core/Log.h"
#include <nfd.h>

namespace Core
{
    std::string FileDialogs::OpenFile(const char* filter, const char* defaultPath)
    {
        std::string returnPath;
        char* outPath = NULL;
        nfdresult_t dialogResult = NFD_OpenDialog(filter, defaultPath, &outPath);

        switch (dialogResult)
        {
            case NFD_OKAY:
                returnPath = outPath;
                free(outPath);
                break;
            case NFD_CANCEL:
                break;
            default:
                ERROR("An error occured with opening the native file dialog");
                break;
        }

        return returnPath;
    }

    std::string FileDialogs::SaveFile(const char* filter, const char* defaultPath)
    {
        std::string returnPath;
        char* savePath = NULL;
        nfdresult_t dialogResult = NFD_SaveDialog(filter, defaultPath, &savePath);

        switch (dialogResult)
        {
            case NFD_OKAY:
                returnPath = savePath;
                free(savePath);
                break;
            case NFD_CANCEL:
                break;
            default:
                ERROR("An error occured with opening the native file dialog");
                break;
        }

        return returnPath;
    }
}
