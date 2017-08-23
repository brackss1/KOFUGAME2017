#pragma once

#include "Model.hpp"

class Result : public Scene {

	Font font;

	double s;

public:

	void init() override {

		s = 5.0;

		font = Font(Window::Height() / 4);
	}

	void draw() const override{

		if (winner == Turn::Player)
			font(L"You win!").drawAt(Window::Center(), Palette::Gold);
		else
			font(L"You lose!").drawAt(Window::Center(), Palette::Gold);
	}

	void update() override{

		if (s > 0.0)
			s -= 0.1;
		else if (KeyZ.down())
			Game::changeScene(State::Title);


		Graphics2D::SetTransform(Mat3x2::Rotate(s*0.1, Window::Center()));
	}
};