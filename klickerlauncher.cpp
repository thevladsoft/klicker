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
 *//*
 * klickerlauncher.cpp
 *
 *  Created on: 22 janv. 2009
 *      Author: schutz
 */

#include <KPushButton>
#include <KDebug>
#include <KStandardDirs>
#include "klickerlauncher.h"

int alt_dir=1;//1: the las thing it did was shrinking; 2: last thing was growing

KlickerLauncher::KlickerLauncher(QObject *parent, const QVariantList &args) :
	Plasma::Applet(parent, args)
{
  //By Vlad
  m_factor = 0.5;
  //m_infactor = 50;
  //m_factor = m_infactor/100.;
  
  m_iconName = "system-run.png";
  m_icon = KIcon(m_iconName);
  m_process = new KProcess;
  m_process->setProgram ("konqueror");
  m_duration= 500;
  m_timer = new QTimeLine(m_duration,this);
  
  m_timer->setFrameRange(50, 100); 
  //By Vlad
 // m_timer->setFrameRange(m_infactor, 100); 
  
setAcceptHoverEvents(true) ;
  setAcceptedMouseButtons ( Qt::LeftButton);
  setBackgroundHints(NoBackground);

  connect(m_timer, SIGNAL(frameChanged(int)),this, SLOT(changeFactor(int)));
  //resize(200,200);
}
//============================================================
KlickerLauncher::~KlickerLauncher() {
	if (hasFailedToLaunch()) {
		// Do some cleanup here
	} else {
		// Save settings
	}
}
//============================================================
void KlickerLauncher::init() {
 readData();
 
 //By Vlad
 changeFactor(m_infactor);
 
 //tooltip_data.setMainText(m_process->program().join(" "));
 if(tooltip_active==Qt::Checked){
   //Plasma::ToolTipManager::self()->setState(Plasma::ToolTipManager::Activated);
   tooltip_data.setMainText(m_process->program().join(" "));
 }else{
   //Plasma::ToolTipManager::self()->setState(Plasma::ToolTipManager::Inhibited);
   tooltip_data.setMainText("");
 }
 Plasma::ToolTipManager::self()->setContent(this, tooltip_data);
 //////
}
//=====================================================================================
void KlickerLauncher::paintInterface(QPainter *p,
const QStyleOptionGraphicsItem *option, const QRect &contentsRect) {
p->setRenderHint(QPainter::SmoothPixmapTransform);
p->setRenderHint(QPainter::Antialiasing);

float w = contentsRect.width()*m_factor;
float h = contentsRect.height()*m_factor ;    
float x=(contentsRect.width() - w )/2;
float y=(contentsRect.height() -h) / 2;


QImage img = m_icon.pixmap(w,h).toImage();
img.convertToFormat ( QImage::Format_Mono );
p->drawImage(x, y, img);

}

//=============================================================================
void KlickerLauncher::constraintsEvent(Plasma::Constraints constraints)
{
setBackgroundHints(NoBackground);
}
//=============================================================================
void KlickerLauncher:: hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
m_timer->setDirection ( QTimeLine::Forward );
if ( m_timer->state()==QTimeLine::NotRunning)
if(alt_dir==1)m_timer->start();
alt_dir=2;
}
//=============================================================================

void KlickerLauncher:: hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
  m_timer->setDirection ( QTimeLine::Backward );
  if ( m_timer->state()==QTimeLine::NotRunning)
  if(alt_dir==2)m_timer->start();
  alt_dir=1;
}
//============================================================================
 //it should be better a click event, not just press.
 void KlickerLauncher:: mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
  if ( event->button() == Qt::LeftButton)
  m_process->start();
  
  m_timer->setDirection ( QTimeLine::Backward );
  if ( m_timer->state()==QTimeLine::NotRunning)
  if(alt_dir==2)m_timer->start();
  alt_dir=1;
}
//============================================================================
void KlickerLauncher::createConfigurationInterface(KConfigDialog *parent)
{
    m_configWidget = new ConfigForm;
    m_configWidget->ui()->kiconbutton->setIcon(m_iconName);
    //By Vlad
    //m_configWidget->ui()->kurlrequester->setPath(m_process->program().at(0));
    m_configWidget->ui()->kurlrequester->setPath(m_process->program().join(" "));
    ///////
    
    m_configWidget->ui()->kintspinbox->setValue(m_duration);
    
    //By Vlad
    //tooltip_active=Qt::Unchecked;
    m_configWidget->ui()->checkbox->setCheckState(tooltip_active);
    
    m_configWidget->ui()->qslider->setValue(m_infactor/10);
    //m_configWidget->ui()->kintspinbox->setValue(3);
    //////

    parent->addPage(m_configWidget, i18n("klickers Seetings"), KStandardDirs::locate("icon", "oxygen/scalable/apps/accessories-text-editor.svgz"));
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}
//==========================================================================
void KlickerLauncher::configAccepted()
{
  m_iconName= m_configWidget->ui()->kiconbutton->icon();
  m_icon = KIcon(m_iconName);
  m_duration = m_configWidget->ui()->kintspinbox->value();
  //By Vlad
  tooltip_active=m_configWidget->ui()->checkbox->checkState();
  //m_process->setProgram(m_configWidget->ui()->kurlrequester->text());
  m_process->setProgram(m_configWidget->ui()->kurlrequester->text().split(" "));
  //////
  
  m_timer->setDuration(m_duration);
  writeData();
  
 
 //By Vlad
 //tooltip_data.setMainText(m_process->program().join(" "));
 if(tooltip_active==Qt::Checked){
   //Plasma::ToolTipManager::self()->setState(Plasma::ToolTipManager::Activated);
   tooltip_data.setMainText(m_process->program().join(" "));
 }else{
   //Plasma::ToolTipManager::self()->setState(Plasma::ToolTipManager::Inhibited);
   tooltip_data.setMainText("");
 }
 Plasma::ToolTipManager::self()->setContent(this, tooltip_data);
 m_infactor = m_configWidget->ui()->qslider->value()*10;
 m_timer->setFrameRange(m_infactor, 100);
 changeFactor(m_infactor);
 //////
}
//============================================================================
void KlickerLauncher::writeData()
{
   KConfigGroup cg = config();
   cg.writeEntry("icon", m_iconName);
   //By Vlad
   //cg.writeEntry("program",m_process->program().at(0));
   cg.writeEntry("program",m_process->program().join(" "));
   if(tooltip_active==Qt::Checked){
    cg.writeEntry("tooltip",1);
   }else{
    cg.writeEntry("tooltip",0);
   }
   cg.writeEntry("minFactor",m_infactor);
   //cg.writeEntry("tooltip",tooltip_active);
   //////
   
   cg.writeEntry("duration",m_duration);
}
//==============================================================================
void KlickerLauncher::readData()
{
    KConfigGroup cg = config();
    m_iconName = cg.readEntry("icon", "system-run");
    QString program = cg.readEntry("program", QString());
    m_duration = cg.readEntry("duration",500);
    m_icon = KIcon(m_iconName);
    //By Vlad
    //m_process->setProgram(program);
    m_process->setProgram(program.split(" "));
    //tooltip_active = cg.readEntry("tooltip",Qt::Unchecked);
    if(cg.readEntry("tooltip",1)){
     tooltip_active = Qt::Checked;
    }else{
     tooltip_active = Qt::Unchecked;
    }
    //m_infactor = cg.readEntry("minFactor",50);
    //m_infactor = 10;
    m_infactor = cg.readEntry("minFactor",50);
    //printf("- %d -",m_infactor);
    m_timer->setFrameRange(m_infactor, 100);
    
    //////
    
    m_timer->setDuration(m_duration);
}


#include "klickerlauncher.moc"
