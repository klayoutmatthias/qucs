/*
 * mod_amp.cpp - device implementations for mod_amp module
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 */

#include "mod_amp.h"

mod_amp::mod_amp()
{
  Description = QObject::tr ("Modular Operational Amplifier verilog device");

  Props.push_back (Property ("GBP", "1e6", false,
    QObject::tr ("Gain bandwidth product (Hz)")));
  Props.push_back (Property ("AOLDC", "106.0", false,
    QObject::tr ("Open-loop differential gain at DC (dB)")));
  Props.push_back (Property ("FP2", "3e6", false,
    QObject::tr ("Second pole frequency (Hz)")));
  Props.push_back (Property ("RO", "75", false,
    QObject::tr ("Output resistance (Ohm)")));
  Props.push_back (Property ("CD", "1e-12", false,
    QObject::tr ("Differential input capacitance (F)")));
  Props.push_back (Property ("RD", "2e6", false,
    QObject::tr ("Differential input resistance (Ohm)")));
  Props.push_back (Property ("IOFF", "20e-9", false,
    QObject::tr ("Input offset current (A)")));
  Props.push_back (Property ("IB", "80e-9", false,
    QObject::tr ("Input bias current (A)")));
  Props.push_back (Property ("VOFF", "7e-4", false,
    QObject::tr ("Input offset voltage (V)")));
  Props.push_back (Property ("CMRRDC", "90.0", false,
    QObject::tr ("Common-mode rejection ratio at DC (dB)")));
  Props.push_back (Property ("FCM", "200.0", false,
    QObject::tr ("Common-mode zero corner frequency (Hz)")));
  Props.push_back (Property ("PSRT", "5e5", false,
    QObject::tr ("Positive slew rate (V/s)")));
  Props.push_back (Property ("NSRT", "5e5", false,
    QObject::tr ("Negative slew rate (V/s)")));
  Props.push_back (Property ("VLIMP", "14", false,
    QObject::tr ("Positive output voltage limit (V)")));
  Props.push_back (Property ("VLIMN", "-14", false,
    QObject::tr ("Negative output voltage limit (V)")));
  Props.push_back (Property ("ILMAX", "35e-3", false,
    QObject::tr ("Maximum DC output current (A)")));
 Props.push_back (Property ("CSCALE", "50", false,
    QObject::tr ("Current limit scale factor")));
  createSymbol ();
  tx = x2 + 4;
  ty = y1 + 4;
  Model = "mod_amp";
  Name  = "OP";
}

Component * mod_amp::newOne()
{
  mod_amp * p = new mod_amp();
  p->Props.front().Value = Props.front().Value; 
  p->recreate(0); 
  return p;
}

Element * mod_amp::info(QString& Name, char * &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Modular OpAmp");
  BitmapFile = (char *) "mod_amp";

  if(getNewOne) return new mod_amp();
  return 0;
}

void mod_amp::createSymbol()
{

  Lines.push_back(Line(-30,-20,-20,-20,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-30, 20,-20, 20,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 30,  0, 40,  0,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line(-20,-35,-20, 35,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-20,-35, 30,  0,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-20, 35, 30,  0,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line(-16, 19, -9, 19,QPen(Qt::black,2)));
  Lines.push_back(Line(-16,-19, -9,-19,QPen(Qt::red,2)));
  Lines.push_back(Line(-13,-22,-13,-15,QPen(Qt::red,2)));

  Lines.push_back(Line(-10, -10, -10, 10,QPen(Qt::red,2)));
  Lines.push_back(Line(-10, -10,  0,   0,QPen(Qt::red,2)));
  Lines.push_back(Line(  0,  0,  10, -10,QPen(Qt::red,2)));
  Lines.push_back(Line( 10, -10, 10,  10,QPen(Qt::red,2)));
  
  Ports.push_back(Port(-30,-20));
  Ports.push_back(Port(-30, 20));
  Ports.push_back(Port( 40,  0));

  x1 = -30; y1 = -38;
  x2 =  40; y2 =  38;

}
