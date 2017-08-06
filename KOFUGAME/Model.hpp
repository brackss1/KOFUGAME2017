#pragma once

#include "Base.hpp"

class Game {

	static std::unordered_map<State, std::unique_ptr<Scene>> scenes;

	static State state;

public:

	static void init() {

		state = State::Title;

		scenes.at(state)->init();
	}

	static void changeScene(State _state) {

		state = _state;

		scenes.at(state)->init();
	}

	static void update() {

		scenes.at(state)->update();
	}

	static void draw() {

		scenes.at(state)->draw();
	}

	template<class T>
	static auto addScene(State _state) -> std::enable_if_t<std::is_base_of_v<Scene,T>> {

		scenes.emplace(_state, std::make_unique<T>());
	}
};

State Game::state;

std::unordered_map<State, std::unique_ptr<Scene>> Game::scenes;