/*
 * phototransistor.cpp - device implementations for phototransistor module
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 */

#include "phototransistor.h"

phototransistor::phototransistor()
{
  Description = QObject::tr ("Phototransistor verilog device");

  Props.append (Property ("Bf", "100", false,
    QObject::tr ("forward beta")));
  Props.append (Property ("Br", "0.1", false,
    QObject::tr ("reverse beta")));
  Props.append (Property ("Is", "1e-10", false,
    QObject::tr ("dark current")
    +" ("+QObject::tr ("A")+")"));
  Props.append (Property ("Nf", "1", false,
    QObject::tr ("forward emission coefficient")));
  Props.append (Property ("Nr", "1", false,
    QObject::tr ("reverse emission coefficient")));
  Props.append (Property ("Vaf", "100", false,
    QObject::tr ("forward early voltage")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Var", "100", false,
    QObject::tr ("reverse early voltage")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Mje", "0.33", false,
    QObject::tr ("base-emitter junction exponential factor")));
  Props.append (Property ("Vje", "0.75", false,
    QObject::tr ("base-emitter junction built-in potential")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Cje", "1e-12", false,
    QObject::tr ("base-emitter zero-bias depletion capacitance")
    +" ("+QObject::tr ("F")+")"));
  Props.append (Property ("Mjc", "0.33", false,
    QObject::tr ("base-collector junction exponential factor")));
  Props.append (Property ("Vjc", "0.75", false,
    QObject::tr ("base-collector junction built-in potential")
    +" ("+QObject::tr ("V")+")"));
  Props.append (Property ("Cjc", "2e-12", false,
    QObject::tr ("base-collector zero-bias depletion capacitance")
    +" ("+QObject::tr ("F")+")"));
  Props.append (Property ("Tr", "100n", false,
    QObject::tr ("ideal reverse transit time")
    +" ("+QObject::tr ("s")+")"));
  Props.append (Property ("Tf", "0.1n", false,
    QObject::tr ("ideal forward transit time")
    +" ("+QObject::tr ("s")+")"));
  Props.append (Property ("Ikf", "10", false,
    QObject::tr ("high current corner for forward beta")
    +" ("+QObject::tr ("A")+")"));
  Props.append (Property ("Ikr", "10", false,
    QObject::tr ("high current corner for reverse beta")
    +" ("+QObject::tr ("A")+")"));
  Props.append (Property ("Rc", "10", false,
    QObject::tr ("collector series resistance")
    +" ("+QObject::tr ("Ohm")+")"));
  Props.append (Property ("Re", "1", false,
    QObject::tr ("emitter series resistance")
    +" ("+QObject::tr ("Ohm")+")"));
  Props.append (Property ("Rb", "100", false,
    QObject::tr ("base series resistance")
    +" ("+QObject::tr ("Ohm")+")"));
  Props.append (Property ("Kf", "1e-12", false,
    QObject::tr ("flicker noise coefficient")));
  Props.append (Property ("Ffe", "1", false,
    QObject::tr ("flicker noise coefficient")));
  Props.append (Property ("Af", "1", false,
    QObject::tr ("flicker noise exponent")));
  Props.append (Property ("Responsivity", "1.5", false,
    QObject::tr ("responsivity at relative selectivity=100%")
    +" ("+QObject::tr ("A/W")+")"));
  Props.append (Property ("P0", "2.6122e3", false,
    QObject::tr ("relative selectivity polynomial coefficient")));
  Props.append (Property ("P1", "-1.489e1", false,
    QObject::tr ("relative selectivity polynomial coefficient")));
  Props.append (Property ("P2", "3.0332e-2", false,
    QObject::tr ("relative selectivity polynomial coefficient")));
  Props.append (Property ("P3", "-2.5708e-5", false,
    QObject::tr ("relative selectivity polynomial coefficient")));
  Props.append (Property ("P4", "7.6923e-9", false,
    QObject::tr ("relative selectivity polynomial coefficient")));
  Props.append (Property ("Temp", "26.85", false,
    QObject::tr ("simulation temperature")));

  createSymbol ();
  tx = x2 + 4;
  ty = y1 +24;
  Model = "phototransistor";
  Name  = "PT";
}

Component * phototransistor::newOne()
{
  phototransistor * p = new phototransistor();
  p->Props.first().Value = Props.first().Value; 
  p->recreate(0); 
  return p;
}

Element * phototransistor::info(QString& Name, char * &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Phototransistor");
  BitmapFile = (char *) "phototransistor";

  if(getNewOne) return new phototransistor();
  return 0;
}

void phototransistor::createSymbol()
{
  Arcs.append(Arc(-25,-20, 40, 40,  0,16*360,QPen(Qt::red,2)));
  Lines.append(Line(-10,-15,-10, 15,QPen(Qt::darkBlue,3)));
  Lines.append(Line(-30,  0,-10,  0,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-10, -5,  0,-15,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0,-15,  0,-30,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-10,  5,  0, 15,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0, 15,  0, 30,QPen(Qt::darkBlue,2)));
  Lines.append(Line( -6, 15,  0, 15,QPen(Qt::darkBlue,2)));
  Lines.append(Line(  0,  9,  0, 15,QPen(Qt::darkBlue,2)));

  Lines.append(Line(-50, -50, -40, -50,QPen(Qt::green,2)));
  Lines.append(Line(-40, -50, -40, -30,QPen(Qt::green,2)));
  Lines.append(Line(-40, -30, -50, -30,QPen(Qt::green,2)));

  // green arrow
  Lines.append(Line(-40, -40, -16, -16,QPen(Qt::green,2)));
  Lines.append(Line(-16, -16, -16, -23,QPen(Qt::green,2)));
  Lines.append(Line(-16, -16, -23, -16,QPen(Qt::green,2)));

  // P
  Lines.append(Line(-60, -55, -60, -65,QPen(Qt::black,2)));
  Lines.append(Line(-60, -65, -55, -65,QPen(Qt::black,2)));
  Lines.append(Line(-55, -65, -55, -60,QPen(Qt::black,2)));
  Lines.append(Line(-60, -60, -55, -60,QPen(Qt::black,2)));

  // W
  Lines.append(Line(-63, -40, -60, -35,QPen(Qt::black,2)));
  Lines.append(Line(-60, -35, -57, -40,QPen(Qt::black,2)));
  Lines.append(Line(-57, -40, -54, -35,QPen(Qt::black,2)));
  Lines.append(Line(-54, -35, -51, -40,QPen(Qt::black,2)));

  Ports.append(Port(  0,-30)); // Collector
  Ports.append(Port(-30,  0)); // Base
  Ports.append(Port(  0, 30)); // Emitter
  Ports.append(Port(-50,-50)); // Power
  Ports.append(Port(-50,-30)); // Wavelength

  x1 = -50; y1 = -60;
  x2 =  20; y2 =  30;
}
