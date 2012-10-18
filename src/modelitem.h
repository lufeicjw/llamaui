#ifndef MODELITEM_H
#define MODELITEM_H

#include <QHash>
#include <QObject>

class ModelItem
{
public:
	ModelItem()
	{
	}

	virtual ~ModelItem()
	{
	}

	virtual QString id() const = 0;
	virtual QVariant data( int role ) const = 0;
};

#endif // MODELITEM_H
