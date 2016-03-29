#include "gtest/gtest.h"
#include "model.h"
#include "iview.h"
#include "controller.h"

class ViewMock : public IView {
public:
	ViewMock(std::shared_ptr<IModel> a_model, bool a_wrapAroundEdges) : wrapAroundEdges(a_wrapAroundEdges){}

	void SetObserver(std::shared_ptr<IController>) override {}

	bool GetWrapAroundEdges() const override { return wrapAroundEdges; }

	void Show() override {}

	void RenderModel() override {}

private:
	bool wrapAroundEdges;
};

class ControllerTextFixtureBase : public ::testing::Test {
public:
	ControllerTextFixtureBase(bool a_wrapAroundEdges) : wrapAroundEdges(a_wrapAroundEdges) {}

	void SetUp() override {
		model = std::make_shared<Model>(modelWidth, modelHeight);
		controller = Controller::Create(model, std::make_unique<ViewMock>(model, wrapAroundEdges));
	}

	void ValidateModel(const IModel::GridType &expectedState) {
		for (size_t i = 0; i < model->GetHeight(); ++i) {
			for (size_t j = 0; j < model->GetWidth(); ++j) {
				EXPECT_EQ(expectedState[i][j], model->IsElementAtSet(i, j)) << "For i = " << i << "; j = " << j;
			}
		}
	}

protected:
	bool wrapAroundEdges;

	std::shared_ptr<Model> model;
	std::shared_ptr<IController> controller;

private:
	static constexpr size_t modelWidth = 5;
	static constexpr size_t modelHeight = 5;
};

class ControllerTextFixtureWrapAroundEdges : public ControllerTextFixtureBase {
public:
	ControllerTextFixtureWrapAroundEdges() : ControllerTextFixtureBase(true) {}
};

class ControllerTextFixtureNoWrapAroundEdges : public ControllerTextFixtureBase {
public:
	ControllerTextFixtureNoWrapAroundEdges() : ControllerTextFixtureBase(false) {}
};

TEST_F(ControllerTextFixtureNoWrapAroundEdges, CenterWithOneNeighbourSet)
{
	model->SetElementAt(1, 1);
	model->SetElementAt(2, 2);

	ASSERT_EQ(1, controller->CountLiveNeighbours(2, 2));
}

TEST_F(ControllerTextFixtureNoWrapAroundEdges, CenterWithAllNeighboursSet)
{
	model->SetElementAt(1, 1);
	model->SetElementAt(1, 2);
	model->SetElementAt(1, 3);
	model->SetElementAt(2, 1);
	model->SetElementAt(2, 3);
	model->SetElementAt(3, 1);
	model->SetElementAt(3, 2);
	model->SetElementAt(3, 3);

	ASSERT_EQ(8, controller->CountLiveNeighbours(2, 2));
}

TEST_F(ControllerTextFixtureWrapAroundEdges, CenterWithTwoNeighboursSet)
{
	model->SetElementAt(2, 1);
	model->SetElementAt(1, 2);
	model->SetElementAt(2, 2);

	ASSERT_EQ(2, controller->CountLiveNeighbours(2, 2));
}

TEST_F(ControllerTextFixtureNoWrapAroundEdges, TopLeftWithTwoNeighboursSet)
{
	model->SetElementAt(1, 1);
	model->SetElementAt(1, 0);
	model->SetElementAt(4, 4);

	ASSERT_EQ(2, controller->CountLiveNeighbours(0, 0));
}

TEST_F(ControllerTextFixtureWrapAroundEdges, TopLeftWithTwoNeighboursSet)
{
	model->SetElementAt(1, 1);
	model->SetElementAt(4, 4);

	ASSERT_EQ(2, controller->CountLiveNeighbours(0, 0));
}

TEST_F(ControllerTextFixtureNoWrapAroundEdges, BottomRightWithTwoNeighboursSet)
{
	model->SetElementAt(0, 0);
	model->SetElementAt(4, 3);
	model->SetElementAt(3, 3);

	ASSERT_EQ(2, controller->CountLiveNeighbours(4, 4));
}

TEST_F(ControllerTextFixtureWrapAroundEdges, BottomRightWithTwoNeighboursSet)
{
	model->SetElementAt(0, 0);
	model->SetElementAt(3, 3);

	ASSERT_EQ(2, controller->CountLiveNeighbours(4, 4));
}

TEST_F(ControllerTextFixtureNoWrapAroundEdges, TopLeftStillBlock)
{
	model->SetElementAt(0, 0);
	model->SetElementAt(0, 1);
	model->SetElementAt(1, 0);
	model->SetElementAt(1, 1);

	controller->ComputeNextIteration();

	IModel::GridType expectedState(model->GetHeight(), IModel::RowType(model->GetWidth()));

	expectedState[0][0] = true;
	expectedState[0][1] = true;
	expectedState[1][0] = true;
	expectedState[1][1] = true;

	ValidateModel(expectedState);
}

TEST_F(ControllerTextFixtureWrapAroundEdges, TopLeftStillBlock)
{
	model->SetElementAt(0, 0);
	model->SetElementAt(0, 1);
	model->SetElementAt(1, 0);
	model->SetElementAt(1, 1);

	controller->ComputeNextIteration();

	IModel::GridType expectedState(model->GetHeight(), IModel::RowType(model->GetWidth()));

	expectedState[0][0] = true;
	expectedState[0][1] = true;
	expectedState[1][0] = true;
	expectedState[1][1] = true;

	ValidateModel(expectedState);
}

TEST_F(ControllerTextFixtureNoWrapAroundEdges, TopLeftOscillatorBlinker)
{
	model->SetElementAt(0, 0);
	model->SetElementAt(0, 1);
	model->SetElementAt(0, 2);

	controller->ComputeNextIteration();

	IModel::GridType expectedState(model->GetHeight(), IModel::RowType(model->GetWidth()));

	expectedState[0][1] = true;
	expectedState[1][1] = true;

	ValidateModel(expectedState);
}

TEST_F(ControllerTextFixtureWrapAroundEdges, TopLeftOscillatorBlinker)
{
	model->SetElementAt(0, 0);
	model->SetElementAt(0, 1);
	model->SetElementAt(0, 2);

	controller->ComputeNextIteration();

	IModel::GridType expectedState(model->GetHeight(), IModel::RowType(model->GetWidth()));

	expectedState[0][1] = true;
	expectedState[1][1] = true;
	expectedState[4][1] = true;

	ValidateModel(expectedState);
}