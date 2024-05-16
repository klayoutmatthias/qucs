/***************************************************************************
                               bondwire.cpp
                              --------------
    begin                : Dec 22 2006
    copyright            : (C) 2006 by Michael Margraf
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

#include "bondwire.h"


BondWire::BondWire()
{
  Description = QObject::tr("bond wire");

  Lines.push_back(Line(-30, 0,-8, 0,QPen(Qt::darkBlue,3)));
  Lines.push_back(Line( 30, 0, 8, 0,QPen(Qt::darkBlue,3)));

  Arcs.push_back(Arc(-11,-10, 22, 26, 16*30,16*120,QPen(Qt::darkBlue,1)));
  Arcs.push_back(Arc(-19,-13, 10, 13,16*205,16*130,QPen(Qt::darkBlue,1)));
  Arcs.push_back(Arc(  9,-13, 10, 13,16*205,16*130,QPen(Qt::darkBlue,1)));

  Ports.push_back(Port(-30, 0));
  Ports.push_back(Port( 30, 0));

  x1 = -30; y1 =-13;
  x2 =  30; y2 =  5;

  tx = x1+4;
  ty = y2+4;
  Model = "BOND";
  Name  = "Line";

  Props.push_back(Property("L", "3 mm", true,
		QObject::tr("length of the wire")));
  Props.push_back(Property("D", "50 um", true,
		QObject::tr("diameter of the wire")));
  Props.push_back(Property("H", "2 mm", true,
		QObject::tr("height above ground plane")));
  Props.push_back(Property("rho", "0.022e-6", false,
		QObject::tr("specific resistance of the metal")));
  Props.push_back(Property("mur", "1", false,
		QObject::tr("relative permeability of the metal")));
  Props.push_back(Property("Model", "FREESPACE", false,
	QObject::tr("bond wire model")+" [FREESPACE, MIRROR, DESCHARLES]"));
  Props.push_back(Property("Subst", "Subst1", true,
		QObject::tr("substrate")));
  Props.push_back(Property("Temp", "26.85", false,
		QObject::tr("simulation temperature in degree Celsius")));
}

Component* BondWire::newOne()
{
  return new BondWire();
}

Element* BondWire::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Bond Wire");
  BitmapFile = (char *) "bondwire";

  if(getNewOne)  return new BondWire();
  return 0;
}
