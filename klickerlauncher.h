/*
 * Copyright (C) 2008-2009 Sacha Schutz <istdasklar@free.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
/*
 * klickerlauncher.h
 *
 *  Created on: 22 janv. 2009
 *      Author: schutz
 */

#ifndef KLICKERLAUNCHER_H_
#define KLICKERLAUNCHER_H_

#include <KIcon>
#include <KLineEdit>
#include <KPushButton>
#include <KPushButton>
#include <QGraphicsLinearLayout>
#include <QtNetwork>
#include <Plasma/Applet>
#include <Plasma/Svg>
#include <Plasma/IconWidget>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <Plasma/PopupApplet>
#include <plasma/widgets/pushbutton.h>
#include <QGraphicsSceneMouseEvent>
#include <KConfigDialog>
#include <KProcess>
#include <QTimeLine>
#include "configform.h"
//By Vlad
#include <Plasma/ToolTipManager> 
#include <QCheckBox>
#include <QSlider>
//////
class QSizeF;

class KlickerLauncher: public Plasma::Applet {
	Q_OBJECT
public:
	KlickerLauncher(QObject *parent, const QVariantList &args);
	~KlickerLauncher();

	void paintInterface(QPainter *painter,
	        const QStyleOptionGraphicsItem *option, const QRect& contentsRect);
	void init();
	void constraintsEvent(Plasma::Constraints constraints);
        void createConfigurationInterface(KConfigDialog *parent);
        void  hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
        void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
        void mousePressEvent ( QGraphicsSceneMouseEvent * event );
        
public slots:
	void changeFactor(int f){m_factor = float(f)/100;update();}
	void configAccepted();
	void writeData();
	void readData();


private:
        KIcon m_icon;
        QString m_iconName;
        float m_factor;
        QTimeLine *m_timer;
	ConfigForm * m_configWidget;
        KProcess * m_process;
        int m_duration;
	//By Vlad
	Plasma::ToolTipContent tooltip_data;
	Qt::CheckState tooltip_active;
	int m_infactor;
	//////


};
K_EXPORT_PLASMA_APPLET(klicker,KlickerLauncher);
#endif /* KLICKERLAUNCHER_H_ */
