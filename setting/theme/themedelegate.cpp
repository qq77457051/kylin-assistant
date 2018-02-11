/*
 * Copyright (C) 2013 ~ 2018 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    xiangli@ubuntukylin.com/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "themedelegate.h"

#include <QPainter>
#include <QDebug>

namespace {

const int THEME_IMAGE_MARGIN = 10;

}

ThemeDelegate::ThemeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_textColor(QColor("rgba(0,153,68,0.3)"))
{
//"#009944" "#e60012"
}

QWidget *ThemeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ThemeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void ThemeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void ThemeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QList<QVariant> datas = index.model()->data(index, Qt::DisplayRole).toList();
    if (!datas.isEmpty()) {
        // Draw background
        drawBackground(option, painter);//or draw shadow effect:   "://res/theme-cover.png"

        // Draw theme image
        drawImage(option, index, painter);

        // Draw title text
        drawTitle(option, index, painter);
    }
}

QSize ThemeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    return index.model()->data(index, Qt::SizeHintRole).toSize();
}

void ThemeDelegate::drawBackground(const QStyleOptionViewItem &option, QPainter *painter) const
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRect rect = option.rect;
    rect.setSize(QSize(rect.width() - 1, rect.height() - 1));

    QColor bgColor;
    QColor borderColor;
    if ((option.state & QStyle::State_MouseOver) && (option.state & QStyle::State_Selected) == 0) {
        bgColor = QColor("#e0f4f9");
        borderColor = QColor("#999999");
    }
    else if (option.state & QStyle::State_Selected) {
        bgColor = QColor("#2bb6ea");
        borderColor = QColor("#e0f4f9");
    }
    else {
        bgColor = QColor("#999999");
        borderColor = QColor("#ffffff");
    }

    QPainterPath path;
    path.addRoundedRect(rect.x(), rect.y(), rect.width(), rect.height(), 3, 4);
    painter->fillPath(path, QBrush(bgColor));
    QPen borderPen(QBrush(borderColor), 1);
    painter->setPen(borderPen);
    painter->drawPath(path);
}

void ThemeDelegate::drawImage(const QStyleOptionViewItem &option, const QModelIndex& index, QPainter *painter) const
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRect rect = option.rect;
    const int pixmapSize = rect.width() - THEME_IMAGE_MARGIN * 2;

    QList<QVariant> datas = index.model()->data(index, Qt::DisplayRole).toList();
    //QString data = index.data().toString();

    const QString name = datas[0].toString();
    const QString picture = datas[1].toString();
    const QString defaultPic = datas[2].toString();
    bool isCurrent = datas[3].toBool();

    // Theme image
    QSize imageSize;
    imageSize.setWidth(option.rect.width() - THEME_IMAGE_MARGIN * 2);
    imageSize.setHeight(imageSize.width());

    /*if ((option.state & QStyle::State_MouseOver) && (option.state & QStyle::State_Selected) == 0) {
    }
    else if (option.state & QStyle::State_Selected) {
    }*/
    QPixmap pixmap = QPixmap(picture);
    if (!pixmap.isNull())
        pixmap = pixmap.scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    else
        pixmap = QPixmap(defaultPic).scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPixmap scalePixmap = pixmap.scaled(pixmapSize, pixmapSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter->drawPixmap(rect.x() + THEME_IMAGE_MARGIN, rect.y() + THEME_IMAGE_MARGIN, pixmapSize, pixmapSize, scalePixmap);

    if (isCurrent) {
        painter->drawPixmap(rect.x() + 2*THEME_IMAGE_MARGIN, rect.y() + 2*THEME_IMAGE_MARGIN, 22, 22, QPixmap("://res/choose.png"));
    }
}

void ThemeDelegate::drawTitle(const QStyleOptionViewItem &option, const QModelIndex &index, QPainter *painter) const
{
    painter->setRenderHint(QPainter::Antialiasing, false);

    QRect rect = option.rect;
    QFont font;
    font.setPixelSize(12);
    QPen pen(m_textColor);

    QList<QVariant> datas = index.model()->data(index, Qt::DisplayRole).toList();
//        QString data = index.data().toString();

    QString name = datas[0].toString();
    painter->setPen(pen);

    QFont textFont(painter->font());
    textFont.setPixelSize(12);
    textFont.setWeight(24);
    const QFontMetrics fm(textFont);
    QSize textSize(qMin(fm.width(name) + 18, rect.width()), fm.height() + 2);

    const int textHeight = rect.height() * 0.78;
    const QRectF textRect = QRect(rect.x(), rect.y() + textHeight + 5, rect.width(), rect.height() - textHeight);

    // Draw theme name text background
    /*QRect bgRect(rect.x() + (rect.width() - textSize.width()) / 2, textRect.y()+ (textRect.height() - textSize.height()) / 2, textSize.width(), textSize.height());
    if (option.state & QStyle::State_Selected) {
        QPainterPath textPainterPath;
        textPainterPath.addRoundedRect(bgRect, 2, 2);
        painter->fillPath(textPainterPath, QBrush(QColor("#2ca7f8")));
    }*/

    painter->setFont(textFont);
    painter->drawText(textRect, fm.elidedText(name, Qt::ElideMiddle, rect.width()), QTextOption(Qt::AlignCenter));
}