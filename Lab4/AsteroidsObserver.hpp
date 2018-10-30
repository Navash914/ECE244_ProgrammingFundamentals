/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_GALAXY_EXPLORER_ASTEROIDS_OBSERVER_HPP
#define ECE244_GALAXY_EXPLORER_ASTEROIDS_OBSERVER_HPP

//#include <galaxy-explorer/AsteroidList.hpp>
#include "AsteroidList.hpp"

// Forward declarations
class GameAI;

/**
 * This class is notified of various events that allows it to keep track
 * of the current set of asteroids that are one the screen.
 */
class AsteroidsObserver {
public:
	AsteroidsObserver(GameAI* game_ai = nullptr) {
		this->game_ai = game_ai;
	}

	/**
	 * Called whenever a new asteroid appears on the field
	 */
	void onAsteroidInRange(Asteroid asteroid);

	/**
	 * Called when an asteroid has moved.
	 * Use the ID of the asteroid to determine what asteroid to replace
	 */
	void onAsteroidUpdate(Asteroid asteroid);

	/**
	 * Called when an asteroid leaves the field without being destroyed
	 */
	void onAsteroidOutOfRange(Asteroid asteroid);

	/**
	 * Called when an asteroid is destroyed by a phaser shot
	 */
	void onAsteroidDestroy(Asteroid asteroid);

	/**
	 * This list must contain the current list of entities that are in the field.
	 * Ie. entities must be added when the move in-range, and removed when destroyed or when they move out-of-range.
	 * Order is not tested.
	 */
	const AsteroidList& asteroids() const { return asteroid_list; }
	      AsteroidList& asteroids()       { return asteroid_list; }
private:
	AsteroidList asteroid_list;
	GameAI* game_ai;
};

#endif /* ECE244_GALAXY_EXPLORER_ASTEROIDS_OBSERVER_HPP */
