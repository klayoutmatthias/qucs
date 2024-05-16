/***************************************************************************
    copyright            : (C) 2008 by Michael Margraf
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

#include "tunneldiode.h"

TunnelDiode::TunnelDiode()
{
  Description = QObject::tr("resonance tunnel diode");

  Props.push_back(Property("Ip", "4 mA", true,
	QObject::tr("peak current")));
  Props.push_back(Property("Iv", "0.6 mA", true,
	QObject::tr("valley current")));
  Props.push_back(Property("Vv", "0.8 V", true,
	QObject::tr("valley voltage")));
  Props.push_back(Property("Wr", "2.7e-20", false,
	QObject::tr("resonance energy in Ws")));
  Props.push_back(Property("eta", "1e-20", false,
	QObject::tr("Fermi energy in Ws")));
  Props.push_back(Property("dW", "4.5e-21", false,
	QObject::tr("resonance width in Ws")));
  Props.push_back(Property("Tmax", "0.95", false,
	QObject::tr("maximum of transmission")));
  Props.push_back(Property("de", "0.9", false,
	QObject::tr("fitting factor for electron density")));
  Props.push_back(Property("dv", "2.0", false,
	QObject::tr("fitting factor for voltage drop")));
  Props.push_back(Property("nv", "16", false,
	QObject::tr("fitting factor for diode current")));

  Props.push_back(Property("Cj0", "80 fF", false,
	QObject::tr("zero-bias depletion capacitance")));
  Props.push_back(Property("M", "0.5", false,
	QObject::tr("grading coefficient")));
  Props.push_back(Property("Vj", "0.5 V", false,
	QObject::tr("junction potential")));
  Props.push_back(Property("te", "0.6 ps", false,
	QObject::tr("life-time of electrons")));

  Props.push_back(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
  Props.push_back(Property("Area", "1.0", false,
	QObject::tr("default area for diode")));

  Lines.push_back(Line(-30,  0,-12,  0,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 12,  0, 30,  0,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-12, -9,-12,  9,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0, -9,  0,  9,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 12, -9, 12,  9,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-12, -9, 12,  9,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-12,  9, 12, -9,QPen(Qt::darkBlue,2)));

  Ports.push_back(Port(-30, 0));
  Ports.push_back(Port( 30, 0));

  x1 = -30; y1 = -11;
  x2 =  30; y2 =  11;

  tx = x1+4;
  ty = y2+4;
  Model = "RTD";
  Name  = "D";
}

// -------------------------------------------------------------------
Component* TunnelDiode::newOne()
{
  return new TunnelDiode();
}

Element* TunnelDiode::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Tunnel Diode");
  BitmapFile = (char *) "tunneldiode";

  if(getNewOne)  return new TunnelDiode();
  return 0;
}
