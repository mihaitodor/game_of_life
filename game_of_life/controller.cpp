#include "controller.h"

std::shared_ptr<IController> Controller::Create(std::shared_ptr<IModel> model, std::unique_ptr<IView> view) {
	//Can't use std::make_shared due to visibility rules :(
	auto controller = std::shared_ptr<Controller>(new Controller(model, std::move(view)));

	controller->Init();

	return controller;
}

size_t Controller::CountLiveNeighbours(size_t x, size_t y) {
	std::vector<size_t> horizontalNeighbourIndexes;
	std::vector<size_t> verticalNeighbourIndexes;

	if (view->GetWrapAroundEdges()) {
		horizontalNeighbourIndexes.push_back(x == 0 ? model->GetWidth() - 1 : x - 1);
		horizontalNeighbourIndexes.push_back(x);
		horizontalNeighbourIndexes.push_back(x == model->GetWidth() - 1 ? 0 : x + 1);

		verticalNeighbourIndexes.push_back(y == 0 ? model->GetHeight() - 1 : y - 1);
		verticalNeighbourIndexes.push_back(y);
		verticalNeighbourIndexes.push_back(y == model->GetHeight() - 1 ? 0 : y + 1);
	}
	else {
		if (x != 0) {
			horizontalNeighbourIndexes.push_back(x - 1);
		}

		horizontalNeighbourIndexes.push_back(x);

		if (x != model->GetWidth() - 1) {
			horizontalNeighbourIndexes.push_back(x + 1);
		}

		if (y != 0) {
			verticalNeighbourIndexes.push_back(y - 1);
		}

		verticalNeighbourIndexes.push_back(y);

		if (y != model->GetHeight() - 1) {
			verticalNeighbourIndexes.push_back(y + 1);
		}
	}

	int aliveNeighbourCount = 0;
	for (auto i : horizontalNeighbourIndexes) {
		for (auto j : verticalNeighbourIndexes) {
			if (i != x || j != y) {
				if (model->IsElementAtSet(i, j)) {
					++aliveNeighbourCount;
				}
			}
		}
	}

	return aliveNeighbourCount;
}

void Controller::ComputeNextIteration() {
	// Temporary buffer for the next state.
	IModel::GridType tempState(model->GetHeight(), IModel::RowType(model->GetWidth()));

	for (size_t y = 0; y < model->GetHeight(); y++) {
		for (size_t x = 0; x < model->GetWidth(); x++) {
			auto aliveNeighbours = CountLiveNeighbours(x, y);
			auto isCellDead = model->IsElementAtSet(x, y);

			// A live cell dies unless it has exactly 2 or 3 live neighbors
			// A dead cell remains dead unless it has exactly 3 live neighbors
			tempState[x][y] = isCellDead && (aliveNeighbours == 2 || aliveNeighbours == 3) || !isCellDead && aliveNeighbours == 3;
		}
	}

	model->SetState(tempState);

	view->RenderModel();
}