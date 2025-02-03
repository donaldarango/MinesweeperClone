#include "TextureManager.h"
#include <iostream>

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(const string& textureName) {
    string path = "images/";

    if(textureName.find("digits") == 00) {
        path += textureName.substr(0, textureName.length() - 1) + ".png";
        unsigned char element = textureName.at(6);
        if (element == '-') {
            element -= 35;
        }
        else
            element -= 48;
        textures[textureName].loadFromFile(path, true, sf::IntRect({21*element, 0}, {21, 32}));
        }
    else {
        path += textureName + ".png";   
        textures[textureName].loadFromFile(path);
    }
}

sf::Texture& TextureManager::GetTexture(const string& textureName) {
    if (textures.find(textureName) == textures.end()){
        LoadTexture(textureName);
    }
    return textures[textureName];
}

void TextureManager::Clear() {
    textures.clear();
}
