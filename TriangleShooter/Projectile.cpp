#include "Projectile.h"

Projectile::Projectile() :
projectile(sf::Lines, 2) {
	projectile[0].color = sf::Color::Red;
	projectile[1].color = sf::Color::White;
}

Projectile::Projectile(const sf::Vector2f &origin, const sf::Vector2f &facing) :
	_facing(20.f * facing),
	projectile(sf::Lines, 2) {
	projectile[0] = origin;
	projectile[1] = origin + _facing;
	projectile[0].color = sf::Color::Red;
	projectile[1].color = sf::Color::White;
}

void Projectile::setPosition(const sf::Vector2f & origin, const sf::Vector2f & facing) {
	_facing = facing;
	projectile[0] = origin;
	projectile[1] = origin + 20.f * _facing;
	projectile[0].color = sf::Color::Red;
	projectile[1].color = sf::Color::White;
}

void Projectile::update() {
	projectile[0].position += 0.025f * _facing;
	projectile[1].position += 0.025f * _facing;
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(projectile, states);
}