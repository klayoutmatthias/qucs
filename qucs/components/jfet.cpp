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
  Props.push_back(Property("Type", "nfet", true,
	QObject::tr("polarity")+" [nfet, pfet]"));
  Props.push_back(Property("Vt0", "-2.0 V", true,
	QObject::tr("threshold voltage")));
  Props.push_back(Property("Beta", "1e-4", true,
	QObject::tr("transconductance parameter")));
  Props.push_back(Property("Lambda", "0.0", true,
	QObject::tr("channel-length modulation parameter")));
  Props.push_back(Property("Rd", "0.0", false,
	QObject::tr("parasitic drain resistance")));
  Props.push_back(Property("Rs", "0.0", false,
	QObject::tr("parasitic source resistance")));
  Props.push_back(Property("Is", "1e-14", false,
	QObject::tr("gate-junction saturation current")));
  Props.push_back(Property("N", "1.0", false,
	QObject::tr("gate-junction emission coefficient")));
  Props.push_back(Property("Isr", "1e-14", false,
	QObject::tr("gate-junction recombination current parameter")));
  Props.push_back(Property("Nr", "2.0", false,
	QObject::tr("Isr emission coefficient")));
  Props.push_back(Property("Cgs", "0.0", false,
	QObject::tr("zero-bias gate-source junction capacitance")));
  Props.push_back(Property("Cgd", "0.0", false,
	QObject::tr("zero-bias gate-drain junction capacitance")));
  Props.push_back(Property("Pb", "1.0", false,
	QObject::tr("gate-junction potential")));
  Props.push_back(Property("Fc", "0.5", false,
	QObject::tr("forward-bias junction capacitance coefficient")));
  Props.push_back(Property("M", "0.5", false,
	QObject::tr("gate P-N grading coefficient")));
  Props.push_back(Property("Kf", "0.0", false,
	QObject::tr("flicker noise coefficient")));
  Props.push_back(Property("Af", "1.0", false,
	QObject::tr("flicker noise exponent")));
  Props.push_back(Property("Ffe", "1.0", false,
	QObject::tr("flicker noise frequency exponent")));
  Props.push_back(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
  Props.push_back(Property("Xti", "3.0", false,
	QObject::tr("saturation current temperature exponent")));
  Props.push_back(Property("Vt0tc", "0.0", false,
	QObject::tr("Vt0 temperature coefficient")));
  Props.push_back(Property("Betatce", "0.0", false,
	QObject::tr("Beta exponential temperature coefficient")));
  Props.push_back(Property("Tnom", "26.85", false,
	QObject::tr("temperature at which parameters were extracted")));
  Props.push_back(Property("Area", "1.0", false,
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
  p->Props.front().Value = Props.front().Value;
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
    p->Props.front().Value = "pfet";
    p->recreate(0);
    return p;
  }
  return 0;
}

// -------------------------------------------------------
void JFET::createSymbol()
{
  Lines.push_back(Line(-10,-15,-10, 15,QPen(Qt::darkBlue,3)));
  Lines.push_back(Line(-30,  0,-10,  0,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-10,-10,  0,-10,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0,-10,  0,-30,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-10, 10,  0, 10,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0, 10,  0, 30,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line( -4, 24,  4, 20,QPen(Qt::darkBlue,2)));

  if(Props.front().Value == "nfet") {
    Lines.push_back(Line(-16, -5,-11,  0,QPen(Qt::darkBlue,2)));
    Lines.push_back(Line(-16,  5,-11,  0,QPen(Qt::darkBlue,2)));
  }
  else {
    Lines.push_back(Line(-18,  0,-13, -5,QPen(Qt::darkBlue,2)));
    Lines.push_back(Line(-18,  0,-13,  5,QPen(Qt::darkBlue,2)));
  }

  Ports.push_back(Port(-30,  0));
  Ports.push_back(Port(  0,-30));
  Ports.push_back(Port(  0, 30));

  x1 = -30; y1 = -30;
  x2 =   4; y2 =  30;
}
