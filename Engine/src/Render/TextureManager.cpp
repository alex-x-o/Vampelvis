#include "precomp.h"
#include "TextureManager.h"

#include "Render/Texture.h"
#include "Render/Renderer.h"

#include <random>

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
        return GetTextureLevelTextureInternal(0, categoryId_, name_, true);
    }

    Texture* TextureManager::GetLevelTexture(int levelId_, int categoryId_, std::string name_)
    {
        return GetTextureLevelTextureInternal(levelId_, categoryId_, name_, true);
    }

    Texture* TextureManager::GetLevelOrCommonTexture(int levelId_, int categoryId_, std::string name_)
    {
        auto texture = GetTextureLevelTextureInternal(levelId_, categoryId_, name_, false);
        if (texture == nullptr)
        {
            texture = GetTextureLevelTextureInternal(0, categoryId_, name_, true);
        }

        return texture;
    }

    Texture* TextureManager::GetRandomTexture(int levelId_, int categoryId_)
    {
        auto textureDict = GetTextureDict(levelId_, categoryId_, true);
        if (textureDict == nullptr)
        {
            LOG_ERROR("Texture dict not found");
            return nullptr;
        }
            

        int size = static_cast<int>(textureDict->size());
        if (size == 0)
        {
            LOG_ERROR("No textures in category {}!", categoryId_);
            return nullptr;
        }

        std::random_device rd;
        std::uniform_int_distribution<> randomIndex(0, size - 1);

        int texIndex = randomIndex(rd);
        int currentIndex = 0;

        Texture* texture = nullptr;

        for (auto [key, value] : *textureDict)
        {
            if(currentIndex == texIndex)
            {
                texture = GetTextureFromStore(value, key, true);
                break;
            }
            currentIndex++;
        }

        return texture;
    }

    Texture* TextureManager::GetTextureLevelTextureInternal(int levelId_, int categoryId_, std::string name_, bool shouldLog_)
    {
        auto textureDict = GetTextureDict(levelId_, categoryId_, shouldLog_);
        if (textureDict == nullptr)
            return nullptr;

        auto textureDictFinder = textureDict->find(name_);
        if (textureDictFinder == textureDict->end()) {
            if(shouldLog_)
            {
                LOG_ERROR("Attempting to get an unknown texture! name: {}", name_);
            }

            return nullptr;
        }

        int id = textureDictFinder->second;

        return GetTextureFromStore(id, name_, true);
    }

    TextureManager::TextureDictionary* TextureManager::GetTextureDict(int levelId_, int categoryId_, bool shouldLog_)
    {
        auto levelFinder = m_LevelMap.find(levelId_);
        if (levelFinder == m_LevelMap.end()) {
            if (shouldLog_)
            {
                LOG_ERROR("Attempting to get a texture from a level that doesn't exist! level: {}", levelId_);
            }

            return nullptr;
        }
        auto& categoryMap = levelFinder->second;

        auto categoryFinder = categoryMap.find(categoryId_);
        if (categoryFinder == categoryMap.end()) {
            if (shouldLog_)
            {
                LOG_ERROR("Attempting to get a texture from a category that doesn't exist! category: {}", categoryId_);
            }

            return nullptr;
        }

        TextureDictionary* textureDict = &categoryFinder->second;
        
        return textureDict;
    }

    Texture* TextureManager::GetTextureFromStore(int id_, std::string name_, bool shouldLog_)
    {
        auto textureFinder = m_TextureStore.find(id_);
        if (textureFinder == m_TextureStore.end()) {
            if (shouldLog_)
            {
                LOG_ERROR("Texture {} not found with expected id: {}", name_, id_);
            }
            return nullptr;
        }


        return m_TextureStore.at(id_).get();
    }
}
