#ifndef IMODEL_H
#define IMODEL_H

#include <vector>

class IModel {
public:
	typedef std::vector<bool> RowType;
	typedef std::vector<RowType> GridType;

	virtual ~IModel() = 0 {}

	virtual size_t GetWidth() const = 0;

	virtual size_t GetHeight() const = 0;

	virtual void SetElementAt(size_t, size_t) = 0;

	virtual void UnsetElementAt(size_t, size_t) = 0;

	virtual void InvertElementAt(size_t, size_t) = 0;

	virtual bool IsElementAtSet(size_t, size_t) = 0;

	virtual void SetState(const GridType &) = 0;

	virtual void Randomize() = 0;

	virtual void Reset() = 0;

};

#endif//IMODEL_H