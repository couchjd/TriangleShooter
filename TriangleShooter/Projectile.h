#pragma once
#include <SFML/Graphics.hpp>

class Projectile :
	public sf::Drawable {
public:
	Projectile();
	Projectile(const Projectile&) = delete;
	Projectile& operator=(const Projectile&) = delete;
	Projectile(const sf::Vector2f &origin, const sf::Vector2f &facing);
	sf::VertexArray projectile;
	sf::Vector2f _facing;
	void setPosition(const sf::Vector2f &origin, const sf::Vector2f &facing);
	void update();
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

