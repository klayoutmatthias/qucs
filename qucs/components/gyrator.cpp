/***************************************************************************
                          gyrator.cpp  -  description
                             -------------------
    begin                : Sat Aug 23 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gyrator.h"


Gyrator::Gyrator()
{
  Description = QObject::tr("gyrator (impedance inverter)");

  Arcs.push_back(Arc(  3, -9, 18, 18, 16*90, 16*180,QPen(Qt::darkBlue,2)));
  Arcs.push_back(Arc(-21, -9, 18, 18,16*270, 16*180,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line(-30,-30,-12,-30,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-30, 30,-12, 30,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 12,-30, 30,-30,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 12, 30, 30, 30,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line( 12,-30, 12, 30,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-12,-30,-12, 30,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line(-22,-22, 22,-22,QPen(Qt::darkGray,1)));
  Lines.push_back(Line( 22,-22, 22, 22,QPen(Qt::darkGray,1)));
  Lines.push_back(Line( 22, 22,-22, 22,QPen(Qt::darkGray,1)));
  Lines.push_back(Line(-22, 22,-22,-22,QPen(Qt::darkGray,1)));

  Ports.push_back(Port(-30,-30));
  Ports.push_back(Port( 30,-30));
  Ports.push_back(Port( 30, 30));
  Ports.push_back(Port(-30, 30));

  x1 = -30; y1 = -30;
  x2 =  30; y2 =  30;

  tx = x1+4;
  ty = y2+4;
  Model = "Gyrator";
  Name  = "X";

  Props.push_back(Property("R", "50 Ohm", true,
		QObject::tr("gyrator ratio")));
  Props.push_back(Property("Zref", "50 Ohm", false,
		QObject::tr("reference impedance")));
}

Gyrator::~Gyrator()
{
}

Component* Gyrator::newOne()
{
  return new Gyrator();
}

Element* Gyrator::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Gyrator");
  BitmapFile = (char *) "gyrator";

  if(getNewOne)  return new Gyrator();
  return 0;
}
