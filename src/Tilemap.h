#pragma once

#include<SFML/Graphics.hpp>
#include<vector>

class Tilemap : public sf::Drawable, public sf::Transformable {
public:
    unsigned int width=0, height=0;
    bool load(const std::string& tileset, sf::Vector2u _tileSize, const std::vector<std::vector<int>>& tiles);

    void updateTile(sf::Vector2u pos, int tileNumber);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    int textureWidth=0;
    sf::Vector2u tileSize;
};
