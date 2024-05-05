/***************************************************************************
                                 triac.cpp
                                -----------
    begin                : Sun Dec 23 2007
    copyright            : (C) 2007 by Michael Margraf
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

#include "triac.h"


Triac::Triac()
{
  Description = QObject::tr("triac (bidirectional thyristor)");

  Lines.append(Line(  0,-30,  0, -6,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0, 30,  0,  6,QPen(Qt::darkBlue,2)));

  Lines.append(Line(-18,  6, 18,  6,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-18, -6, 18, -6,QPen(Qt::darkBlue,2)));
  Lines.append(Line( -9,  6,-18, -6,QPen(Qt::darkBlue,2)));
  Lines.append(Line( -9,  6,  0, -6,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  9, -6,  0,  6,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  9, -6, 18,  6,QPen(Qt::darkBlue,2)));

  Lines.append(Line(-13, 10, -9,  6,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-30, 10,-13, 10,QPen(Qt::darkBlue,2)));

  Ports.append(Port(  0,-30));
  Ports.append(Port(  0, 30));
  Ports.append(Port(-30, 10));

  x1 = -30; y1 = -30;
  x2 =  20; y2 =  30;

  tx = x2+4;
  ty = y1+4;
  Model = "Triac";
  Name  = "D";

  Props.append(Property("Vbo", "400 V", false,
	QObject::tr("(bidirectional) breakover voltage")));
  Props.append(Property("Igt", "50 uA", true,
	QObject::tr("(bidirectional) gate trigger current")));
  Props.append(Property("Cj0", "10 pF", false,
	QObject::tr("parasitic capacitance")));
  Props.append(Property("Is", "1e-10 A", false,
	QObject::tr("saturation current")));
  Props.append(Property("N", "2", false,
	QObject::tr("emission coefficient")));
  Props.append(Property("Ri", "10 Ohm", false,
	QObject::tr("intrinsic junction resistance")));
  Props.append(Property("Rg", "5 Ohm", false,
	QObject::tr("gate resistance")));
  Props.append(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature")));
}

Component* Triac::newOne()
{
  return new Triac();
}

Element* Triac::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Triac");
  BitmapFile = (char *) "triac";

  if(getNewOne)  return new Triac();
  return 0;
}
