/***************************************************************************
                                switch.cpp
                               ------------
    begin                : Sat Feb 25 2006
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
#include "switch.h"
#include "node.h"
#include "schematic.h"


Switch::Switch()
{
  Description = QObject::tr("switch (time controlled)");

  Props.append(Property("init", "off", false,
		QObject::tr("initial state")+" [on, off]"));
  Props.append(Property("time", "1 ms", false,
		QObject::tr("time when state changes (semicolon separated list possible, even numbered lists are repeated)")));
  Props.append(Property("Ron", "0", false,
		QObject::tr("resistance of \"on\" state in ohms")));
  Props.append(Property("Roff", "1e12", false,
		QObject::tr("resistance of \"off\" state in ohms")));
  Props.append(Property("Temp", "26.85", false,
		QObject::tr("simulation temperature in degree Celsius")));
  Props.append(Property("MaxDuration", "1e-6", false,
		QObject::tr("Max possible switch transition time (transition time 1/100 smallest value in 'time', or this number)")));
  Props.append(Property("Transition", "spline", false,
		QObject::tr("Resistance transition shape")+" [abrupt, linear, spline]"));

  createSymbol();
  tx = x1+4;
  ty = y2+4;
  Model = "Switch";
  Name  = "S";
}

// -------------------------------------------------------
Component* Switch::newOne()
{
  Switch *p = new Switch();
  p->Props.first().Value = Props.first().Value;
  p->recreate(0);
  return p;
}

// -------------------------------------------------------
Element* Switch::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Switch");
  BitmapFile = (char *) "switch";

  if(getNewOne)  return new Switch();
  return 0;
}

// -------------------------------------------------------
QString Switch::netlist()
{
  QString s = Model+":"+Name;

  // output all node names
  s += " "+Ports.at(0).Connection->Name;
  s += " "+Ports.at(1).Connection->Name;

  // output all properties
  s += " "+Props[0].Name+"=\""+Props[0].Value+"\"";
  s += " "+Props[1].Name+"=\"["+Props[1].Value+"]\"";
  for(int i = 2; i < Props.count(); ++i)
    s += " "+Props[i].Name+"=\""+Props[i].Value+"\"";

  return s + '\n';
}

// -------------------------------------------------------
void Switch::createSymbol()
{
  if(Props.first().Value != "on") {
    Lines.append(Line(-15,  0, 15,-15,QPen(Qt::darkBlue,2)));
    y1 = -17;
  }
  else {
    Lines.append(Line(-15,  0, 16,-5,QPen(Qt::darkBlue,2)));
    y1 = -7;
  }

  Lines.append(Line(-30,  0,-15,  0,QPen(Qt::darkBlue,2)));
  Lines.append(Line( 17,  0, 30,  0,QPen(Qt::darkBlue,2)));
  Arcs.append(Arc( 12, -3, 5, 5, 0, 16*360,QPen(Qt::darkBlue,2)));
  Ellips.append(Area(-18, -3, 6, 6, QPen(Qt::darkBlue,2),
                QBrush(Qt::darkBlue, Qt::SolidPattern)));

  Ports.append(Port(-30,  0));
  Ports.append(Port( 30,  0));

  x1 = -30;
  x2 =  30; y2 =   7;
}
