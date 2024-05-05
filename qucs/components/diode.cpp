/***************************************************************************
                                diode.cpp
                               -----------
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

#include "diode.h"


Diode::Diode()
{
  Description = QObject::tr("diode");

  Props.append(Property("Is", "1e-15 A", true,
	QObject::tr("saturation current")));
  Props.append(Property("N", "1", true,
	QObject::tr("emission coefficient")));
  Props.append(Property("Cj0", "10 fF", true,
	QObject::tr("zero-bias junction capacitance")));
  Props.append(Property("M", "0.5", false,
	QObject::tr("grading coefficient")));
  Props.append(Property("Vj", "0.7 V", false,
	QObject::tr("junction potential")));
  Props.append(Property("Fc", "0.5", false,
	QObject::tr("forward-bias depletion capacitance coefficient")));
  Props.append(Property("Cp", "0.0 fF", false,
	QObject::tr("linear capacitance")));
  Props.append(Property("Isr", "0.0", false,
	QObject::tr("recombination current parameter")));
  Props.append(Property("Nr", "2.0", false,
	QObject::tr("emission coefficient for Isr")));
  Props.append(Property("Rs", "0.0 Ohm", false,
	QObject::tr("ohmic series resistance")));
  Props.append(Property("Tt", "0.0 ps", false,
	QObject::tr("transit time")));
  Props.append(Property("Ikf", "0", false,
	QObject::tr("high-injection knee current (0=infinity)")));
  Props.append(Property("Kf", "0.0", false,
	QObject::tr("flicker noise coefficient")));
  Props.append(Property("Af", "1.0", false,
	QObject::tr("flicker noise exponent")));
  Props.append(Property("Ffe", "1.0", false,
	QObject::tr("flicker noise frequency exponent")));
  Props.append(Property("Bv", "0", false,
	QObject::tr("reverse breakdown voltage")));
  Props.append(Property("Ibv", "1 mA", false,
	QObject::tr("current at reverse breakdown voltage")));
  Props.append(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
  Props.append(Property("Xti", "3.0", false,
	QObject::tr("saturation current temperature exponent")));
  Props.append(Property("Eg", "1.11", false,
	QObject::tr("energy bandgap in eV")));
  Props.append(Property("Tbv", "0.0", false,
	QObject::tr("Bv linear temperature coefficient")));
  Props.append(Property("Trs", "0.0", false,
	QObject::tr("Rs linear temperature coefficient")));
  Props.append(Property("Ttt1", "0.0", false,
	QObject::tr("Tt linear temperature coefficient")));
  Props.append(Property("Ttt2", "0.0", false,
	QObject::tr("Tt quadratic temperature coefficient")));
  Props.append(Property("Tm1", "0.0", false,
	QObject::tr("M linear temperature coefficient")));
  Props.append(Property("Tm2", "0.0", false,
	QObject::tr("M quadratic temperature coefficient")));
  Props.append(Property("Tnom", "26.85", false,
	QObject::tr("temperature at which parameters were extracted")));
  Props.append(Property("Area", "1.0", false,
	QObject::tr("default area for diode")));
  Props.append(Property("Symbol", "normal", false,
	QObject::tr("schematic symbol")+" [normal, US, Schottky, Zener, Varactor]"));

  createSymbol();
  tx = x1+4;
  ty = y2+4;
  Model = "Diode";
  Name  = "D";
}

Component* Diode::newOne()
{
  return new Diode();
}

Element* Diode::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Diode");
  BitmapFile = (char *) "diode";

  if(getNewOne)  return new Diode();
  return 0;
}

// -------------------------------------------------------
void Diode::createSymbol()
{
  if(Props.last().Value.at(0) == 'V') {
    Lines.append(Line(-30,  0, -9,  0,QPen(Qt::darkBlue,2)));
    Lines.append(Line( -6,  0, 30,  0,QPen(Qt::darkBlue,2)));
    Lines.append(Line( -9, -9, -9,  9,QPen(Qt::darkBlue,2)));
  }
  else if(Props.last().Value.at(0) == 'U') {
    Lines.append(Line(-30,  0, -6,  0,QPen(Qt::darkBlue,2)));
    Lines.append(Line(  6,  0, 30,  0,QPen(Qt::darkBlue,2)));
  }
  else {
    Lines.append(Line(-30,  0, 30,  0,QPen(Qt::darkBlue,2)));
  }
  Lines.append(Line( -6, -9, -6,  9,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  6, -9,  6,  9,QPen(Qt::darkBlue,2)));
  Lines.append(Line( -6,  0,  6, -9,QPen(Qt::darkBlue,2)));
  Lines.append(Line( -6,  0,  6,  9,QPen(Qt::darkBlue,2)));

  if(Props.last().Value.at(0) == 'S') {
    Lines.append(Line( -6, -9,-12,-12,QPen(Qt::darkBlue,2)));
    Lines.append(Line( -6,  9,  0, 12,QPen(Qt::darkBlue,2)));
  }
  else if(Props.last().Value.at(0) == 'Z') {
    Lines.append(Line( -6, 9, -1, 9,QPen(Qt::darkBlue,2)));
  }

  Ports.append(Port(-30, 0));
  Ports.append(Port( 30, 0));

  x1 = -30; y1 = -11;
  x2 =  30; y2 =  11;
}
