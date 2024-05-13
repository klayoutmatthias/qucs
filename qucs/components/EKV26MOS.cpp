/*
 * EKV26MOS.cpp - device implementations for EKV26MOS module
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 */
#include "node.h"
#include "EKV26MOS.h"

EKV26MOS::EKV26MOS()
{
  Description = QObject::tr ("EPFL-EKV MOS 2.6 verilog device");

  Props.append (Property ("Type", "nmos", true,
    QObject::tr ("polarity") + " [nmos, pmos]"));
  Props.append (Property ("LEVEL", "1", false,
    QObject::tr ("long = 1, short = 2")));
  Props.append (Property ("L", "0.5e-6", false,
    QObject::tr ("length parameter")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("W", "10e-6", false,
    QObject::tr ("Width parameter")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("Np", "1.0", false,
    QObject::tr ("parallel multiple device number")));
  Props.append (Property ("Ns", "1.0", false,
    QObject::tr ("series multiple device number")));
  Props.append (Property ("Cox", "3.45e-3", false,
    QObject::tr ("gate oxide capacitance per unit area")
    +" ("+QObject::tr ("F/m**2")+")"));
  Props.append (Property ("Xj", "0.15e-6", false,
    QObject::tr ("metallurgical junction depth")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("Dw", "-0.02e-6", false,
    QObject::tr ("channel width correction")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("Dl", "-0.05e-6", false,
    QObject::tr ("channel length correction")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("Vto", "0.6", false,
    QObject::tr ("long channel threshold voltage")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Gamma", "0.71", false,
    QObject::tr ("body effect parameter")
    +" ("+QObject::tr ("V**(1/2)")+")"));
  Props.append (Property ("Phi", "0.97", false,
    QObject::tr ("bulk Fermi potential")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Kp", "150e-6", false,
    QObject::tr ("transconductance parameter")
    +" ("+QObject::tr ("A/V**2")+")"));
  Props.append (Property ("Theta", "50e-3", false,
    QObject::tr ("mobility reduction coefficient")
    +" ("+QObject::tr ("1/V")+")"));
  Props.append (Property ("EO", "88.0e6", false,
    QObject::tr ("mobility coefficient")
    +" ("+QObject::tr ("V/m")+")"));
  Props.append (Property ("Ucrit", "4.5e6", false,
    QObject::tr ("longitudinal critical field")
    +" ("+QObject::tr ("V/m")+")"));
  Props.append (Property ("Lambda", "0.23", false,
    QObject::tr ("depletion length coefficient")));
  Props.append (Property ("Weta", "0.05", false,
    QObject::tr ("narrow-channel effect coefficient")));
  Props.append (Property ("Leta", "0.28", false,
    QObject::tr ("longitudinal critical field")));
  Props.append (Property ("Q0", "280e-6", false,
    QObject::tr ("reverse short channel charge density")
    +" ("+QObject::tr ("A*s/m**2")+")"));
  Props.append (Property ("Lk", "0.5e-6", false,
    QObject::tr ("characteristic length")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("Tcv", "1.5e-3", false,
    QObject::tr ("threshold voltage temperature coefficient")
    +" ("+QObject::tr ("V/K")+")"));
  Props.append (Property ("Bex", "-1.5", false,
    QObject::tr ("mobility temperature coefficient")));
  Props.append (Property ("Ucex", "1.7", false,
    QObject::tr ("Longitudinal critical field temperature exponent")));
  Props.append (Property ("Ibbt", "0.0", false,
    QObject::tr ("Ibb temperature coefficient")
    +" ("+QObject::tr ("1/K")+")"));
  Props.append (Property ("Hdif", "0.9e-6", false,
    QObject::tr ("heavily doped diffusion length")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("Rsh", "510.0", false,
    QObject::tr ("drain/source diffusion sheet resistance")
    +" ("+QObject::tr ("Ohm/square")+")"));
  Props.append (Property ("Rsc", "0.0", false,
    QObject::tr ("source contact resistance")
    +" ("+QObject::tr ("Ohm")+")"));
  Props.append (Property ("Rdc", "0.0", false,
    QObject::tr ("drain contact resistance")
    +" ("+QObject::tr ("Ohm")+")"));
  Props.append (Property ("Cgso", "1.5e-10", false,
    QObject::tr ("gate to source overlap capacitance")
    +" ("+QObject::tr ("F/m")+")"));
  Props.append (Property ("Cgdo", "1.5e-10", false,
    QObject::tr ("gate to drain overlap capacitance")
    +" ("+QObject::tr ("F/m")+")"));
  Props.append (Property ("Cgbo", "4.0e-10", false,
    QObject::tr ("gate to bulk overlap capacitance")
    +" ("+QObject::tr ("F/m")+")"));
  Props.append (Property ("Iba", "2e8", false,
    QObject::tr ("first impact ionization coefficient")
    +" ("+QObject::tr ("1/m")+")"));
  Props.append (Property ("Ibb", "3.5e8", false,
    QObject::tr ("second impact ionization coefficient")
    +" ("+QObject::tr ("V/m")+")"));
  Props.append (Property ("Ibn", "1.0", false,
    QObject::tr ("saturation voltage factor for impact ionization")));
  Props.append (Property ("Kf", "1.0e-27", false,
    QObject::tr ("flicker noise coefficient")));
  Props.append (Property ("Af", "1.0", false,
    QObject::tr ("flicker noise exponent")));
  Props.append (Property ("Avto", "0.0", false,
    QObject::tr ("area related theshold voltage mismatch parameter")
    +" ("+QObject::tr ("V*m")+")"));
  Props.append (Property ("Akp", "0.0", false,
    QObject::tr ("area related gain mismatch parameter")
    +" ("+QObject::tr ("m")+")"));
  Props.append (Property ("Agamma", "0.0", false,
    QObject::tr ("area related body effect mismatch parameter")
    +" ("+QObject::tr ("sqrt(V)*m")+")"));
  Props.append (Property ("N", "1.0", false,
    QObject::tr ("emission coefficient")));
  Props.append (Property ("Is", "1e-14", false,
    QObject::tr ("saturation current")
    +" ("+QObject::tr ("A")+")"));
  Props.append (Property ("Bv", "100", false,
    QObject::tr ("reverse breakdown voltage")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Ibv", "1e-3", false,
    QObject::tr ("current at reverse breakdown voltage")
    +" ("+QObject::tr ("A")+")"));
  Props.append (Property ("Vj", "1.0", false,
    QObject::tr ("junction potential")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Cj0", "300e-15", false,
    QObject::tr ("zero-bias junction capacitance")
    +" ("+QObject::tr ("F")+")"));
  Props.append (Property ("M", "0.5", false,
    QObject::tr ("grading coefficient")));
  Props.append (Property ("Area", "1.0", false,
    QObject::tr ("diode relative area")));
  Props.append (Property ("Fc", "0.5", false,
    QObject::tr ("forward-bias depletion capacitance coefficient")));
  Props.append (Property ("Tt", "0.1e-9", false,
    QObject::tr ("transit time")
    +" ("+QObject::tr ("s")+")"));
  Props.append (Property ("Xti", "3.0", false,
    QObject::tr ("saturation current temperature exponent")));
  Props.append (Property ("Xpart", "0.4", false,
    QObject::tr ("charge partition parameter")));
  Props.append (Property ("Tnom", "26.85", false,
    QObject::tr ("parameter measurement temperature")
    +" ("+QObject::tr ("Celsius")+")"));
  Props.append (Property ("Temp", "26.85", false,
    QObject::tr ("simulation temperature")));

  createSymbol ();
  tx = x2 + 4;
  ty = y1 + 4;
  Model = "EKV26MOS";
  Name  = "M";
}

Component * EKV26MOS::newOne()
{
  EKV26MOS * p = new EKV26MOS();
  p->getProperty("Type").Value = getProperty("Type").Value;
  p->recreate(0); 
  return p;
}

Element * EKV26MOS::info(QString& Name, char * &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("EPFL-EKV NMOS 2.6");
  BitmapFile = (char *) "EKV26nMOS";

  if(getNewOne) return new EKV26MOS();
  return 0;
}

Element * EKV26MOS::info_pmos(QString& Name, char * &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("EPFL-EKV PMOS 2.6");
  BitmapFile = (char *) "EKV26pMOS";

  if(getNewOne)
  {
    EKV26MOS* p = new EKV26MOS();
    p->getProperty("Type").Value = "pmos";
    p->getProperty("Dw").Value = "-0.03e-6";
    p->getProperty("Vto").Value = "-0.55";
    p->getProperty("Gamma").Value = "0.69";
    p->getProperty("Phi").Value = "0.87";
    p->getProperty("Kp").Value = "35e-6";
    p->getProperty("EO").Value = "51.0e6";
    p->getProperty("Ucrit").Value = "18.0e6";
    p->getProperty("Lambda").Value = "1.1";
    p->getProperty("Weta").Value = "0.0";
    p->getProperty("Leta").Value = "0.45";
    p->getProperty("Q0").Value = "200e-6";
    p->getProperty("Lk").Value = "0.6e-6";
    p->getProperty("Tcv").Value = "-1.4e-3";
    p->getProperty("Bex").Value = "-1.4";
    p->getProperty("Ucex").Value = "2.0";
    p->getProperty("Rsh").Value = "990.0";
    p->getProperty("Iba").Value = "0.0";
    p->getProperty("Ibb").Value = "3.0e8";
    p->getProperty("Kf").Value = "1.0e-28";
    p->recreate(0);
    return p;
  }
  return 0;
}

void EKV26MOS::createSymbol()
{
  // put in here symbol drawing code and terminal definitions
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

  // arrow
  if(getProperty("Type").Value == "nmos") {
    Lines.append(Line( -9,  0, -4, -5,QPen(Qt::darkBlue,2)));
    Lines.append(Line( -9,  0, -4,  5,QPen(Qt::darkBlue,2)));
  } else {
    Lines.append(Line( -5,  5,  0,   0,QPen(Qt::darkBlue,2)));
    Lines.append(Line( -5,  -5, 0,  0,QPen(Qt::darkBlue,2)));
  }
  
  Lines.append(Line(-10, -3,-10,  3,QPen(Qt::darkBlue,3)));
  Lines.append(Line(-10, -8,-10, -6,QPen(Qt::darkBlue,3)));
  Lines.append(Line(-10,  8,-10,  6,QPen(Qt::darkBlue,3)));

  // E
  Lines.append(Line(-30,-30,-30,-24,QPen(Qt::darkBlue,1)));
  Lines.append(Line(-30,-30,-26,-30,QPen(Qt::darkBlue,1)));
  Lines.append(Line(-30,-27,-26,-27,QPen(Qt::darkBlue,1)));
  Lines.append(Line(-30,-24,-26,-24,QPen(Qt::darkBlue,1)));
  // K
  Lines.append(Line(-24,-30,-24,-24,QPen(Qt::darkBlue,1)));
  Lines.append(Line(-24,-27,-20,-30,QPen(Qt::darkBlue,1)));
  Lines.append(Line(-24,-27,-20,-24,QPen(Qt::darkBlue,1)));
  // V
  Lines.append(Line(-18,-30,-16,-24,QPen(Qt::darkBlue,1)));
  Lines.append(Line(-14,-30,-16,-24,QPen(Qt::darkBlue,1)));

  Ports.append(Port(  0,-30)); // drain
  Ports.append(Port(-30,  0)); // gate
  Ports.append(Port(  0, 30)); // source
  Ports.append(Port( 20,  0)); // bulk

  x1 = -30; y1 = -30;
  x2 =  20; y2 =  30;
}

QString EKV26MOS::netlist()
{
  QString s = Model+":"+Name;

  // output all node names
  for(auto p1 = Ports.begin(); p1 != Ports.end(); ++p1)
    s += " "+p1->getConnection()->Name;   // node names

  // output type npn/pnp property
  auto p2 = Props.begin();
  if(p2->Value == "nmos")
    s += " nmos=\"1\"";
  else
    s += " pmos=\"1\"";

  // output all remaining properties
  ++p2;
  for( ; p2 != Props.end(); ++p2)
    s += " "+p2->Name+"=\""+p2->Value+"\"";

  return s + '\n';
}
