/***************************************************************************
                               msvia.cpp
                              -----------
    begin                : Sat Oct 30 2004
    copyright            : (C) 2004 by Michael Margraf
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

#include "msvia.h"
#include "node.h"


MSvia::MSvia()
{
  Description = QObject::tr("microstrip via");

  Arcs.push_back(Arc(-5,-4, 10,  7,  0, 16*360,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-20,  0, -5,  0,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( -5,  0, -5, 14,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  5,  0,  5, 14,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-11, 14, 11, 14,QPen(Qt::darkBlue,3)));
  Lines.push_back(Line( -7, 20,  7, 20,QPen(Qt::darkBlue,3)));
  Lines.push_back(Line( -3, 26,  3, 26,QPen(Qt::darkBlue,3)));

  Ports.push_back(Port(-20,  0));

  x1 = -20; y1 = -7;
  x2 =  14; y2 = 30;

  tx = 20;
  ty = 0;
  Model = "MVIA";
  Name  = "MS";

  Props.push_back(Property("Subst", "Subst1", true,
		QObject::tr("substrate")));
  Props.push_back(Property("D", "1 mm", true,
		QObject::tr("diameter of round via conductor")));
  Props.push_back(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
}

MSvia::~MSvia()
{
}

// -------------------------------------------------------
Component* MSvia::newOne()
{
  return new MSvia();
}

// -------------------------------------------------------
Element* MSvia::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Microstrip Via");
  BitmapFile = (char *) "msvia";

  if(getNewOne)  return new MSvia();
  return 0;
}

// -------------------------------------------------------
QString MSvia::netlist()
{
  QString s = Model+":"+Name;

  // output node name and add ground node
  s += " " + Ports.front().getConnection()->Name + " gnd";

  // output all properties
  for(auto p2 = Props.begin(); p2 != Props.end(); ++p2)
    s += " "+p2->Name+"=\""+p2->Value+"\"";

  return s + '\n';
}
