/***************************************************************************
                          msline.cpp  -  description
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

#include "msline.h"


MSline::MSline()
{
  Description = QObject::tr("microstrip line");

  Lines.append(Line(-30,  0,-18,  0,QPen(Qt::darkBlue,2)));
  Lines.append(Line( 18,  0, 30,  0,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-13, -8, 23, -8,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-23,  8, 13,  8,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-13, -8,-23,  8,QPen(Qt::darkBlue,2)));
  Lines.append(Line( 23, -8, 13,  8,QPen(Qt::darkBlue,2)));

  Ports.append(Port(-30, 0));
  Ports.append(Port( 30, 0));

  x1 = -30; y1 =-11;
  x2 =  30; y2 = 11;

  tx = x1+4;
  ty = y2+4;
  Model = "MLIN";
  Name  = "MS";

  Props.append(Property("Subst", "Subst1", true,
	QObject::tr("name of substrate definition")));
  Props.append(Property("W", "1 mm", true,
	QObject::tr("width of the line")));
  Props.append(Property("L", "10 mm", true,
	QObject::tr("length of the line")));
  Props.append(Property("Model", "Hammerstad", false,
	QObject::tr("quasi-static microstrip model")+
		    " [Hammerstad, Wheeler, Schneider]"));
  Props.append(Property("DispModel", "Kirschning", false,
	QObject::tr("microstrip dispersion model")+" [Kirschning, Kobayashi, "
	"Yamashita, Hammerstad, Getsinger, Schneider, Pramanick]"));
  Props.append(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
}

MSline::~MSline()
{
}

Component* MSline::newOne()
{
  return new MSline();
}

Element* MSline::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Microstrip Line");
  BitmapFile = (char *) "msline";

  if(getNewOne)  return new MSline();
  return 0;
}
