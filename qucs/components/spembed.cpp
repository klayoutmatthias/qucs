/***************************************************************************
                               spembed.cpp
                              ----------------
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
#include "spembed.h"
#include "qucs.h"
#include "schematic.h"
#include "misc.h"

#include <QFileInfo>


SPEmbed::SPEmbed()
{
  Description = QObject::tr("S parameter file embedding");

  Model = "SPfile";
  Name  = "X";

  // must be the first property !!!
  Props.append(Property("File", "test.s1p", true,
		QObject::tr("name of the s parameter file")));
  Props.append(Property("Data", "rectangular", false,
		QObject::tr("data type")+" [rectangular, polar]"));
  Props.append(Property("Interpolator", "linear", false,
		QObject::tr("interpolation type")+" [linear, cubic]"));
  Props.append(Property("duringDC", "open", false,
		QObject::tr("representation during DC analysis")+
			    " [open, short, shortall, unspecified]"));

  // must be the last property !!!
  Props.append(Property("Ports", "1", false,
		QObject::tr("number of ports")));

  createSymbol();
}

// -------------------------------------------------------
Component* SPEmbed::newOne()
{
  SPEmbed* p = new SPEmbed();
  p->Props.last().Value = Props.last().Value;
  p->recreate(0);
  return p;
}

// -------------------------------------------------------
Element* SPEmbed::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-port S parameter file");
  BitmapFile = (char *) "spfile3";

  if(getNewOne) {
    SPEmbed* p = new SPEmbed();
    p->Props[0].Value = "test.s3p";
    p->Props.last().Value = "3";
    p->recreate(0);
    return p;
  }
  return 0;
}

// -------------------------------------------------------
Element* SPEmbed::info1(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("1-port S parameter file");
  BitmapFile = (char *) "spfile1";

  if(getNewOne)  return new SPEmbed();
  return 0;
}

// -------------------------------------------------------
Element* SPEmbed::info2(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("2-port S parameter file");
  BitmapFile = (char *) "spfile2";

  if(getNewOne) {
    SPEmbed* p = new SPEmbed();
    p->Props[0].Value = "test.s2p";
    p->Props.last().Value = "2";
    p->recreate(0);
    return p;
  }
  return 0;
}

// -------------------------------------------------------
QString SPEmbed::getSubcircuitFile()
{
  // construct full filename
  QString FileName = Props[0].Value;
  return misc::properAbsFileName(FileName);
}

// -------------------------------------------------------
QString SPEmbed::netlist()
{
  QString s = Model+":"+Name;

  // output all node names
  for(auto p1 = Ports.begin(); p1 != Ports.end(); ++p1)
    s += " "+p1->Connection->Name;   // node names

  // output all properties
  s += " "+Props[0].Name+"=\"{"+getSubcircuitFile()+"}\"";

  // data type
  s += " "+Props[1].Name+"=\""+Props[1].Value+"\"";

  // interpolator type
  s += " "+Props[2].Name+"=\""+Props[2].Value+"\"";

  // DC property
  s += " "+Props[3].Name+"=\""+Props[3].Value+"\"\n";

  return s;
}

// -------------------------------------------------------
void SPEmbed::createSymbol()
{
  QFont Font(QucsSettings.font); // default application font
  // symbol text is smaller (10 pt default)
  Font.setPointSize(10 ); 
  // get the small font size; use the screen-compatible metric
  QFontMetrics  smallmetrics(Font, 0);
  int fHeight = smallmetrics.lineSpacing();
  QString stmp;

  int w, PortDistance = 60;
  int Num = Props.last().Value.toInt();

  // adjust number of ports
  if(Num < 1) Num = 1;
  else if(Num > 8) {
    PortDistance = 20;
    if(Num > 40) Num = 40;
  }
  Props.last().Value = QString::number(Num);

  // draw symbol outline
  int h = (PortDistance/2)*((Num-1)/2) + 15;
  Lines.append(Line(-15, -h, 15, -h,QPen(Qt::darkBlue,2)));
  Lines.append(Line( 15, -h, 15,  h,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-15,  h, 15,  h,QPen(Qt::darkBlue,2)));
  Lines.append(Line(-15, -h,-15,  h,QPen(Qt::darkBlue,2)));
  stmp = QObject::tr("file");
  w = smallmetrics.horizontalAdvance(stmp); // compute text size to center it
  Texts.append(Text(-w/2, -fHeight/2, stmp));

  int i=0, y = 15-h;
  while(i<Num) { // add ports lines and numbers
    i++;
    Lines.append(Line(-30, y,-15, y,QPen(Qt::darkBlue,2)));
    Ports.append(Port(-30, y));
    stmp = QString::number(i);
    w = smallmetrics.horizontalAdvance(stmp);
    Texts.append(Text(-25-w, y-fHeight-2, stmp)); // text right-aligned

    if(i == Num) break; // if odd number of ports there will be one port less on the right side
    i++;
    Lines.append(Line( 15, y, 30, y,QPen(Qt::darkBlue,2)));
    Ports.append(Port( 30, y));
    stmp = QString::number(i);
    Texts.append(Text(25, y-fHeight-2, stmp)); // text left-aligned
    y += PortDistance;
  }

  Lines.append(Line( 0, h, 0,h+15,QPen(Qt::darkBlue,2)));
  Texts.append(Text( 4, h,"Ref"));
  Ports.append(Port( 0,h+15));    // 'Ref' port

  x1 = -30; y1 = -h-2;
  x2 =  30; y2 =  h+15;
  // compute component name text position - normal size font
  QFontMetrics  metrics(QucsSettings.font, 0);   // use the screen-compatible metric
  tx = x1+4;
  ty = y1 - 2*metrics.lineSpacing() - 4;
}
