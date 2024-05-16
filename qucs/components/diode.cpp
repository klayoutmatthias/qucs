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

  Props.push_back(Property("Is", "1e-15 A", true,
	QObject::tr("saturation current")));
  Props.push_back(Property("N", "1", true,
	QObject::tr("emission coefficient")));
  Props.push_back(Property("Cj0", "10 fF", true,
	QObject::tr("zero-bias junction capacitance")));
  Props.push_back(Property("M", "0.5", false,
	QObject::tr("grading coefficient")));
  Props.push_back(Property("Vj", "0.7 V", false,
	QObject::tr("junction potential")));
  Props.push_back(Property("Fc", "0.5", false,
	QObject::tr("forward-bias depletion capacitance coefficient")));
  Props.push_back(Property("Cp", "0.0 fF", false,
	QObject::tr("linear capacitance")));
  Props.push_back(Property("Isr", "0.0", false,
	QObject::tr("recombination current parameter")));
  Props.push_back(Property("Nr", "2.0", false,
	QObject::tr("emission coefficient for Isr")));
  Props.push_back(Property("Rs", "0.0 Ohm", false,
	QObject::tr("ohmic series resistance")));
  Props.push_back(Property("Tt", "0.0 ps", false,
	QObject::tr("transit time")));
  Props.push_back(Property("Ikf", "0", false,
	QObject::tr("high-injection knee current (0=infinity)")));
  Props.push_back(Property("Kf", "0.0", false,
	QObject::tr("flicker noise coefficient")));
  Props.push_back(Property("Af", "1.0", false,
	QObject::tr("flicker noise exponent")));
  Props.push_back(Property("Ffe", "1.0", false,
	QObject::tr("flicker noise frequency exponent")));
  Props.push_back(Property("Bv", "0", false,
	QObject::tr("reverse breakdown voltage")));
  Props.push_back(Property("Ibv", "1 mA", false,
	QObject::tr("current at reverse breakdown voltage")));
  Props.push_back(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
  Props.push_back(Property("Xti", "3.0", false,
	QObject::tr("saturation current temperature exponent")));
  Props.push_back(Property("Eg", "1.11", false,
	QObject::tr("energy bandgap in eV")));
  Props.push_back(Property("Tbv", "0.0", false,
	QObject::tr("Bv linear temperature coefficient")));
  Props.push_back(Property("Trs", "0.0", false,
	QObject::tr("Rs linear temperature coefficient")));
  Props.push_back(Property("Ttt1", "0.0", false,
	QObject::tr("Tt linear temperature coefficient")));
  Props.push_back(Property("Ttt2", "0.0", false,
	QObject::tr("Tt quadratic temperature coefficient")));
  Props.push_back(Property("Tm1", "0.0", false,
	QObject::tr("M linear temperature coefficient")));
  Props.push_back(Property("Tm2", "0.0", false,
	QObject::tr("M quadratic temperature coefficient")));
  Props.push_back(Property("Tnom", "26.85", false,
	QObject::tr("temperature at which parameters were extracted")));
  Props.push_back(Property("Area", "1.0", false,
	QObject::tr("default area for diode")));
  Props.push_back(Property("Symbol", "normal", false,
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
  if(Props.back().Value.at(0) == 'V') {
    Lines.push_back(Line(-30,  0, -9,  0,QPen(Qt::darkBlue,2)));
    Lines.push_back(Line( -6,  0, 30,  0,QPen(Qt::darkBlue,2)));
    Lines.push_back(Line( -9, -9, -9,  9,QPen(Qt::darkBlue,2)));
  }
  else if(Props.back().Value.at(0) == 'U') {
    Lines.push_back(Line(-30,  0, -6,  0,QPen(Qt::darkBlue,2)));
    Lines.push_back(Line(  6,  0, 30,  0,QPen(Qt::darkBlue,2)));
  }
  else {
    Lines.push_back(Line(-30,  0, 30,  0,QPen(Qt::darkBlue,2)));
  }
  Lines.push_back(Line( -6, -9, -6,  9,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  6, -9,  6,  9,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( -6,  0,  6, -9,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( -6,  0,  6,  9,QPen(Qt::darkBlue,2)));

  if(Props.back().Value.at(0) == 'S') {
    Lines.push_back(Line( -6, -9,-12,-12,QPen(Qt::darkBlue,2)));
    Lines.push_back(Line( -6,  9,  0, 12,QPen(Qt::darkBlue,2)));
  }
  else if(Props.back().Value.at(0) == 'Z') {
    Lines.push_back(Line( -6, 9, -1, 9,QPen(Qt::darkBlue,2)));
  }

  Ports.push_back(Port(-30, 0));
  Ports.push_back(Port( 30, 0));

  x1 = -30; y1 = -11;
  x2 =  30; y2 =  11;
}
