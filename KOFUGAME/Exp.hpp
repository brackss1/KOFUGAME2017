#pragma once

#include "Model.hpp"

class Exp : public Scene {

	static TextureRegion background;

public:

	void init() override {

		background = Texture(L"pictures/exp").resize(Window::Width(), Window::Height());
	}

	void draw() const {

		background.drawAt(Window::Center());
	}

	void update() override {

		if (KeyZ.down)
			Game::changeScene(State::Rule);
	}

};