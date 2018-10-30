/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_ASTEROID_HPP
#define ECE244_ASTEROID_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

/**
 * Represents a single asteroid in the playing field.
 */
class Asteroid {
public:
	Asteroid(
		int id,
		int mass,
		int health,
		sf::IntRect hitbox,
		sf::Vector2i velocity
	) {
		this->id = id;
		this->mass = mass;
		this->health = health;
		this->hitbox = hitbox;
		this->velocity = velocity;
	}

	/**
	 * Accessors. Use copy-assign operator to mutate.
	 */
	int getID() const { return id; }
	int getMass() const { return mass; }
	int getHealth() const { return health; }
	sf::IntRect getCurrentHitbox() const { return hitbox; }
	sf::Vector2i getVelocity() const { return velocity; }

private:
	int id; //< an identifier for this asteroid, unique within one game (pointers are dangerous for this purpose)
	int mass; //< what mass of asteroid this is
	int health; //< how much this asteroid must be damaged in order to destroy it

	sf::IntRect hitbox; //< the location and size of this asteroid
	sf::Vector2i velocity; //< the current velocity of this asteroid, in field units per frame
};

#endif /* ECE244_ASTEROID_HPP */
