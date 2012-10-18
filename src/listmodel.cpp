#include "listmodel.h"

#include "modelitem.h"
#include <QtDebug>

ListModel::ListModel( QObject* parent )
:	QAbstractListModel( parent )
{
}

int ListModel::rowCount( const QModelIndex& /*parent*/ ) const
{
	return _list.count();
}

QVariant ListModel::data( const QModelIndex& index, int role ) const
{
	const int row = index.row();

	if ( row < 0 || row >= _list.count() )
		return QVariant();

	return _list.at( row )->data( role );
}

void ListModel::addItem( ModelItem* item )
{
	addItems( QList< ModelItem* >() << item );
}

void ListModel::addItems( const QList< ModelItem* >& items )
{
	_list.append( items );
}

void ListModel::clear()
{
	beginResetModel();

	foreach ( ModelItem* item, _list )
		delete item;

	_list.clear();
	endResetModel();
}

ModelItem* ListModel::itemAt( int index ) const
{
	if ( index < 0 || index >= _list.count() ) {
		qWarning() << "Index" << index << "out of range.";
		return 0;
	}

	return _list.at( index );
}
