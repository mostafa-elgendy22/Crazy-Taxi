#include <Windows.h>
#include <MMSystem.h>

#pragma (lib, "winmm")

class Utils
{
public:
    static void playAudio(const char *file)
    {
      PlaySound(TEXT(file),  NULL, SND_SYNC);

    }
};