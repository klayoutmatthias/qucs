/***************************************************************************
                               am_modulator.cpp
                              ------------------
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

#include "am_modulator.h"


AM_Modulator::AM_Modulator()
{
  Description = QObject::tr("ac voltage source with amplitude modulator");

  Arcs.push_back(Arc(-12,-12, 24, 24,     0, 16*360,QPen(Qt::darkBlue,2)));
  Arcs.push_back(Arc( -7, -4,  7,  7,     0, 16*180,QPen(Qt::darkBlue,2)));
  Arcs.push_back(Arc(  0, -4,  7,  7,16*180, 16*180,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0, 30,  0, 12,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0,-30,  0,-12,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  5,-18, 11,-18,QPen(Qt::red,1)));
  Lines.push_back(Line(  8,-21,  8,-15,QPen(Qt::red,1)));
  Lines.push_back(Line(  5, 18, 11, 18,QPen(Qt::black,1)));

  Lines.push_back(Line(-12,  0,-30,  0,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-12,  0,-17,  5,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-12,  0,-17, -5,QPen(Qt::darkBlue,2)));
  Texts.push_back(Text(-30,-22, QObject::tr("AM"), Qt::black, 10.0,1.0,0.0));

  Ports.push_back(Port(  0,-30));
  Ports.push_back(Port(  0, 30));
  Ports.push_back(Port(-30,  0));

  x1 = -30; y1 = -30;
  x2 =  14; y2 =  30;

  tx = x2+4;
  ty = y1+4;
  Model = "AM_Mod";
  Name  = "V";

  Props.push_back(Property("U", "1 V", true,
		QObject::tr("peak voltage in Volts")));
  Props.push_back(Property("f", "1 GHz", false,
		QObject::tr("frequency in Hertz")));
  Props.push_back(Property("Phase", "0", false,
		QObject::tr("initial phase in degrees")));
  Props.push_back(Property("m", "1.0", false,
		QObject::tr("modulation level")));
}

AM_Modulator::~AM_Modulator()
{
}

Component* AM_Modulator::newOne()
{
  return new AM_Modulator();
}

Element* AM_Modulator::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("AM modulated Source");
  BitmapFile = (char *) "am_mod";

  if(getNewOne)  return new AM_Modulator();
  return 0;
}
