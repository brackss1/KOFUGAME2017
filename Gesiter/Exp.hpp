#pragma once

#include "Model.hpp"

class Exp : public Scene {

	static std::array<TextureRegion, 4> exp;

	int cnt = 0;

public:

	Exp() {
		for (int i = 0; i < exp.size(); ++i) {
			exp[i] = TextureRegion(Texture(L"pictures/" + std::to_wstring(i) + L".png")).resize(Window::Width(), Window::Height());
		}
	}

	void init() override {

		cnt = 0;
	}

	void draw() const {

		exp.at(cnt).drawAt(Window::Center());
	}

	void update() override {

		if (KeyZ.down())
			++cnt;

		if (cnt == 4)
			Game::changeScene(State::Rule);
	}
};

std::array<TextureRegion, 4> Exp::exp;