#pragma once

#include "Model.hpp"

class Game;

class Title : public Scene {

	Texture texture{ L"pictures/titleback.png" };

	const Font font{ Window::Height() / 4, Typeface::Light, FontStyle::Bold };

	double s;

public:

	Title() {

		texture.resize(Window::Size());
	}

	void init() override {

		Graphics::SetBackground(Palette::Gray);

		s = 5.0;
	}

	void update() override{

		if (s > 1.0)
			s -= ease();
		else if (KeyZ.down())
			Game::changeScene(State::Rule);

		Graphics2D::SetTransform(Mat3x2::Scale(s, Window::Center()));
	}

	void draw() const{

		//texture.drawAt(Window::Center());

		font(L"Geister").drawAt(Window::Center(),Palette::Bisque);
	}

private:

	double ease() {

		return s / 100;
	}
};