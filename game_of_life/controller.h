#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "imodel.h"
#include "iview.h"
#include "icontroller.h"

class Controller : public IController {
public:
	static std::shared_ptr<IController> Create(std::shared_ptr<IModel> model, std::unique_ptr<IView> view);

	void ShowView() override {
		view->Show();
	}

	size_t CountLiveNeighbours(size_t x, size_t y) override;

	void ComputeNextIteration() override;

private:
	std::shared_ptr<IModel> model;
	std::unique_ptr<IView> view;

	Controller(std::shared_ptr<IModel> a_model, std::unique_ptr<IView> a_view) :
				model(a_model),
				view(std::move(a_view)) {}

	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;

	void Init() {
		view->SetObserver(shared_from_this());
	}
};

#endif//CONTROLLER_H