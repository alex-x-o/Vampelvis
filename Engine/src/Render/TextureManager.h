

namespace Engine
{
    class Renderer;
    struct Texture;

    class TextureManager
    {
    public:
        /*
        Texture* GetLevelTexture(int levelId_, int categoryId_, std::string name_);
        Texture* GetLevelOrCommonTexture(int levelId_, int categoryId_, std::string name_);
        */
        Texture* GetCommonTexture(int categoryId_, std::string name_);
        
        bool CreateTexture(Renderer* renderer_, int lvlId_, int categoryId_, std::string name_, std::string path_);
        bool CreateTexture(Renderer* renderer_, int categoryId_, std::string name_, std::string path_);

        TextureManager() = default;
    private:
        using TextureDictionary = std::unordered_map<std::string, int>;
        using CategoryMap = std::unordered_map<int, TextureDictionary>;

        std::unordered_map<int, CategoryMap> m_LevelMap;
        std::unordered_map<int, std::unique_ptr<Texture>> m_TextureStore;

        int nextId = 0;

        TextureManager(const TextureManager& other) = delete;
        TextureManager& operator=(TextureManager& other) = delete;
        
    };
}
