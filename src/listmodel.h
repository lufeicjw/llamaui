#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QList>

class ModelItem;

class ListModel
:	public QAbstractListModel
{
Q_OBJECT

public:
	explicit ListModel( QObject* parent = 0 );

	void addItem( ModelItem* item );
	void addItems( const QList< ModelItem* >& items );
	void clear();

	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

	ModelItem* itemAt( int index ) const;

private:
	QList< ModelItem* > _list;
};

#endif // LISTMODEL_H
