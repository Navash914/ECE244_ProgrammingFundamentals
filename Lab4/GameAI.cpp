#include <cmath>

//#include <galaxy-explorer/GameAI.hpp>
//#include <galaxy-explorer/MyAIData.hpp>

#include "GameAI.hpp"
#include "MyAIData.hpp"
#include "AsteroidsObserver.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>


GameAI::GameAI(const GameInfo& game_info, sf::RenderTarget* debug_rt)
{
	this->debug_rt = debug_rt;
	this->game_info = game_info;
	this->asteroid_observer = AsteroidsObserver(this);
	this->my_game_ai = new MyAIData();

	// customState().debug_on = false;
}

GameAI::~GameAI() {
	delete my_game_ai;
}

SuggestedAction GameAI::suggestAction(const ShipState& ship_state) {
	debug_rt->clear(sf::Color::Transparent);

	/*if (customState().debug_on) {
		if (!asteroidsObserver().asteroids().isEmpty()) {
			const sf::IntRect first_ast_hb = asteroidsObserver().asteroids().front().getCurrentHitbox();
			sf::RectangleShape ast_rect(sf::Vector2f(first_ast_hb.width, first_ast_hb.height));
			ast_rect.setPosition(first_ast_hb.left, first_ast_hb.top);
			ast_rect.setOutlineThickness(33.0f); // if lines are too thin, they won't show up sometimes
			ast_rect.setOutlineColor(sf::Color::Yellow);
			ast_rect.setFillColor(sf::Color::Transparent);
			debug_rt->draw(ast_rect);
		}
	}*/

	/*
	// return SimpleActions::NO_CHANGE;
	return SimpleActions::START_YAWING_CW;
	// return SimpleActions::START_YAWING_ACW;
	// return SimpleActions::STOP_YAWING_AND_FIRE;
	 return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
	 */

    const Asteroid* asteroidToHit = getMostDangerousAsteroid(asteroid_observer, ship_state);
	if (asteroidToHit == nullptr) {
	    if (ship_state.millidegree_rotation > 45 * 1000)
	        return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
        else if (ship_state.millidegree_rotation < -45 * 1000)
            return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
        else
            return SuggestedAction{ SuggestedAction::YawingNoChange, SuggestedAction::FiringTry };
	} else {
        if (customState().debug_on) {
            if (!asteroidsObserver().asteroids().isEmpty()) {
                const sf::IntRect first_ast_hb = asteroidToHit->getCurrentHitbox();
                sf::RectangleShape ast_rect(sf::Vector2f(first_ast_hb.width, first_ast_hb.height));
                ast_rect.setPosition(first_ast_hb.left, first_ast_hb.top);
                ast_rect.setOutlineThickness(33.0f); // if lines are too thin, they won't show up sometimes
                ast_rect.setOutlineColor(sf::Color::Yellow);
                ast_rect.setFillColor(sf::Color::Transparent);
                debug_rt->draw(ast_rect);
            }
        }
        int angleToFire_L = getAngleToAsteroid(*asteroidToHit, ship_state, true);
        int angleToFire_R = getAngleToAsteroid(*asteroidToHit, ship_state, false);
        int shipAngle = ship_state.millidegree_rotation;
        if (shipAngle < angleToFire_L)
            return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
        if (shipAngle > angleToFire_R)
            return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
        return SimpleActions::STOP_YAWING_AND_FIRE;
	}

}

int getAngleToAsteroid (const Asteroid& asteroid, const ShipState& shipState, bool leftEnd) {
    sf::Vector2f forward(0.0, 1.0);
    sf::Vector2f shipDot( shipState.hitbox.left + shipState.hitbox.width/2.0f,
                            shipState.hitbox.top + shipState.hitbox.height/2.0f );
    sf::Vector2f asteroidDot;
    asteroidDot.y = asteroid.getCurrentHitbox().top + asteroid.getCurrentHitbox().height;
    asteroidDot.x = asteroid.getCurrentHitbox().left;
    if (!leftEnd)
        asteroidDot.x += asteroid.getCurrentHitbox().width;
    sf::Vector2f shipToAst ( asteroidDot.x - shipDot.x, asteroidDot.y - shipDot.y );

    float dotProduct = shipToAst.x * forward.x + shipToAst.y * forward.y;
    float magProduct = (float) std::sqrt(shipToAst.x * shipToAst.x + shipToAst.y * shipToAst.y);
    float angle = (std::acos(dotProduct / magProduct) * 180.0f * 7.0f) / 22.0f;
    angle *= 1000;

    return (int) angle;
}

const Asteroid* getMostDangerousAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState) {
    if (asteroidsObserver.asteroids().isEmpty())
        return nullptr;
    const Asteroid* asteroid = nullptr;
    float maxDanger = 0;
    /*sf::Vector2f ship_TopLeft( shipState.hitbox.left, shipState.hitbox.top);
    sf::Vector2f ship_TopRight( shipState.hitbox.left + shipState.hitbox.width, shipState.hitbox.top);
    sf::Vector2f ship_BotLeft( shipState.hitbox.left, shipState.hitbox.top + shipState.hitbox.height);
    sf::Vector2f ship_BotRight( shipState.hitbox.left + shipState.hitbox.width, shipState.hitbox.top + shipState.hitbox.height);*/

    sf::Vector2i shipDot( shipState.hitbox.left + shipState.hitbox.width/2,
                          shipState.hitbox.top + shipState.hitbox.height/2 );

    for (const AsteroidListItem* it = asteroidsObserver.asteroids().begin();
            it != asteroidsObserver.asteroids().end();
            it = it->getNext()
    ) {
        Asteroid ast = it->getData();
        sf::Vector2i asteroidDot (ast.getCurrentHitbox().left + ast.getCurrentHitbox().width / 2,
                                    ast.getCurrentHitbox().top + ast.getCurrentHitbox().height / 2);
        sf::Vector2i velocity = ast.getVelocity();
        int t = std::abs(shipDot.y - asteroidDot.y) / velocity.y;
        sf::Vector2i finalPos (asteroidDot.x + velocity.x * t, asteroidDot.y + velocity.y * t);
        sf::IntRect finalHitbox (finalPos.x - ast.getCurrentHitbox().width / 2, finalPos.y - ast.getCurrentHitbox().height / 2,
                                    ast.getCurrentHitbox().width, ast.getCurrentHitbox().height);
        if (shipState.hitbox.intersects(finalHitbox)) {
            float danger = 1.0f / (float) t;
            if (danger > maxDanger) {
                maxDanger = danger;
                asteroid = &it->getData();
            }
        }
    }
    return asteroid;

}