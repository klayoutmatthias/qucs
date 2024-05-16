/*
 * nigbt.cpp - device implementations for nigbt module
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 */

#include "nigbt.h"

nigbt::nigbt()
{
  Description = QObject::tr ("NIGBT verilog device");

  Props.push_back (Property ("Agd", "5.0e-6", false,
    QObject::tr ("gate-drain overlap area")
    +" ("+QObject::tr ("m**2")+")"));
  Props.push_back (Property ("Area", "1.0e-5", false,
    QObject::tr ("area of the device")
    +" ("+QObject::tr ("m**2")+")"));
  Props.push_back (Property ("Kp", "0.38", false,
    QObject::tr ("MOS transconductance")
    +" ("+QObject::tr ("A/V**2")+")"));
  Props.push_back (Property ("Tau", "7.1e-6", false,
    QObject::tr ("ambipolar recombination lifetime")
    +" ("+QObject::tr ("s")+")"));
  Props.push_back (Property ("Wb", "9.0e-5", false,
    QObject::tr ("metallurgical base width")
    +" ("+QObject::tr ("m")+")"));
  Props.push_back (Property ("BVf", "1.0", false,
    QObject::tr ("avalanche uniformity factor")));
  Props.push_back (Property ("BVn", "4.0", false,
    QObject::tr ("avalanche multiplication exponent")));
  Props.push_back (Property ("Cgs", "1.24e-8", false,
    QObject::tr ("gate-source capacitance per unit area")
    +" ("+QObject::tr ("F/cm**2")+")"));
  Props.push_back (Property ("Coxd", "3.5e-8", false,
    QObject::tr ("gate-drain oxide capacitance per unit area")
    +" ("+QObject::tr ("F/cm**2")+")"));
  Props.push_back (Property ("Jsne", "6.5e-13", false,
    QObject::tr ("emitter saturation current density")
    +" ("+QObject::tr ("A/cm**2")+")"));
  Props.push_back (Property ("Kf", "1.0", false,
    QObject::tr ("triode region factor")));
  Props.push_back (Property ("Mun", "1.5e-3", false,
    QObject::tr ("electron mobility")
    +" ("+QObject::tr ("cm**2/Vs")+")"));
  Props.push_back (Property ("Mup", "4.5e-2", false,
    QObject::tr ("hole mobility")
    +" ("+QObject::tr ("cm**2/Vs")+")"));
  Props.push_back (Property ("Nb", "2.0e14", false,
    QObject::tr ("base doping")
    +" ("+QObject::tr ("1/cm**3")+")"));
  Props.push_back (Property ("Theta", "0.02", false,
    QObject::tr ("transverse field factor")
    +" ("+QObject::tr ("1/V")+")"));
  Props.push_back (Property ("Vt", "4.7", false,
    QObject::tr ("threshold voltage")
    +" ("+QObject::tr ("V")+")"));
  Props.push_back (Property ("Vtd", "1.0e-3", false,
    QObject::tr ("gate-drain overlap depletion threshold")
    +" ("+QObject::tr ("V")+")"));
  Props.push_back (Property ("Tnom", "26.85", false,
    QObject::tr ("parameter measurement temperature")
    +" ("+QObject::tr ("Celsius")+")"));
  Props.push_back (Property ("Temp", "26.85", false,
    QObject::tr ("simulation temperature")
    +" ("+QObject::tr ("Celsius")+")"));

  createSymbol ();
  tx = x2 + 4;
  ty = y1 + 4;
  Model = "nigbt";
  Name  = "T";
}

Component * nigbt::newOne()
{
  nigbt * p = new nigbt();
  p->Props.front().Value = Props.front().Value; 
  p->recreate(0); 
  return p;
}

Element * nigbt::info(QString& Name, char * &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("NIGBT");
  BitmapFile = (char *) "nigbt";

  if(getNewOne) return new nigbt();
  return 0;
}

void nigbt::createSymbol()
{
  // bipolar
  Lines.push_back(Line(-10,-15,-10, 15,QPen(Qt::darkBlue,3)));
  Lines.push_back(Line(-30,  0,-14,  0,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-10, -5,  0,-15,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0,-15,  0,-30,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-10,  5,  0, 15,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0, 15,  0, 30,QPen(Qt::darkBlue,2)));

  // mos gate
  Lines.push_back(Line(-14,-13,-14, 13,QPen(Qt::darkBlue,3)));

  // arrow
  Lines.push_back(Line( -6, 15,  0, 15,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(  0,  9,  0, 15,QPen(Qt::darkBlue,2)));

  // terminal definitions
  Ports.push_back(Port(  0,-30)); // collector
  Ports.push_back(Port(-30,  0)); // gate
  Ports.push_back(Port(  0, 30)); // emitter

  // relative boundings
  x1 = -30; y1 = -30;
  x2 =   4; y2 =  30;
}
