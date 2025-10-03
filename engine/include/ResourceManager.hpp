/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * TextureManager *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄ * ▐
 * ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀ * █▄▄▄▄▄  ▐
 * █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             * █    ▌
 * █          █        █        █    ▌    █         █   █             * ▄▀▄▄▄▄
 * ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             * █    ▐
 * █         █       █ █          █    ▐   █     ▐   █   █               * ▐ ▐
 * ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef INCLUDED_TEXTUREMANAGER_HPP
#define INCLUDED_TEXTUREMANAGER_HPP

#include <iostream>
#include <map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

typedef enum { TEXTURE, FONT } resource_type;

class ResourceManager {
  private:
    std::map<std::string, sf::Texture> m_textureMap;
    std::map<std::string, sf::Font> m_fontMap;

    ResourceManager() {}

  public:
    ~ResourceManager() {}

    static ResourceManager& Instance() {
        static ResourceManager instance;
        return instance;
    }

    bool load(
        const std::string& fileName, const std::string& id, resource_type type
    ) {
        if (type == FONT) {
            sf::Font font;

            if (!font.loadFromFile(fileName)) {
                return false;
            }

            m_fontMap[id] = std::move(font);
            return true;
        } else if (type == TEXTURE) {
            sf::Texture texture;

            if (!texture.loadFromFile(fileName)) {
                return false;
            }

            m_textureMap[id] = std::move(texture);
            return true;
        } else {
            return false;
        }
    }

    sf::Texture& getTexture(const std::string& id) {
        return m_textureMap.at(id);
    }
    sf::Font& getFont(const std::string& id) {
        return m_fontMap.at(id);
    }
};

#endif

/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * MIT License * Copyright (c) 2025 Anonymous *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * * of this software and associated documentation files (the "Software"), to
 * deal        * in the Software without restriction, including without
 * limitation the rights         * to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell            * copies of the Software, and
 * to permit persons to whom the Software is                * furnished to do
 * so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in
 * all       * copies or substantial portions of the Software. *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE        * SOFTWARE. *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */
