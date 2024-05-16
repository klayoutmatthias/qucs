/***************************************************************************
                               noise_iv.cpp
                              --------------
    begin                : Sat Aug 20 2005
    copyright            : (C) 2005 by Michael Margraf
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

#include "noise_iv.h"


Noise_iv::Noise_iv()
{
  Description = QObject::tr("correlated current sources");

  // left noise source
  Arcs.push_back(Arc(-42,-12, 24, 24,  0, 16*360,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-30, 30,-30, 12,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-30,-30,-30,-12,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-30,  7,-30, -7,QPen(Qt::darkBlue,3)));
  Lines.push_back(Line(-30, -6,-34,  0,QPen(Qt::darkBlue,3)));
  Lines.push_back(Line(-30, -6,-26,  0,QPen(Qt::darkBlue,3)));

  Lines.push_back(Line(-29, 12,-42, -1,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-24, 10,-27,  7,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-37, -3,-40, -6,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-20,  7,-25,  2,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-34, -7,-37,-10,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-18,  1,-31,-12,QPen(Qt::darkBlue,2)));

  // right noise source
  Arcs.push_back(Arc( 18,-12, 24, 24,  0, 16*360,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 30, 30, 30, 12,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 30,-30, 30,-12,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line( 31, 12, 18, -1,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 36, 10, 20, -6,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 40,  7, 23,-10,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 42,  2, 28,-12,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line(-18,  0, 18,  0,QPen(Qt::darkBlue,3)));

  Ports.push_back(Port(-30,-30));
  Ports.push_back(Port( 30,-30));
  Ports.push_back(Port( 30, 30));
  Ports.push_back(Port(-30, 30));

  x1 = -44; y1 = -30;
  x2 =  44; y2 =  30;

  tx = x1+4;
  ty = y2+4;
  Model = "IVnoise";
  Name  = "SRC";

  Props.push_back(Property("i1", "1e-6", true,
		QObject::tr("current power spectral density of source 1")));
  Props.push_back(Property("v2", "1e-6", true,
		QObject::tr("voltage power spectral density of source 2")));
  Props.push_back(Property("C", "0.5", true,
		QObject::tr("normalized correlation coefficient")));
  Props.push_back(Property("e", "0", false,
		QObject::tr("frequency exponent")));
  Props.push_back(Property("c", "1", false,
		QObject::tr("frequency coefficient")));
  Props.push_back(Property("a", "0", false,
		QObject::tr("additive frequency term")));
}

Noise_iv::~Noise_iv()
{
}

Component* Noise_iv::newOne()
{
  return new Noise_iv();
}

Element* Noise_iv::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Correlated Noise Sources");
  BitmapFile = (char *) "noise_iv";

  if(getNewOne)  return new Noise_iv();
  return 0;
}
