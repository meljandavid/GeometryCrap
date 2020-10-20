#pragma once

#include<SFML/Graphics.hpp>
#include<vector>
#include"Objects.h"
#include<unordered_set>

class Verticemap : public sf::Drawable, public sf::Transformable {
public:
    unsigned int width = 0, height = 0;
    bool load(const std::string& tileset, sf::Vector2u _tileSize, const std::vector<std::vector<int>>& tiles);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray vertices, m_vertices;
    sf::Texture m_tileset;
    sf::Vector2u tileSize;
    int textureWidth = 0;
};
