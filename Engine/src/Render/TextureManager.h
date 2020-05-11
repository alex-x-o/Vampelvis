

namespace Engine
{
    class Renderer;
    struct Texture;

    class TextureManager
    {
    public:
        bool CreateTexture(Renderer* renderer_, int lvlId_, int categoryId_, std::string name_, std::string path_);
        bool CreateTexture(Renderer* renderer_, int categoryId_, std::string name_, std::string path_);

        Texture* GetCommonTexture(int categoryId_, std::string name_);
		Texture* GetLevelTexture(int levelId_, int categoryId_, std::string name_);
        Texture* GetLevelOrCommonTexture(int levelId_, int categoryId_, std::string name_);
        Texture* GetRandomTexture(int levelId_, int categoryId_);

        TextureManager() = default;
    private:
        using TextureDictionary = std::unordered_map<std::string, int>;
        using CategoryMap = std::unordered_map<int, TextureDictionary>;

        std::unordered_map<int, CategoryMap> m_LevelMap;
        std::unordered_map<int, std::unique_ptr<Texture>> m_TextureStore;

        int nextId = 0;

        TextureManager(const TextureManager& other) = delete;
        TextureManager& operator=(TextureManager& other) = delete;

        
        Texture* GetTextureLevelTextureInternal(int levelId_, int categoryId_, std::string name_, bool shouldLog_);
        Texture* GetTextureFromStore(int id_, std::string name_, bool shouldLog_);
        TextureDictionary* GetTextureDict(int levelId_, int categoryId_, bool shouldLog_);
    };
}
