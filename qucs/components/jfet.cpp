/***************************************************************************
                                jfet.cpp
                               ----------
    begin                : Fri Jun 4 2004
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

#include "jfet.h"


JFET::JFET()
{
  Description = QObject::tr("junction field-effect transistor");

  // this must be the first property in the list !!!
  Props.append(Property("Type", "nfet", true,
	QObject::tr("polarity")+" [nfet, pfet]"));
  Props.append(Property("Vt0", "-2.0 V", true,
	QObject::tr("threshold voltage")));
  Props.append(Property("Beta", "1e-4", true,
	QObject::tr("transconductance parameter")));
  Props.append(Property("Lambda", "0.0", true,
	QObject::tr("channel-length modulation parameter")));
  Props.append(Property("Rd", "0.0", false,
	QObject::tr("parasitic drain resistance")));
  Props.append(Property("Rs", "0.0", false,
	QObject::tr("parasitic source resistance")));
  Props.append(Property("Is", "1e-14", false,
	QObject::tr("gate-junction saturation current")));
  Props.append(Property("N", "1.0", false,
	QObject::tr("gate-junction emission coefficient")));
  Props.append(Property("Isr", "1e-14", false,
	QObject::tr("gate-junction recombination current parameter")));
  Props.append(Property("Nr", "2.0", false,
	QObject::tr("Isr emission coefficient")));
  Props.append(Property("Cgs", "0.0", false,
	QObject::tr("zero-bias gate-source junction capacitance")));
  Props.append(Property("Cgd", "0.0", false,
	QObject::tr("zero-bias gate-drain junction capacitance")));
  Props.append(Property("Pb", "1.0", false,
	QObject::tr("gate-junction potential")));
  Props.append(Property("Fc", "0.5", false,
	QObject::tr("forward-bias junction capacitance coefficient")));
  Props.append(Property("M", "0.5", false,
	QObject::tr("gate P-N grading coefficient")));
  Props.append(Property("Kf", "0.0", false,
	QObject::tr("flicker noise coefficient")));
  Props.append(Property("Af", "1.0", false,
	QObject::tr("flicker noise exponent")));
  Props.append(Property("Ffe", "1.0", false,
	QObject::tr("flicker noise frequency exponent")));
  Props.append(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
  Props.append(Property("Xti", "3.0", false,
	QObject::tr("saturation current temperature exponent")));
  Props.append(Property("Vt0tc", "0.0", false,
	QObject::tr("Vt0 temperature coefficient")));
  Props.append(Property("Betatce", "0.0", false,
	QObject::tr("Beta exponential temperature coefficient")));
  Props.append(Property("Tnom", "26.85", false,
	QObject::tr("temperature at which parameters were extracted")));
  Props.append(Property("Area", "1.0", false,
	QObject::tr("default area for JFET")));

  createSymbol();
  tx = x2+4;
  ty = y1+4;
  Model = "JFET";
  Name  = "T";
}

// -------------------------------------------------------
Component* JFET::newOne()
{
  JFET* p = new JFET();
  p->Props.first().Value = Props.first().Value;
  p->recreate(0);
  return p;
}

// -------------------------------------------------------
Element* JFET::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-JFET");
  BitmapFile = (char *) "nfet";

  if(getNewOne)  return new JFET();
  return 0;
}

// -------------------------------------------------------
Element* JFET::info_p(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("p-JFET");
  BitmapFile = (char *) "pfet";

  if(getNewOne) {
    JFET* p = new JFET();
    p->Props.first().Value = "pfet";
    p->recreate(0);
    return p;
  }
  return 0;
}

// -------------------------------------------------------
void JFET::createSymbol()
{
  Lines.append(Line(-10,-15,-10, 15,QPen(Qt::darkBlue,3)));
  Lines.append(Line(-30,  0,-10,  0,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-10,-10,  0,-10,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0,-10,  0,-30,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-10, 10,  0, 10,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0, 10,  0, 30,QPen(Qt::darkBlue,2)));

  Lines.append(Line( -4, 24,  4, 20,QPen(Qt::darkBlue,2)));

  if(Props.first().Value == "nfet") {
    Lines.append(Line(-16, -5,-11,  0,QPen(Qt::darkBlue,2)));
    Lines.append(Line(-16,  5,-11,  0,QPen(Qt::darkBlue,2)));
  }
  else {
    Lines.append(Line(-18,  0,-13, -5,QPen(Qt::darkBlue,2)));
    Lines.append(Line(-18,  0,-13,  5,QPen(Qt::darkBlue,2)));
  }

  Ports.append(Port(-30,  0));
  Ports.append(Port(  0,-30));
  Ports.append(Port(  0, 30));

  x1 = -30; y1 = -30;
  x2 =   4; y2 =  30;
}
