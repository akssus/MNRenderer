#include "VideoBuffer.h"

using namespace sf;

VideoBuffer::VideoBuffer(int width, int height)
{
	_width = width; _height = height;
	_texture.create(width, height);
	_sprite.setTexture(_texture);
	_image.create(width, height, sf::Color::White);
};
VideoBuffer::~VideoBuffer() {};

void VideoBuffer::clearBuffer(sf::Color color = sf::Color::Black)
{
	_image.create(_width, _height, color);
}
void VideoBuffer::drawPixel(int x, int y, sf::Color color)
{
	if (x<0 || x>=_width || y<0 || y>=_height) return; //WTF it makes it sooooo slow shit
	_image.setPixel(x, y, color);
}
void VideoBuffer::swapBuffer(sf::RenderWindow* pWindow)
{
	_texture.loadFromImage(_image);
	pWindow->draw(_sprite);
}