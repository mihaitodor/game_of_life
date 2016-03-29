#include "model.h"
#include "view.h"
#include "controller.h"

int main()
{
	auto model = std::make_shared<Model>(58,58);
	auto view = std::make_unique<View>(model);
	auto controller = Controller::Create(model, std::move(view));

	controller->ShowView();

	return 0;
}