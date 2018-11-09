/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP
#define ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP

#include "Asteroid.hpp"
#include "AsteroidsObserver.hpp"
#include "GameAI.hpp"

int getAngleToAsteroid (const Asteroid& asteroid, const ShipState& shipState, bool leftEnd);
const Asteroid* getMostDangerousAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState)

/**
 * Student editable struct for storing their ai state
 * Yes, you may change this header.
 * Maybe you want to remember the last asteroid Id you shot at?
 */
struct MyAIData {
	bool debug_on = true;
	const Asteroid* asteroidToHit;
};

#endif /* ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP */
