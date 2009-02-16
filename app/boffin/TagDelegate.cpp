/***************************************************************************
 *   Copyright 2005-2009 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "TagDelegate.h"
#include "TagCloudModel.h"
#include <QPainter>
#include <math.h>

static const float k_exponentFactor = 0.4;
static const float k_factor = 9;
static const int k_margin = 4;

TagDelegate::TagDelegate( QObject* parent ) 
            : QAbstractItemDelegate( parent )
{

}

void 
TagDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{

    painter->save();
    painter->setRenderHint( QPainter::Antialiasing );
    QPen p = ((option.state & QStyle::State_MouseOver) ? QColor( 100, 100, 100 ): Qt::transparent);
    if( option.state & QStyle::State_Selected )
    {
        painter->setBrush( option.palette.highlight() );
        painter->setPen( option.palette.highlightedText().color() );
    }
    else
    {
        painter->setBrush( Qt::transparent );
    }
  
  
    p.setWidth( 3 );
    painter->setPen( p );

    painter->drawRoundedRect( option.rect.adjusted( k_margin, k_margin, -k_margin, -k_margin ), 10.0f, 10.0f );
    painter->restore();

    QFont f = option.font;
    f.setPointSize( k_factor * pow( f.pointSize(), k_exponentFactor * ( index.data( TagCloudModel::WeightRole ).value<float>() + 1 ) ));
    f.setWeight( 99 * index.data( TagCloudModel::WeightRole ).value<float>());
    painter->setFont( f );
    QFontMetrics fm( f );
    painter->drawText( option.rect.translated(  0, fm.descent() - (k_margin * 1.5)), Qt::AlignHCenter | Qt::AlignBottom , index.data().toString());
}


#include <QDebug>
QSize 
TagDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QFont f = option.font;
    f.setPointSize( k_factor * pow( f.pointSize(), k_exponentFactor * ( index.data( TagCloudModel::WeightRole ).value<float>() + 1 )));
    f.setWeight( 99 * index.data( TagCloudModel::WeightRole ).value<float>());
    QFontMetrics fm( f );
    const QSize fmSize = fm.size( Qt::TextSingleLine, index.data().toString() + "  " );
    return fmSize; 
}
