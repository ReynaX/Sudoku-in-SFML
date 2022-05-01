#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>

class Line final : public sf::Drawable
{
public:
	Line(const sf::Vector2f& point1, const sf::Vector2f& point2);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Vertex m_vertices[4];
	int m_thickness;
	sf::Color m_color;
};
