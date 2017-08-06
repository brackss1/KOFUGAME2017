
#include "Title.hpp"
#include "Rule.hpp"
#include "Play.hpp"
#include "Result.hpp"

void Main() {

	Game::addScene<Title>(State::Title);

	Game::addScene<Rule>(State::Rule);

	Game::addScene<Play>(State::Play);

	Game::addScene<Result>(State::Result);

	Game::init();


	while (System::Update())
	{
		Game::update();

		Game::draw();
	}
}