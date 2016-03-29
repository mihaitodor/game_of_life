#ifndef VIEW_H
#define VIEW_H

#include "iview.h"
#include "imodel.h"
#include "nana/gui/wvl.hpp"
#include "nana/gui/widgets/button.hpp"
#include "nana/gui/widgets/label.hpp"
#include "nana/gui/widgets/checkbox.hpp"
#include "nana/gui/place.hpp"
#include "nana/gui/timer.hpp"

class View : public IView {
public:
	View(std::shared_ptr<IModel> a_model);

	void SetObserver(std::shared_ptr<IController> a_observer) override {
		observer = a_observer;
	}

	bool GetWrapAroundEdges() const override;

	void Show() override;

	void RenderModel() override;

	void CellClicked(size_t x, size_t y);

	void StartAnimation();

	void StopAnimation();

	nana::colors GetElementColour(size_t x, size_t y) {
		return model->IsElementAtSet(x, y) ? aliveCellColour : deadCellColour;
	}

private:
	std::shared_ptr<IModel> model;
	std::weak_ptr<IController> observer;

	static constexpr unsigned windowWidth = 700;
	static constexpr unsigned windowHeight = 700;

	const nana::colors deadCellColour = nana::colors::silver;
	const nana::colors aliveCellColour = nana::colors::green;

	nana::form window;
	nana::place place;
	nana::checkbox wrapAroundEdgesCheckbox;
	nana::button startStopButton;
	nana::button randomizeButton;
	nana::button clearButton;
	nana::button exitButton;

	static constexpr unsigned animationSpeedMililseconds = 500;

	nana::timer animationTimer;

	typedef nana::label GameCellType;
	typedef std::vector<std::shared_ptr<GameCellType>> GameRowType;
	typedef std::vector<GameRowType> GameGridType;
	GameGridType gameCells;

	View(const View&) = delete;
	View& operator=(const View&) = delete;
};

#endif//VIEW_H