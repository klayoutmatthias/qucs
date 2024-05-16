/***************************************************************************
                            binarytogrey4bit
                          -------------------
    begin                : December 2008
    copyright            : (C) 2008 by Mike Brinson
    email                : mbrin72043@yahoo.co.uk
 ***************************************************************************/

/*
 * binarytogrey4bit.cpp - device implementations for binarytogrey4bit module
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 */
#include "binarytogrey4bit.h"
#include "node.h"
#include "misc.h"

binarytogrey4bit::binarytogrey4bit()
{
  Type = isComponent; // Analogue and digital component.
  Description = QObject::tr ("4bit binary to Gray converter verilog device");

  Props.push_back (Property ("TR", "6", false,
    QObject::tr ("transfer function scaling factor")));
  Props.push_back (Property ("Delay", "1 ns", false,
    QObject::tr ("output delay")
    +" ("+QObject::tr ("s")+")"));

  createSymbol ();
  tx = x1 + 19;
  ty = y2 + 4;
  Model = "binarytogrey4bit";
  Name  = "Y";
}

Component * binarytogrey4bit::newOne()
{
  binarytogrey4bit * p = new binarytogrey4bit();
  p->Props.front().Value = Props.front().Value; 
  p->recreate(0); 
  return p;
}

Element * binarytogrey4bit::info(QString& Name, char * &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("4Bit Bin2Gray");
  BitmapFile = (char *) "binarytogrey4bit";

  if(getNewOne) return new binarytogrey4bit();
  return 0;
}

void binarytogrey4bit::createSymbol()
{
  Lines.push_back(Line(-30, -60, 30,-60,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 30, -60, 30, 40,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line( 30,  40,-30, 40,QPen(Qt::darkBlue,2)));
  Lines.push_back(Line(-30,  40,-30, -60,QPen(Qt::darkBlue,2)));

  Lines.push_back(Line(-50,-30,-30,-30,QPen(Qt::darkBlue,2)));  // B0
  Lines.push_back(Line(-50,-10,-30,-10,QPen(Qt::darkBlue,2)));  // B1
  Lines.push_back(Line(-50, 10,-30, 10,QPen(Qt::darkBlue,2)));  // B2
  Lines.push_back(Line(-50, 30,-30, 30,QPen(Qt::darkBlue,2)));  // B3

  Lines.push_back(Line( 30, 30, 50, 30,QPen(Qt::darkBlue,2)));  // G3
  Lines.push_back(Line( 30, 10, 50, 10,QPen(Qt::darkBlue,2)));  // G2
  Lines.push_back(Line( 30,-10, 50,-10,QPen(Qt::darkBlue,2)));  // G1
  Lines.push_back(Line( 30,-30, 50,-30,QPen(Qt::darkBlue,2)));  // G0
 
  Texts.push_back(Text(-16,-59, "B", Qt::darkBlue, 12.0));
  Texts.push_back(Text( -2,-59, "/", Qt::darkBlue, 12.0));
  Texts.push_back(Text(  5,-59, "G", Qt::darkBlue, 12.0));

  Texts.push_back(Text(-25,-43, "0", Qt::darkBlue, 12.0));
  Texts.push_back(Text(-25,-23, "1", Qt::darkBlue, 12.0));
  Texts.push_back(Text(-25, -3, "2", Qt::darkBlue, 12.0));
  Texts.push_back(Text(-25, 17, "3", Qt::darkBlue, 12.0));

  Texts.push_back(Text( 15,-43, "0", Qt::darkBlue, 12.0));
  Texts.push_back(Text( 15,-23, "1", Qt::darkBlue, 12.0));
  Texts.push_back(Text( 15, -3, "2", Qt::darkBlue, 12.0));
  Texts.push_back(Text( 15, 17, "3", Qt::darkBlue, 12.0));

  Ports.push_back(Port(-50,-30));  // B0
  Ports.push_back(Port(-50,-10));  // B1
  Ports.push_back(Port(-50, 10));  // B2
  Ports.push_back(Port(-50, 30));  // B3

  Ports.push_back(Port( 50, 30));  // G3
  Ports.push_back(Port( 50, 10));  // G2
  Ports.push_back(Port( 50,-10));  // G1
  Ports.push_back(Port( 50,-30));  // G0

  x1 = -50; y1 = -64;
  x2 =  50; y2 =  44;
}

QString binarytogrey4bit::vhdlCode( int )
{
  QString s="";

  QString td = prop(1).Value;     // delay time
  if(!misc::VHDL_Delay(td, Name)) return td; // time has not VHDL format
  td += ";\n";

  QString B0 = port(0).getConnection()->Name;
  QString B1 = port(1).getConnection()->Name;
  QString B2 = port(2).getConnection()->Name;
  QString B3 = port(3).getConnection()->Name;
  QString G3 = port(4).getConnection()->Name;
  QString G2 = port(5).getConnection()->Name;
  QString G1 = port(6).getConnection()->Name;
  QString G0 = port(7).getConnection()->Name;
 
 
  s = "\n  "+Name + ":process ("+B0+", "+B1+", "+B2+", "+B3+")\n" +
      "  begin\n"+
      "    "+G0+" <= "+B0+" xor "+B1+td+
      "    "+G1+" <= "+B1+" xor "+B2+td+
      "    "+G2+" <= "+B2+" xor "+B3+td+
      "    "+G3+" <= "+B3+td+
      "  end process;\n";
  return s;
}

QString binarytogrey4bit::verilogCode( int )
{
  QString td = prop(1).Value;        // delay time
  if(!misc::Verilog_Delay(td, Name)) return td; // time does not have VHDL format
  
  QString B0 = port(0).getConnection()->Name;
  QString B1 = port(1).getConnection()->Name;
  QString B2 = port(2).getConnection()->Name;
  QString B3 = port(3).getConnection()->Name;
  QString G3 = port(4).getConnection()->Name;
  QString G2 = port(5).getConnection()->Name;
  QString G1 = port(6).getConnection()->Name;
  QString G0 = port(7).getConnection()->Name;
 
  QString l = "";
 
  QString G0R = "net_reg" + Name + G0;
  QString G1R = "net_reg" + Name + G1;
  QString G2R = "net_reg" + Name + G2;
  QString G3R = "net_reg" + Name + G3;
  
  l = "\n  // " + Name + " 4bit binary to Gray\n" +
      "  assign  " + G0 + " = " + G0R + ";\n" +
      "  reg     " + G0R + " = 0;\n" +
      "  assign  " + G1 + " = " + G1R + ";\n" +
      "  reg     " + G1R + " = 0;\n" +
      "  assign  " + G2 + " = " + G2R + ";\n" +
      "  reg     " + G2R + " = 0;\n" +
      "  assign  " + G3 + " = " + G3R + ";\n" +
      "  reg     " + G3R + " = 0;\n" +
      "  always @ ("+B0+" or "+B1+" or "+B2+" or "+B3+")\n" +
      "  begin\n"+
      "    "+G0R+" <="+td+" "+B0+" ^ "+B1+";\n"+
      "    "+G1R+" <="+td+" "+B1+" ^ "+B2+";\n"+
      "    "+G2R+" <="+td+" "+B2+" ^ "+B3+";\n"+
      "    "+G3R+" <="+td+" "+B3+";\n"+
      "  end\n";
  return l;
}
