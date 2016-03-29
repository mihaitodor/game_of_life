#ifndef MODEL_H
#define MODEL_H

#include "imodel.h"

class Model : public IModel {
public:
	Model() : state(defaultHeight, RowType(defaultWidth)) {}

	Model(size_t width, size_t height) : state(width, RowType(height)) {}

	size_t GetWidth() const override {
		return state.empty() ? 0 : state[0].size();
	}

	size_t GetHeight() const override {
		return state.size();
	}

	void SetElementAt(size_t x, size_t y) override {
		state[x][y] = true;
	}

	void UnsetElementAt(size_t x, size_t y) override {
		state[x][y] = false;
	}

	void InvertElementAt(size_t x, size_t y) override {
		state[x][y] = !state[x][y];
	}

	bool IsElementAtSet(size_t x, size_t y) override {
		return state[x][y];
	}

	void SetState(const GridType &newState) override {
		state = newState;
	}

	void Randomize() override;
	
	void Reset() override {
		state = GridType(GetHeight(), RowType(GetWidth()));
	}

private:
	static constexpr size_t defaultWidth = 50;
	static constexpr size_t defaultHeight = 50;

	GridType state;

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
};

#endif//MODEL_H