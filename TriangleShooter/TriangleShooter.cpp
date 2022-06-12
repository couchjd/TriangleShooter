#include "TriangleShooter.h"

TriangleShooter::TriangleShooter() :
	window(sf::VideoMode(800, 600), "Shooter"),
	alloc(sizeof(Projectile), 1024, 8) {
}

void TriangleShooter::run() {
	sf::VertexArray triangle(sf::Triangles, 3);
	sf::Clock clock;
	std::vector<float> z = { 0, 0, 0 };

	triangle[0].position = sf::Vector2f(100, 100);
	triangle[1].position = sf::Vector2f(150, 125);
	triangle[2].position = sf::Vector2f(100, 150);

	sf::Vertex centroidVertex = sf::Vertex(getCentroid(triangle));

	triangle[0].color = sf::Color::White;
	triangle[1].color = sf::Color::Red;
	triangle[2].color = sf::Color::White;

	centroidVertex.color = sf::Color::White;

	while(window.isOpen()) {
		sf::Vector2f centroid = getCentroid(triangle);

		sf::Event event;
		while(window.pollEvent(event)) {
			if(event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
				window.close();

			if(event.type == sf::Event::MouseButtonPressed) {
				if(event.mouseButton.button == sf::Mouse::Left)
					firing = true;
			}

			if(event.type == sf::Event::MouseButtonReleased) {
				if(event.mouseButton.button == sf::Mouse::Left)
					firing = false;
			}

			if(event.type == sf::Event::KeyReleased) {
				if(event.key.code == sf::Keyboard::W)
					upPressed = false;
				if(event.key.code == sf::Keyboard::S)
					downPressed = false;
				if(event.key.code == sf::Keyboard::A)
					leftPressed = false;
				if(event.key.code == sf::Keyboard::D)
					rightPressed = false;
				if(event.key.code == sf::Keyboard::Space)
					firing = false;
			}

			if(event.type == sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::Space)
					firing = true;
				if(event.key.code == sf::Keyboard::W)
					upPressed = true;
				if(event.key.code == sf::Keyboard::S)
					downPressed = true;
				if(event.key.code == sf::Keyboard::A)
					leftPressed = true;
				if(event.key.code == sf::Keyboard::D)
					rightPressed = true;
			}
			centroidVertex.position = getCentroid(triangle);
		}

		if(clock.getElapsedTime().asSeconds() > 0.05f) {
			clock.restart();
			if(rightPressed) {
				for(int x = 0; x < 3; x++) {
					triangle[x].position = sf::Vector2f(triangle[x].position.x - centroid.x, triangle[x].position.y - centroid.y);
					float xPos = (triangle[x].position.x * cos(THETA)) - (triangle[x].position.y * sin(THETA));
					float yPos = (triangle[x].position.x * sin(THETA)) + (triangle[x].position.y * cos(THETA));
					triangle[x].position = sf::Vector2f(xPos + centroid.x, yPos + centroid.y);
				}
			}

			if(leftPressed) {
				for(int x = 0; x < 3; x++) {
					triangle[x].position = sf::Vector2f(triangle[x].position.x - centroid.x, triangle[x].position.y - centroid.y);
					float xPos = (triangle[x].position.x * cos(-THETA)) - (triangle[x].position.y * sin(-THETA));
					float yPos = (triangle[x].position.x * sin(-THETA)) + (triangle[x].position.y * cos(-THETA));
					triangle[x].position = sf::Vector2f(xPos + centroid.x, yPos + centroid.y);
				}
			}

			if(upPressed) {
				sf::Vector2f faceDir = facing(triangle);
				for(int x = 0; x < 3; x++) {
					triangle[x].position = sf::Vector2f(triangle[x].position.x + 10 * faceDir.x, triangle[x].position.y + 10 * faceDir.y);
				}
			}

			if(downPressed) {
				sf::Vector2f faceDir = facing(triangle);
				for(int x = 0; x < 3; x++) {
					triangle[x].position = sf::Vector2f(triangle[x].position.x - 10 * faceDir.x, triangle[x].position.y - 10 * faceDir.y);
				}
			}

			if(firing) {
				Projectile* newProj = new Projectile(triangle[1].position, facing(triangle));
				projectiles.push_back(newProj);
			}
		}

		window.clear();
		drawProjectiles();
		window.draw(triangle);
		window.draw(&centroidVertex, 1, sf::Points);
		window.display();
	}
}

sf::Vector2f TriangleShooter::getCentroid(const sf::VertexArray& polygon) {
	return sf::Vector2f((polygon[0].position.x + polygon[1].position.x + polygon[2].position.x) / 3,
		(polygon[0].position.y + polygon[1].position.y + polygon[2].position.y) / 3);
}

sf::Vector2f TriangleShooter::normalize(const sf::Vector2f& vector) {
	float length = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	return sf::Vector2f(vector.x / length, vector.y / length);
}

sf::Vector2f TriangleShooter::facing(const sf::VertexArray& polygon) {
	sf::Vector2f v1(polygon[0].position.x - polygon[1].position.x, polygon[0].position.y - polygon[1].position.y);
	sf::Vector2f v2(polygon[2].position.x - polygon[1].position.x, polygon[2].position.y - polygon[1].position.y);

	return normalize(sf::Vector2f(-(v1.x + v2.x), -(v1.y + v2.y)));
}

bool TriangleShooter::outOfBounds(Projectile &projectile) {
	float x = projectile.projectile[0].position.x;
	float y = projectile.projectile[0].position.y;
	float xBound = window.getSize().x;
	float yBound = window.getSize().y;

	return (x > xBound || x < 0 || y > yBound || y < 0);
}

void TriangleShooter::drawProjectiles() {
	std::cout << "Projectile Count: " << projectiles.size() << std::endl;
	for(int x = 0; x < projectiles.size(); x++) {
		window.draw(*projectiles[x]);
		projectiles[x]->update();
		if(outOfBounds(*projectiles[x])) {
			alloc.Free(projectiles[x]);
			projectiles.erase(projectiles.begin() + x);
		}
	}
}