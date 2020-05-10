#include "precomp.h"
#include "TextureManager.h"

#include "Render/Texture.h"
#include "Render/Renderer.h"

namespace Engine
{
    bool TextureManager::CreateTexture(Renderer* renderer_, int lvlId_, int categoryId_, std::string name_, std::string path_)
    {
        auto& textureDict = m_LevelMap[lvlId_][categoryId_];
        if (textureDict.find(name_) != textureDict.end()) {
            LOG_ERROR("Attempting to create a texture twice! name: {}, path: {}", name_, path_);
            return false;
        }

        textureDict[name_] = nextId;
        if (m_TextureStore.find(nextId) != m_TextureStore.end()) {
            LOG_ERROR("Texture with same ID ({}) exists in store! Texture name: {}, path: {}", nextId, name_, path_);
            return false;
        }
        m_TextureStore.emplace(nextId, std::make_unique<Texture>(renderer_, path_));

        nextId++;

        return true;
    }

    bool TextureManager::CreateTexture(Renderer* renderer_, int categoryId_, std::string name_, std::string path_)
    {
        auto& textureDict = m_LevelMap[0][categoryId_];
        if (textureDict.find(name_) != textureDict.end()) {
            LOG_ERROR("Attempting to create a texture twice! name: {}, path: {}", name_, path_);
            return false;
        }

        textureDict[name_] = nextId;
        if (m_TextureStore.find(nextId) != m_TextureStore.end()) {
            LOG_ERROR("Texture with same ID ({}) exists in store! Texture name: {}, path: {}", nextId, name_, path_);
            return false;
        }
        m_TextureStore.emplace(nextId, std::make_unique<Texture>(renderer_, path_));

        nextId++;

        return true;
    }

    Texture* TextureManager::GetCommonTexture(int categoryId_, std::string name_)
    {
        auto levelFinder = m_LevelMap.find(0);
        if (levelFinder == m_LevelMap.end()) {
            LOG_ERROR("Attempting to get a texture from a level that doesn't exist! level: {} (Common)", 0);
            return nullptr;
        }
        auto& levelMap = levelFinder->second;

        auto categoryFinder = levelMap.find(categoryId_);
        if (categoryFinder == levelMap.end()) {
            LOG_ERROR("Attempting to get a texture from a category that doesn't exist! category: {}", categoryId_);
            return nullptr;
        }
        auto& categoryMap = categoryFinder->second;

        auto textureDictFinder = categoryMap.find(name_);
        if (textureDictFinder == categoryMap.end()) {
            LOG_ERROR("Attempting to get an unknown texture! name: {}", name_);
            return nullptr;
        }

        int id = textureDictFinder->second;
        auto textureFinder = m_TextureStore.find(id);
        if (textureFinder == m_TextureStore.end()) {
            LOG_ERROR("Texture {} not found with expected id: {}", name_, id);
            return nullptr;
        }


        return m_TextureStore.at(id).get();
    }

    /*
    bool TextureManager::CreateTexture(Renderer* renderer_, int categoryId_, std::string name_, std::string path_)
    {
        auto textureMap = m_LevelStore[0][categoryId_];
        if (textureMap.find(name_) != textureMap.end()) {
            LOG_ERROR("Attempting to create a texture twice! name: {}, path: {}", name_, path_);
            return false;
        }
        textureMap.emplace(name_, std::make_unique<Texture>(renderer_, path_));


        return true;
    }

    Texture* TextureManager::GetLevelTexture(int levelId_, int categoryId_, std::string name_)
    {
        auto levelFinder = m_LevelStore.find(levelId_);
        if (levelFinder == m_LevelStore.end()) {
            LOG_ERROR("Attempting to get a texture from a level that doesn't exist! level: {}", levelId_);
            return nullptr;
        }
        auto categoryMap = levelFinder->second;

        auto categoryFinder = categoryMap.find(categoryId_);
        if (categoryFinder == categoryMap.end()) {
            LOG_ERROR("Attempting to get a texture from a category that doesn't exist! category: {}", categoryId_);
            return nullptr;
        }
        auto textureMap = categoryFinder->second;

        if (textureMap.find(name_) == textureMap.end()) {
            LOG_ERROR("Attempting to get a texture that doesn't exist! name: {}", name_);
            return nullptr;
        }

        return textureMap.at(name_).get();
    }

    Texture* TextureManager::GetLevelOrCommonTexture(int levelId_, int categoryId_, std::string name_)
    {
        auto levelFinder = m_LevelStore.find(levelId_);
        if (levelFinder == m_LevelStore.end()) {
            levelId_ = 0;
        }

        levelFinder = m_LevelStore.find(levelId_);
        if (levelFinder == m_LevelStore.end()) {
            LOG_ERROR("Attempting to get a texture from a level that doesn't exist! level: {}", levelId_);
            return nullptr;
        }


        auto categoryMap = levelFinder->second;

        auto categoryFinder = categoryMap.find(categoryId_);
        if (categoryFinder == categoryMap.end()) {
            LOG_ERROR("Attempting to get a texture from a category that doesn't exist! category: {}", categoryId_);
            return nullptr;
        }
        auto textureMap = categoryFinder->second;

        if (textureMap.find(name_) == textureMap.end()) {
            LOG_ERROR("Attempting to get a texture that doesn't exist! name: {}", name_);
            return nullptr;
        }

        return textureMap.at(name_).get();
    }

    Texture* TextureManager::GetLevelTexture(int categoryId_, std::string name_)
    {
        auto levelFinder = m_LevelStore.find(0);
        if (levelFinder == m_LevelStore.end()) {
            LOG_ERROR("Attempting to get a texture from a level that doesn't exist! level: {} (common)", 0);
            return nullptr;
        }
        auto categoryMap = levelFinder->second;

        auto categoryFinder = categoryMap.find(categoryId_);
        if (categoryFinder == categoryMap.end()) {
            LOG_ERROR("Attempting to get a texture from a category that doesn't exist! category: {}", categoryId_);
            return nullptr;
        }
        auto textureMap = categoryFinder->second;

        if (textureMap.find(name_) == textureMap.end()) {
            LOG_ERROR("Attempting to get a texture that doesn't exist! name: {}", name_);
            return nullptr;
        }

        return textureMap.at(name_).get();
    }

    */


}
