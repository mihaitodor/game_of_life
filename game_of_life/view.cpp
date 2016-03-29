#include "view.h"

View::View(std::shared_ptr<IModel> a_model) :
			model(a_model),
			window(nana::API::make_center(windowWidth, windowHeight)),
			place(window),
			wrapAroundEdgesCheckbox(window),
			startStopButton(window),
			randomizeButton(window),
			clearButton(window),
			exitButton(window),
			gameCells(model->GetHeight()) {
	window.caption("Game of Life");

	wrapAroundEdgesCheckbox.caption("Wrap around edges");

	startStopButton.caption("Start");
	startStopButton.events().click([this](){ StartAnimation(); });

	randomizeButton.caption("Randomize");
	randomizeButton.events().click([this]() {
												model->Randomize();
												RenderModel();
											});

	clearButton.caption("Clear");
	clearButton.events().click([this]() {
											model->Reset();
											RenderModel();
										});

	exitButton.caption("Exit");
	exitButton.events().click(nana::API::exit);

	std::string windowLayout = std::string("<vertical <gap=2 grid=[") +
		std::to_string(model->GetHeight()) + ',' + std::to_string(model->GetWidth()) +
		"] margin=10 game><vertical weight=10% gap=10 margin=10 <controls><buttons>>>";

	place.div(windowLayout.c_str());
	place.field("controls") << wrapAroundEdgesCheckbox;
	place.field("buttons") << startStopButton << randomizeButton << clearButton << exitButton;

	for (size_t x = 0; x < model->GetHeight(); ++x) {
		gameCells[x].reserve(model->GetWidth());
		for (size_t y = 0; y < model->GetWidth(); ++y) {
			gameCells[x].emplace_back(std::make_shared<GameCellType>(window, true));
			gameCells[x].back()->bgcolor(nana::colors::black);
			gameCells[x].back()->events().click([this, x, y](){ CellClicked(x, y); });

			place.field("game") << *gameCells[x].back();
		}
	}

	place.collocate();

	animationTimer.interval(animationSpeedMililseconds);
	animationTimer.elapse([this]()
	{
		observer.lock()->ComputeNextIteration();
	});
}

bool View::GetWrapAroundEdges() const {
	return wrapAroundEdgesCheckbox.checked();
}

void View::Show() {
	RenderModel();

	window.show();
	nana::exec();
}

void View::RenderModel() {
	for (size_t x = 0; x < model->GetHeight(); ++x) {
		for (size_t y = 0; y < model->GetWidth(); ++y)
		{
			auto colour = model->IsElementAtSet(x, y) ? aliveCellColour : deadCellColour;

			if (gameCells[x][y]->bgcolor() != colour) {
				gameCells[x][y]->bgcolor(colour);
			}
		}
	}
}

void View::CellClicked(size_t x, size_t y) {
	model->InvertElementAt(x, y);

	gameCells[x][y]->bgcolor(GetElementColour(x, y));
}

void View::StartAnimation() {
	wrapAroundEdgesCheckbox.enabled(false);

	startStopButton.caption("Stop");
	startStopButton.events().click.clear();
	startStopButton.events().click([this](){ StopAnimation(); });

	animationTimer.start();
}

void View::StopAnimation() {
	startStopButton.caption("Start");
	startStopButton.events().click.clear();
	startStopButton.events().click([this](){ StartAnimation(); });

	wrapAroundEdgesCheckbox.enabled(true);

	animationTimer.stop();
}