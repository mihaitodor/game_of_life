#ifndef IVIEW_H
#define IVIEW_H

#include "icontroller.h"

class IView {
public:
	virtual ~IView() = 0 {}

	virtual void SetObserver(std::shared_ptr<IController>) = 0;

	virtual bool GetWrapAroundEdges() const = 0;

	virtual void Show() = 0;

	virtual void RenderModel() = 0;
};

#endif//IVIEW_H