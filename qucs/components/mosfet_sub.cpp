/***************************************************************************
                              mosfet_sub.cpp
                             ----------------
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

#include "mosfet_sub.h"

Basic_MOSFET::Basic_MOSFET()
{
  // these must be the first properties in the list !!!
  Props.append(Property("Type", "nfet", false,
	QObject::tr("polarity")+" [nfet, pfet]"));
  Props.append(Property("Vt0", "1.0 V", true,
	QObject::tr("zero-bias threshold voltage")));

  Props.append(Property("Kp", "2e-5", true,
	QObject::tr("transconductance coefficient in A/V^2")));
  Props.append(Property("Gamma", "0.0", false,
	QObject::tr("bulk threshold in sqrt(V)")));
  Props.append(Property("Phi", "0.6 V", false,
	QObject::tr("surface potential")));
  Props.append(Property("Lambda", "0.0", true,
	QObject::tr("channel-length modulation parameter in 1/V")));
  Props.append(Property("Rd", "0.0 Ohm", false,
	QObject::tr("drain ohmic resistance")));
  Props.append(Property("Rs", "0.0 Ohm", false,
	QObject::tr("source ohmic resistance")));
  Props.append(Property("Rg", "0.0 Ohm", false,
	QObject::tr("gate ohmic resistance")));
  Props.append(Property("Is", "1e-14 A", false,
	QObject::tr("bulk junction saturation current")));
  Props.append(Property("N", "1.0", false,
	QObject::tr("bulk junction emission coefficient")));
  Props.append(Property("W", "1 um", false,
	QObject::tr("channel width")));
  Props.append(Property("L", "1 um", false,
	QObject::tr("channel length")));
  Props.append(Property("Ld", "0.0", false,
	QObject::tr("lateral diffusion length")));
  Props.append(Property("Tox", "0.1 um", false,
	QObject::tr("oxide thickness")));
  Props.append(Property("Cgso", "0.0", false,
	QObject::tr("gate-source overlap capacitance per meter of "
		    "channel width in F/m")));
  Props.append(Property("Cgdo", "0.0", false,
	QObject::tr("gate-drain overlap capacitance per meter of "
		    "channel width in F/m")));
  Props.append(Property("Cgbo", "0.0", false,
	QObject::tr("gate-bulk overlap capacitance per meter of "
		    "channel length in F/m")));
  Props.append(Property("Cbd", "0.0 F", false,
	QObject::tr("zero-bias bulk-drain junction capacitance")));
  Props.append(Property("Cbs", "0.0 F", false,
	QObject::tr("zero-bias bulk-source junction capacitance")));
  Props.append(Property("Pb", "0.8 V", false,
	QObject::tr("bulk junction potential")));
  Props.append(Property("Mj", "0.5", false,
	QObject::tr("bulk junction bottom grading coefficient")));
  Props.append(Property("Fc", "0.5", false,
	QObject::tr("bulk junction forward-bias depletion capacitance "
		    "coefficient")));
  Props.append(Property("Cjsw", "0.0", false,
	QObject::tr("zero-bias bulk junction periphery capacitance per meter "
		    "of junction perimeter in F/m")));
  Props.append(Property("Mjsw", "0.33", false,
	QObject::tr("bulk junction periphery grading coefficient")));
  Props.append(Property("Tt", "0.0 ps", false,
	QObject::tr("bulk transit time")));
  Props.append(Property("Nsub", "0.0", false,
	QObject::tr("substrate bulk doping density in 1/cm^3")));
  Props.append(Property("Nss", "0.0", false,
	QObject::tr("surface state density in 1/cm^2")));
  Props.append(Property("Tpg", "1", false,
	QObject::tr("gate material type: 0 = alumina; -1 = same as bulk; "
		    "1 = opposite to bulk")));
  Props.append(Property("Uo", "600.0", false,
	QObject::tr("surface mobility in cm^2/Vs")));
  Props.append(Property("Rsh", "0.0", false,
	QObject::tr("drain and source diffusion sheet resistance in "
		    "Ohms/square")));
  Props.append(Property("Nrd", "1", false,
	QObject::tr("number of equivalent drain squares")));
  Props.append(Property("Nrs", "1", false,
	QObject::tr("number of equivalent source squares")));
  Props.append(Property("Cj", "0.0", false,
	QObject::tr("zero-bias bulk junction bottom capacitance per square "
		    "meter of junction area in F/m^2")));
  Props.append(Property("Js", "0.0", false,
	QObject::tr("bulk junction saturation current per square "
		    "meter of junction area in A/m^2")));
  Props.append(Property("Ad", "0.0", false,
	QObject::tr("drain diffusion area in m^2")));
  Props.append(Property("As", "0.0", false,
	QObject::tr("source diffusion area in m^2")));
  Props.append(Property("Pd", "0.0 m", false,
	QObject::tr("drain junction perimeter")));
  Props.append(Property("Ps", "0.0 m", false,
	QObject::tr("source junction perimeter")));
  Props.append(Property("Kf", "0.0", false,
	QObject::tr("flicker noise coefficient")));
  Props.append(Property("Af", "1.0", false,
	QObject::tr("flicker noise exponent")));
  Props.append(Property("Ffe", "1.0", false,
	QObject::tr("flicker noise frequency exponent")));
  Props.append(Property("Temp", "26.85", false,
	QObject::tr("simulation temperature in degree Celsius")));
  Props.append(Property("Tnom", "26.85", false,
	QObject::tr("parameter measurement temperature")));

  Name  = "T";
}

MOSFET_sub::MOSFET_sub()
{
  Description = QObject::tr("MOS field-effect transistor with substrate");
  createSymbol();
  tx = x2+4;
  ty = y1+4;
  Model = "MOSFET";
}

// -------------------------------------------------------
Component* MOSFET_sub::newOne()
{
  MOSFET_sub* p = new MOSFET_sub();
  p->Props.first().Value = Props.first().Value;
  p->Props.next().Value = Props.next().Value;
  p->recreate(0);
  return p;
}

// -------------------------------------------------------
Element* MOSFET_sub::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-MOSFET");
  BitmapFile = (char *) "nmosfet_sub";

  if(getNewOne)  return new MOSFET_sub();
  return 0;
}

// -------------------------------------------------------
Element* MOSFET_sub::info_p(QString& Name,
			char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("p-MOSFET");
  BitmapFile = (char *) "pmosfet_sub";

  if(getNewOne) {
    MOSFET_sub* p = new MOSFET_sub();
    p->Props.first().Value = "pfet";
    p->Props.next().Value = "-1.0 V";
    p->recreate(0);
    return p;
  }
  return 0;
}

// -------------------------------------------------------
Element* MOSFET_sub::info_depl(QString& Name,
			char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("depletion MOSFET");
  BitmapFile = (char *) "dmosfet_sub";

  if(getNewOne) {
    MOSFET_sub* p = new MOSFET_sub();
    p->Props.first();
    p->Props.next().Value = "-1.0 V";
    p->recreate(0);
    return p;
  }
  return 0;
}

// -------------------------------------------------------
void MOSFET_sub::createSymbol()
{
  Lines.append(Line(-14,-13,-14, 13,QPen(Qt::darkBlue,3)));
  Lines.append(Line(-30,  0,-14,  0,QPen(Qt::darkBlue,2)));

  Lines.append(Line(-10,-11,  0,-11,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0,-11,  0,-30,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-10, 11,  0, 11,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0, 11,  0, 30,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-10,  0, 20,  0,QPen(Qt::darkBlue,2)));

  Lines.append(Line(-10,-16,-10, -7,QPen(Qt::darkBlue,3)));
  Lines.append(Line(-10,  7,-10, 16,QPen(Qt::darkBlue,3)));

  Lines.append(Line( -4, 24,  4, 20,QPen(Qt::darkBlue,2)));

  if(Props.first().Value == "nfet") {
    Lines.append(Line( -9,  0, -4, -5,QPen(Qt::darkBlue,2)));
    Lines.append(Line( -9,  0, -4,  5,QPen(Qt::darkBlue,2)));
  }
  else {
    Lines.append(Line( -1,  0, -6, -5,QPen(Qt::darkBlue,2)));
    Lines.append(Line( -1,  0, -6,  5,QPen(Qt::darkBlue,2)));
  }

  if((Props.next().Value.trimmed().at(0) == '-') ==
     (Props.first().Value == "nfet"))
    Lines.append(Line(-10, -8,-10,  8,QPen(Qt::darkBlue,3)));
  else
    Lines.append(Line(-10, -4,-10,  4,QPen(Qt::darkBlue,3)));

  Ports.append(Port(-30,  0));
  Ports.append(Port(  0,-30));
  Ports.append(Port(  0, 30));
  Ports.append(Port( 20,  0));

  x1 = -30; y1 = -30;
  x2 =  30; y2 =  30;
}
