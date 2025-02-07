#include "Graphics/Material.h"

namespace Graphics
{
    void UnloadMaterial(Material& material)
    {
        UnloadTexture(material.diffuseMap);
    }
}
