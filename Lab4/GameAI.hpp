/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_GALAXY_EXPLORER_GAME_AI_HPP
#define ECE244_GALAXY_EXPLORER_GAME_AI_HPP

//#include <galaxy-explorer/AsteroidsObserver.hpp>
#include <./AsteroidsObserver.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

// Forward declaration
class MyAIData;

/**
 * A struct with data about your ship
 * When the phaser ready countdown reaches zero,
 * and there has been a request to fire, the phaser will fire.
 */
struct ShipState {
	bool fire_phaser_requested = false; //< Has there been a request to fire the phaser since it last fired?
	int frames_until_phaser_is_ready = 0; //< Countdown until the phaser is ready to fire
	int score = 0; //< your current score
	int millidegree_rotation = 0; //< rotation in millidegrees
	sf::IntRect hitbox; //< location of ship
};

/**
 * AIs use this type to communicate their intention.
 */
struct SuggestedAction {
	/**
	 * Commands for changing the ships direction.
	 */
	enum Yawing { YawingClockwise, YawingAntiClockwise, YawingStop, YawingNoChange, };

	/**
	 * Commands for controlling the ship's phaser.
	 * Note that if the phaser is not cool `Try` sets the request flag and does nothing else
	 */
	enum Firing { FiringTry, FiringNo, };

	Yawing yawing = YawingNoChange;
	Firing firing = FiringNo;
};

std::ostream& operator<<(std::ostream& os, const SuggestedAction::Yawing& say);
std::ostream& operator<<(std::ostream& os, const SuggestedAction::Firing& saf);
std::ostream& operator<<(std::ostream& os, const SuggestedAction& sa);

/**
 * Some simple/common actions to take.
 * There should be some examples in the given code,
 * but to use these in your AI, simply:
 *     return SimpleAction::STOP_YAWING_AND_FIRE;
 * for example.
 */
namespace SimpleActions {
	static const SuggestedAction NO_CHANGE            {SuggestedAction::YawingNoChange,      SuggestedAction::FiringNo};
	static const SuggestedAction START_YAWING_CW      {SuggestedAction::YawingClockwise,     SuggestedAction::FiringNo};
	static const SuggestedAction START_YAWING_ACW     {SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringNo};
	static const SuggestedAction STOP_YAWING_AND_FIRE {SuggestedAction::YawingStop,          SuggestedAction::FiringTry};
}

/**
 * Information about the game that will not change while playing.
 */
struct GameInfo {
	sf::IntRect field_shape; //< rectangle showing bounds of the screen/playing field
	int ship_rotation_speed_millidegrees_per_frame; //< how fast the ship rotates when rotating
	int phaser_pulse_speed; //< magnitude of the velocity of a phasor pulse
	sf::Vector2i phaser_pulse_hitbox_size; //< width & height of a phasor pulse
};

/**
 * Provides an artificial intelligence that plays the game
 */
class GameAI {
public:
	GameAI(const GameInfo& game_info, sf::RenderTarget* debug_rt);
	~GameAI();

	GameAI(const GameAI&) = default;
	GameAI& operator=(const GameAI&) = default;

	/**
	 * Look at the current asteroids on the screen via `asteroidsObserver()`,
	 * and command the ship to do something (or nothing)
	 */
	SuggestedAction suggestAction(const ShipState& ship_state);

	/**
	 * Use this to draw graphics over the game, to debug your AI
	 */
	const sf::RenderTarget* debugRT() const { return debug_rt; }
	      sf::RenderTarget* debugRT()       { return debug_rt; }

	/**
	 * Is notified of events, and keeps track of
	 * the asteroids that are currently on the screen
	 */
	const AsteroidsObserver& asteroidsObserver() const { return asteroid_observer; }
	      AsteroidsObserver& asteroidsObserver()       { return asteroid_observer; }

	/**
	 * Accessor for the student-customizable state
	 */
	const MyAIData& customState() const { return *my_game_ai; }
	      MyAIData& customState()       { return *my_game_ai; }

private:
	GameInfo game_info;
	sf::RenderTarget* debug_rt;

	AsteroidsObserver asteroid_observer;

	MyAIData* my_game_ai;
};

inline std::ostream& operator<<(std::ostream& os, const SuggestedAction::Yawing& say) { switch (say) {
	case SuggestedAction::YawingClockwise:        return os << "YawingClockwise";
	case SuggestedAction::YawingAntiClockwise:    return os << "YawingAntiClockwise";
	case SuggestedAction::YawingStop:             return os << "YawingStop";
	case SuggestedAction::YawingNoChange:         return os << "YawingNoChange";
	default:                                      return os << (int)say;
}}

inline std::ostream& operator<<(std::ostream& os, const SuggestedAction::Firing& saf) { switch (saf) {
	case SuggestedAction::FiringTry:    return os << "FiringTry";
	case SuggestedAction::FiringNo:     return os << "FiringNo";
	default:                            return os << (int)saf;
}}

inline std::ostream& operator<<(std::ostream& os, const SuggestedAction& sa) { return os << "{ " << sa.yawing << ", " << sa.firing << " }"; }

#endif /* ECE244_GALAXY_EXPLORER_GAME_AI_HPP */
