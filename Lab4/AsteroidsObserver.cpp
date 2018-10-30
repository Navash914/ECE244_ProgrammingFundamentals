/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 */


//#include <galaxy-explorer/AsteroidsObserver.hpp>
#include "AsteroidsObserver.hpp"

void AsteroidsObserver::onAsteroidInRange(Asteroid asteroid) {
    asteroid_list.pushFront(asteroid);
}

void AsteroidsObserver::onAsteroidUpdate(Asteroid asteroid) {

}

void AsteroidsObserver::onAsteroidOutOfRange(Asteroid asteroid) {
    onAsteroidDestroy(asteroid);
}

void AsteroidsObserver::onAsteroidDestroy(Asteroid asteroid) {
    for (AsteroidListItem* iterator = asteroid_list.beforeBegin();
        iterator != asteroid_list.end();
        iterator = iterator->getNext()
    ) {
        if (iterator->hasNext() && iterator->getNext()->getData().getID() == asteroid.getID()) {
           asteroid_list.eraseAfter(iterator);
           break;
        }
    }
}
