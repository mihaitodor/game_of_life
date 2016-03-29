#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <memory>

class IController : public std::enable_shared_from_this<IController> {
public:
	~IController() {}

	virtual void ShowView() = 0;

	virtual size_t CountLiveNeighbours(size_t x, size_t y) = 0;

	virtual void ComputeNextIteration() = 0;
};

#endif//ICONTROLLER_H