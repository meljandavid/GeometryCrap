#include "Verticemap.h"

bool Verticemap::load(const std::string& tileset, sf::Vector2u _tileSize, const std::vector<std::vector<int>>& tiles) {
    height = tiles.size();
    width = (height == 0 ? 0 : tiles[0].size());
    tileSize = _tileSize;

    vertices.setPrimitiveType(sf::Quads);
    m_vertices.setPrimitiveType(sf::Quads);
    std::unordered_set<int> dec{ 1,2 };
    float outline = 1.f;

    for (int y = 0; y < tiles.size(); y++) {
        for (int x = 0; x < tiles[y].size(); x++) {
            if (dec.count(tiles[y][x]) && (x == 0 || !dec.count(tiles[y][x - 1]))) {
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x, tileSize.y * y), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x + outline, tileSize.y * y), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x + outline, tileSize.y * (y + 1)), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x, tileSize.y * (y + 1)), sf::Color::White));
            }
            if (dec.count(tiles[y][x]) && (x == tiles[y].size() - 1 || !dec.count(tiles[y][x + 1]))) {
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1) - outline, tileSize.y * y), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1), tileSize.y * y), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1), tileSize.y * (y + 1)), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1) - outline, tileSize.y * (y + 1)), sf::Color::White));
            }
        }
    }

    for (size_t x = 0; x < tiles[0].size(); x++) {
        for (size_t y = 0; y < tiles.size(); y++) {
            if (dec.count(tiles[y][x]) && (y == 0 || !dec.count(tiles[y - 1][x]))) {
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x, tileSize.y * y), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1), tileSize.y * y), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1), tileSize.y * y + outline), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x, tileSize.y * y + outline), sf::Color::White));
            }
            if (dec.count(tiles[y][x]) && (y == tiles.size() - 1 || !dec.count(tiles[y+1][x]))) {
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x, tileSize.y * (y + 1) - outline), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1), tileSize.y * (y + 1) - outline), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * (x + 1), tileSize.y * (y + 1)), sf::Color::White));
                vertices.append(sf::Vertex(sf::Vector2f(tileSize.x * x, tileSize.y * (y + 1)), sf::Color::White));
            }
        }
    }

    if (!m_tileset.loadFromFile(tileset))
        return false;
    textureWidth = m_tileset.getSize().x / tileSize.x;
    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            int tileNumber = tiles[j][i];
            //if (tileNumber > 3) {
                int tu = tileNumber % textureWidth;
                int tv = tileNumber / textureWidth;
                size_t bef = m_vertices.getVertexCount();
                m_vertices.resize(bef + 4);

                sf::Vertex* quad = &m_vertices[bef];

                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            //}
        }
    }

    return true;
}

void Verticemap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
    states.texture = nullptr;
    target.draw(vertices, states);
}