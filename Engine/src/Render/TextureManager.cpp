#include "precomp.h"
#include "TextureManager.h"

#include "Render/Texture.h"
#include "Render/Renderer.h"

namespace Engine
{

    bool TextureManager::CreateTexture(Renderer* renderer_, int lvlId_, std::string name_, std::string path_)
    {
        if (m_Textures.find(name_) != m_Textures.end()) {
            LOG_ERROR("Attempting to create a texture twice! name: {}, path: {}", name_, path_);
            return false;
        }

        m_Textures.emplace(name_, std::make_unique<Texture>(renderer_, path_));
        auto cond = m_Textures.at(name_)->m_Texture;

        if (cond != nullptr)
        {
            m_Multi.emplace(lvlId_, &m_Textures);
            /*auto it = m_Multi.equal_range(lvlId_);
            for (auto i = it.first; i != it.second; ++i)
            {
                std::cout << i->first << std::endl;
            }*/
        }

        return cond != nullptr;
    }

    Texture* TextureManager::GetTexture(std::string name_)
    {
        if (m_Textures.find(name_) == m_Textures.end()) {
            LOG_ERROR("Attempting to get a texture that doesn't exist! name: {}", name_);
            return nullptr;
        }

        return m_Textures.at(name_).get();
    }

    void TextureManager::WriteTextures()
    {
        for (auto i = m_Multi.begin(); i != m_Multi.end(); i++)
        {
            for (auto it = i->second->begin(); it != i->second->end(); it++)
            {
                std::cout << i->first << '   ' << it->first << '\n';
            }
        }
    }

}
