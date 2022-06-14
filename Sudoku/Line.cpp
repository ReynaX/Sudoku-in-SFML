#include "Line.h"
#include <SFML/Graphics.hpp>

Line::Line(const sf::Vector2f& point1, const sf::Vector2f& point2) :
	m_thickness(5.f), m_color(sf::Color::Black){
	sf::Vector2f direction = point2 - point1;
	sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
	sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	sf::Vector2f offset = (m_thickness / 2.f) * (unitPerpendicular);

	m_vertices[0].position = point1 + offset;
	m_vertices[1].position = point2 + offset;
	m_vertices[2].position = point2 - offset;
	m_vertices[3].position = point1 - offset;

	for(auto& m_vertice: m_vertices) m_vertice.color = m_color;
}


void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(m_vertices, 4, sf::Quads);
}
