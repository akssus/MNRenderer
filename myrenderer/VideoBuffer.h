#pragma once

#include <SFML/Graphics.hpp>

class VideoBuffer
{
public:
	VideoBuffer(int width, int height);
	~VideoBuffer();

	void clearBuffer(sf::Color color);
	void drawPixel(int x, int y, sf::Color color);
	void swapBuffer(sf::RenderWindow* pWindow);
private:
	int			_width, _height;
	sf::Texture _texture;
	sf::Sprite	_sprite;
	sf::Image	_image;
};