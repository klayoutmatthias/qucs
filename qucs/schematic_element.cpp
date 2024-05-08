/***************************************************************************
                            schematic_element.cpp
                           -----------------------
    begin                : Sat Mar 3 2006
    copyright            : (C) 2006 by Michael Margraf
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
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#include "schematic.h"
#include <QDebug>


/* *******************************************************************
   *****                                                         *****
   *****              Actions handling the nodes                 *****
   *****                                                         *****
   ******************************************************************* */

// Inserts a port into the schematic and connects it to another node if
// the coordinates are identical. The node is returned.
Node* Schematic::insertNode(int x, int y, Element *e)
{
    // check if new node lies upon existing node
    auto pn = Nodes->begin();
    for( ; pn != Nodes->end(); ++pn)  // check every node
        if(pn->cx == x) if(pn->cy == y)
            {
                pn->Connections.append(e);
                break;
            }

    if(pn != Nodes->end()) {
      return pn.operator->();
    }

    // create new node, if no existing one lies at this position
    Node *newNode = new Node(x, y);
    Nodes->append(newNode);
    newNode->Connections.append(e);  // connect schematic node to component node

    // check if the new node lies upon an existing wire
    for(auto pw = Wires->begin(); pw != Wires->end(); ++pw)
    {
        if(pw->x1 == x)
        {
            if(pw->y1 > y) continue;
            if(pw->y2 < y) continue;
        }
        else if(pw->y1 == y)
        {
            if(pw->x1 > x) continue;
            if(pw->x2 < x) continue;
        }
        else continue;

        // split the wire into two wires
        splitWire(pw.operator->(), newNode);
        break;
    }

    return newNode;
}

// ---------------------------------------------------
Node* Schematic::selectedNode(int x, int y)
{
    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn) // test nodes
        if(pn->getSelected(x, y))
            return pn.operator->();

    return 0;
}


/* *******************************************************************
   *****                                                         *****
   *****              Actions handling the wires                 *****
   *****                                                         *****
   ******************************************************************* */

// Inserts a port into the schematic and connects it to another node if the
// coordinates are identical. If 0 is returned, no new wire is inserted.
// If 2 is returned, the wire line ended.
int Schematic::insertWireNode1(Wire *w)
{
    auto pn = Nodes->begin();
    // check if new node lies upon an existing node
    for( ; pn != Nodes->end(); ++pn) // check every node
        if(pn->cx == w->x1) if(pn->cy == w->y1) break;

    if(pn != Nodes->end())
    {
        pn->appendConnection(w);
        w->Port1 = pn.operator->();
        return 2;   // node is not new
    }



    // check if the new node lies upon an existing wire
    for(auto ptr2 = Wires->begin(); ptr2 != Wires->end(); ++ptr2)
    {
        if(ptr2->x1 == w->x1)
        {
            if(ptr2->y1 > w->y1) continue;
            if(ptr2->y2 < w->y1) continue;

            if(ptr2->isHorizontal() == w->isHorizontal())   // ptr2-wire is vertical
            {
                if(ptr2->y2 >= w->y2)
                {
                    delete w;    // new wire lies within an existing wire
                    return 0;
                }
                else
                {
                    // one part of the wire lies within an existing wire
                    // the other part not
                    if(ptr2->Port2->Connections.count() == 1)
                    {
                        w->y1 = ptr2->y1;
                        w->Port1 = ptr2->Port1;
                        if(ptr2->Label)
                        {
                            w->Label = ptr2->Label;
                            w->Label->pOwner = w;
                        }
                        w->Port1->removeConnection(ptr2.operator->());  // two -> one wire
                        w->Port1->appendConnection(w);
                        Nodes->erase(ptr2->Port2);
                        Wires->erase(ptr2);
                        return 2;
                    }
                    else
                    {
                        w->y1 = ptr2->y2;
                        w->Port1 = ptr2->Port2;
                        ptr2->Port2->appendConnection(w);   // shorten new wire
                        return 2;
                    }
                }
            }
        }
        else if(ptr2->y1 == w->y1)
        {
            if(ptr2->x1 > w->x1) continue;
            if(ptr2->x2 < w->x1) continue;

            if(ptr2->isHorizontal() == w->isHorizontal())   // ptr2-wire is horizontal
            {
                if(ptr2->x2 >= w->x2)
                {
                    delete w;   // new wire lies within an existing wire
                    return 0;
                }
                else
                {
                    // one part of the wire lies within an existing wire
                    // the other part not
                    if(ptr2->Port2->Connections.count() == 1)
                    {
                        w->x1 = ptr2->x1;
                        w->Port1 = ptr2->Port1;
                        if(ptr2->Label)
                        {
                            w->Label = ptr2->Label;
                            w->Label->pOwner = w;
                        }
                        ptr2->Port1->removeConnection(ptr2.operator->()); // two -> one wire
                        ptr2->Port1->appendConnection(w);
                        Nodes->erase(ptr2->Port2);
                        Wires->erase(ptr2);
                        return 2;
                    }
                    else
                    {
                        w->x1 = ptr2->x2;
                        w->Port1 = ptr2->Port2;
                        ptr2->Port2->Connections.append(w);   // shorten new wire
                        return 2;
                    }
                }
            }
        }
        else continue;

        Node *newNode = new Node(w->x1, w->y1);   // create new node
        Nodes->append(newNode);
        newNode->appendConnection(w);  // connect schematic node to the new wire
        w->Port1 = pn.operator->();

        // split the wire into two wires
        splitWire(ptr2.operator->(), newNode);
        return 2;
    }

    Node *newNode = new Node(w->x1, w->y1);   // create new node
    Nodes->append(newNode);
    newNode->appendConnection(w);  // connect schematic node to the new wire
    w->Port1 = newNode;
    return 1;
}

// ---------------------------------------------------
// if possible, connect two horizontal wires to one
bool Schematic::connectHWires1(Wire *w)
{
    Node *n = w->Port1;

    auto pw = n->Connections.end();
    --pw;  // last connection is the new wire itself
    for( ; pw != n->Connections.begin(); )
    {
        --pw;
        if((*pw)->Type != isWire) continue;
        Wire *wire = static_cast<Wire *>(*pw);
        if(!wire->isHorizontal()) continue;
        if(wire->x1 < w->x1)
        {
            if(n->Connections.count() != 2) continue;
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            else if(n->Label)
            {
                w->Label = n->Label;
                w->Label->pOwner = w;
                w->Label->Type = isHWireLabel;
            }
            w->x1 = wire->x1;
            w->Port1 = wire->Port1;      // new wire lengthens an existing one
            Nodes->erase(n);
            w->Port1->removeConnection(wire);
            w->Port1->appendConnection(w);
            Wires->erase(wire);
            return true;
        }
        if((*pw)->x2 >= w->x2)    // new wire lies within an existing one ?
        {
            w->Port1->removeConnection(w); // second node not yet made
            delete w;
            return false;
        }
        if(wire->Port2->Connections.count() < 2)
        {
            // existing wire lies within the new one
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            wire->Port1->removeConnection(wire);
            Nodes->erase(wire->Port2);
            Wires->erase(wire);
            return true;
        }
        w->x1 = wire->x2;    // shorten new wire according to an existing one
        w->Port1->removeConnection(w);
        w->Port1 = wire->Port2;
        w->Port1->appendConnection(w);
        return true;
    }

    return true;
}

// ---------------------------------------------------
// if possible, connect two vertical wires to one
bool Schematic::connectVWires1(Wire *w)
{
    Node *n = w->Port1;

    auto pw = n->Connections.end();
    --pw;  // last connection is the new wire itself
    for( ; pw != n->Connections.begin(); )
    {
        --pw;
        if((*pw)->Type != isWire) continue;
        Wire *wire = static_cast<Wire *>(*pw);
        if(wire->isHorizontal()) continue;
        if(wire->y1 < w->y1)
        {
            if(n->Connections.count() != 2) continue;
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            else if(n->Label)
            {
                w->Label = n->Label;
                w->Label->pOwner = w;
                w->Label->Type = isVWireLabel;
            }
            w->y1 = wire->y1;
            w->Port1 = wire->Port1;         // new wire lengthens an existing one
            Nodes->erase(n);
            w->Port1->removeConnection(wire);
            w->Port1->appendConnection(w);
            Wires->erase(wire);
            return true;
        }
        if(wire->y2 >= w->y2)    // new wire lies complete within an existing one ?
        {
            w->Port1->removeConnection(w); // second node not yet made
            delete w;
            return false;
        }
        if(wire->Port2->Connections.count() < 2)
        {
            // existing wire lies within the new one
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            wire->Port1->removeConnection(wire);
            Nodes->erase(wire->Port2);
            Wires->erase(wire);
            return true;
        }
        w->y1 = wire->y2;    // shorten new wire according to an existing one
        w->Port1->removeConnection(w);
        w->Port1 = wire->Port2;
        w->Port1->appendConnection(w);
        return true;
    }

    return true;
}

// ---------------------------------------------------
// Inserts a port into the schematic and connects it to another node if the
// coordinates are identical. If 0 is returned, no new wire is inserted.
// If 2 is returned, the wire line ended.
int Schematic::insertWireNode2(Wire *w)
{
    // check if new node lies upon an existing node
    auto pn = Nodes->begin();
    for( ; pn != Nodes->end(); ++pn)  // check every node
        if(pn->cx == w->x2) if(pn->cy == w->y2) break;

    if(pn != Nodes->end())
    {
        pn->appendConnection(w);
        w->Port2 = pn.operator->();
        return 2;   // node is not new
    }

    // check if the new node lies upon an existing wire
    for(auto ptr2 = Wires->begin(); ptr2 != Wires->end(); ++ptr2)
    {
        if(ptr2->x1 == w->x2)
        {
            if(ptr2->y1 > w->y2) continue;
            if(ptr2->y2 < w->y2) continue;

            // (if new wire lies within an existing wire, was already check before)
            if(ptr2->isHorizontal() == w->isHorizontal())   // ptr2-wire is vertical
            {
                // one part of the wire lies within an existing wire
                // the other part not
                if(ptr2->Port1->Connections.count() == 1)
                {
                    if(ptr2->Label)
                    {
                        w->Label = ptr2->Label;
                        w->Label->pOwner = w;
                    }
                    w->y2 = ptr2->y2;
                    w->Port2 = ptr2->Port2;
                    ptr2->Port2->removeConnection(ptr2.operator->());  // two -> one wire
                    ptr2->Port2->appendConnection(w);
                    Nodes->erase(ptr2->Port1);
                    Wires->erase(ptr2);
                    return 2;
                }
                else
                {
                    w->y2 = ptr2->y1;
                    w->Port2 = ptr2->Port1;
                    ptr2->Port1->appendConnection(w);   // shorten new wire
                    return 2;
                }
            }
        }
        else if(ptr2->y1 == w->y2)
        {
            if(ptr2->x1 > w->x2) continue;
            if(ptr2->x2 < w->x2) continue;

            // (if new wire lies within an existing wire, was already check before)
            if(ptr2->isHorizontal() == w->isHorizontal())   // ptr2-wire is horizontal
            {
                // one part of the wire lies within an existing wire
                // the other part not
                if(ptr2->Port1->Connections.count() == 1)
                {
                    if(ptr2->Label)
                    {
                        w->Label = ptr2->Label;
                        w->Label->pOwner = w;
                    }
                    w->x2 = ptr2->x2;
                    w->Port2 = ptr2->Port2;
                    ptr2->Port2->removeConnection(ptr2.operator->());  // two -> one wire
                    ptr2->Port2->appendConnection(w);
                    Nodes->erase(ptr2->Port1);
                    Wires->erase(ptr2);
                    return 2;
                }
                else
                {
                    w->x2 = ptr2->x1;
                    w->Port2 = ptr2->Port1;
                    ptr2->Port1->appendConnection(w);   // shorten new wire
                    return 2;
                }
            }
        }
        else continue;

        Node *newNode = new Node(w->x2, w->y2);   // create new node
        Nodes->append(newNode);
        newNode->Connections.append(w);  // connect schematic node to the new wire
        w->Port2 = newNode;

        // split the wire into two wires
        splitWire(ptr2.operator->(), newNode);
        return 2;
    }

    Node *newNode = new Node(w->x2, w->y2);   // create new node
    Nodes->append(newNode);
    newNode->Connections.append(w);  // connect schematic node to the new wire
    w->Port2 = newNode;
    return 1;
}

// ---------------------------------------------------
// if possible, connect two horizontal wires to one
bool Schematic::connectHWires2(Wire *w)
{
    Node *n = w->Port2;

    auto pw = n->Connections.end();
    --pw;  // last connection is the new wire itself
    for( ; pw != n->Connections.begin(); )
    {
        --pw;
        if((*pw)->Type != isWire) continue;
        Wire *wire = static_cast<Wire *>(*pw);
        if(!wire->isHorizontal()) continue;
        if(wire->x2 > w->x2)
        {
            if(n->Connections.count() != 2) continue;
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            w->x2 = wire->x2;
            w->Port2 = wire->Port2;      // new wire lengthens an existing one
            Nodes->erase(n);
            w->Port2->removeConnection(wire);
            w->Port2->appendConnection(w);
            Wires->erase(wire);
            return true;
        }
        // (if new wire lies complete within an existing one, was already
        // checked before)

        if(wire->Port1->Connections.count() < 2)
        {
            // existing wire lies within the new one
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            wire->Port2->removeConnection(wire);
            Nodes->erase(wire->Port1);
            Wires->erase(wire);
            return true;
        }
        w->x2 = wire->x1;    // shorten new wire according to an existing one
        w->Port2->removeConnection(w);
        w->Port2 = wire->Port1;
        w->Port2->appendConnection(w);
        return true;
    }

    return true;
}

// ---------------------------------------------------
// if possible, connect two vertical wires to one
bool Schematic::connectVWires2(Wire *w)
{
    Node *n = w->Port2;

    auto pw = n->Connections.end();
    --pw;  // last connection is the new wire itself
    for( ; pw != n->Connections.begin(); )
    {
        --pw;
        if((*pw)->Type != isWire) continue;
        Wire *wire = static_cast<Wire *>(*pw);
        if(wire->isHorizontal()) continue;
        if(wire->y2 > w->y2)
        {
            if(n->Connections.count() != 2) continue;
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            w->y2 = wire->y2;
            w->Port2 = wire->Port2;     // new wire lengthens an existing one
            Nodes->erase(n);
            w->Port2->removeConnection(wire);
            w->Port2->appendConnection(w);
            Wires->erase(wire);
            return true;
        }
        // (if new wire lies complete within an existing one, was already
        // checked before)

        if(wire->Port1->Connections.count() < 2)
        {
            // existing wire lies within the new one
            if(wire->Label)
            {
                w->Label = wire->Label;
                w->Label->pOwner = w;
            }
            wire->Port2->removeConnection(wire);
            Nodes->erase(wire->Port1);
            Wires->erase(wire);
            return true;
        }
        w->y2 = wire->y1;    // shorten new wire according to an existing one
        w->Port2->removeConnection(w);
        w->Port2 = wire->Port1;
        w->Port2->appendConnection(w);
        return true;
    }

    return true;
}

// ---------------------------------------------------
// Inserts a vertical or horizontal wire into the schematic and connects
// the ports that hit together. Returns whether the beginning and ending
// (the ports of the wire) are connected or not.
int Schematic::insertWire(const WireList::holder &w)
{
    int  tmp, con = 0;
    bool ok;

    // change coordinates if necessary (port 1 coordinates must be less
    // port 2 coordinates)
    if(w->x1 > w->x2)
    {
        tmp = w->x1;
        w->x1 = w->x2;
        w->x2 = tmp;
    }
    else if(w->y1 > w->y2)
    {
        tmp = w->y1;
        w->y1 = w->y2;
        w->y2 = tmp;
    }
    else con = 0x100;



    tmp = insertWireNode1(w.get());
    if(tmp == 0) return 3;  // no new wire and no open connection
    if(tmp > 1) con |= 2;   // insert node and remember if it remains open

    if(w->isHorizontal()) ok = connectHWires1(w.get());
    else ok = connectVWires1(w.get());
    if(!ok) return 3;




    tmp = insertWireNode2(w.get());
    if(tmp == 0) return 3;  // no new wire and no open connection
    if(tmp > 1) con |= 1;   // insert node and remember if it remains open

    if(w->isHorizontal()) ok = connectHWires2(w.get());
    else ok = connectVWires2(w.get());
    if(!ok) return 3;



    // change node 1 and 2
    if(con > 255) con = ((con >> 1) & 1) | ((con << 1) & 2);

    Wires->append(w);    // add wire to the schematic




    int  n1, n2;

    QSet<Wire *> wiresToDelete;

    // ................................................................
    // Check if the new line covers existing nodes.
    // In order to also check new appearing wires -> use "for"-loop
    // @@@ was from "current" before ...
    for(auto pw = Wires->begin(); pw != Wires->end(); ++pw) {
        for(auto pn = Nodes->begin(); pn != Nodes->end(); )    // check every node
        {
            if(pn->cx == pw->x1)
            {
                if(pn->cy <= pw->y1)
                {
                    ++pn;
                    continue;
                }
                if(pn->cy >= pw->y2)
                {
                    ++pn;
                    continue;
                }
            }
            else if(pn->cy == pw->y1)
            {
                if(pn->cx <= pw->x1)
                {
                    ++pn;
                    continue;
                }
                if(pn->cx >= pw->x2)
                {
                    ++pn;
                    continue;
                }
            }
            else
            {
                ++pn;
                continue;
            }

            n1 = 2;
            n2 = 3;
            Node *pn1 = 0, *pn2 = 0;
            // check all connections of the current node
            for(auto pe = pn->Connections.begin(); pe != pn->Connections.end(); ++pe)
            {
                if ((*pe)->Type != isWire) continue;
                Wire *nw = (Wire*)pe;
                // wire lies within the new ?
                if (pw->isHorizontal() != nw->isHorizontal()) continue;

                pn1 = nw->Port1;
                pn2 = nw->Port2;
                n1  = pn1->Connections.count();
                n2  = pn2->Connections.count();
                if(n1 == 1)
                {
                    Nodes->erase(pn1);       // delete node 1 if open
                    pn2->removeConnection(nw);   // remove connection
                    pn1 = pn2;
                }

                if(n2 == 1)
                {
                    pn1->removeConnection(nw);   // remove connection
                    Nodes->erase(pn2);       // delete node 2 if open
                    pn2 = pn1;
                }

                if(pn1 == pn2)
                {
                    if(nw->Label)
                    {
                        pw->Label = nw->Label;
                        pw->Label->pOwner = pw.operator->();
                    }
                    wiresToDelete.insert(nw);
                }
                break;
            }

            if (n1 == 1) if(n2 == 1) continue;

            if (pn1 && pn2) {

              // split wire into two wires
              if((pw->x1 != pn1->cx) || (pw->y1 != pn1->cy))
              {
                  Wire *newWire = new Wire(pw->x1, pw->y1, pn->cx, pn->cy, pw->Port1, pn1);
                  pn1->appendConnection(newWire);
                  Wires->append(newWire);
                  pw->Port1->appendConnection(newWire);
              }

              pw->Port1->removeConnection(pw.operator->());
              pw->x1 = pn2->cx;
              pw->y1 = pn2->cy;
              pw->Port1 = pn2;
              pn2->appendConnection(pw.operator->());

            }

        }
    }

    //  remove the obsolete wires
    for (auto i = wiresToDelete.begin(); i != wiresToDelete.end(); ++i) {
      Wires->erase(*i);
    }

    if (Wires->find(w.get()) != Wires->end())  // if two wire lines with different labels ...
        oneLabel(w->Port1);       // ... are connected, delete one label
    return con | 0x0200;   // sent also end flag
}

// ---------------------------------------------------
// Follows a wire line and selects it.
void Schematic::selectWireLine(Element *pe, Node *pn, bool ctrl)
{
    Node *pn_1st = pn;
    while(pn->Connections.count() == 2)
    {
        if(pn->Connections.first() == pe)  pe = pn->Connections.last();
        else  pe = pn->Connections.first();

        if(pe->Type != isWire) break;
        if(ctrl) pe->isSelected ^= ctrl;
        else pe->isSelected = true;

        if(((Wire*)pe)->Port1 == pn)  pn = ((Wire*)pe)->Port2;
        else  pn = ((Wire*)pe)->Port1;
        if(pn == pn_1st) break;  // avoid endless loop in wire loops
    }
}

// ---------------------------------------------------
Wire* Schematic::selectedWire(int x, int y)
{
    for(auto pw = Wires->begin(); pw != Wires->end(); ++pw)
        if(pw->getSelected(x, y))
            return pw.operator->();

    return 0;
}

// ---------------------------------------------------
// Splits the wire "*pw" into two pieces by the node "*pn".
Wire* Schematic::splitWire(Wire *pw, Node *pn)
{
    Wire *newWire = new Wire(pn->cx, pn->cy, pw->x2, pw->y2, pn, pw->Port2);
    newWire->isSelected = pw->isSelected;

    pw->x2 = pn->cx;
    pw->y2 = pn->cy;
    pw->Port2 = pn;

    newWire->Port2->Connections.prepend(newWire);
    pn->Connections.prepend(pw);
    pn->Connections.prepend(newWire);
    newWire->Port2->removeConnection(pw);
    Wires->append(newWire);

    if(pw->Label)
        if((pw->Label->cx > pn->cx) || (pw->Label->cy > pn->cy))
        {
            newWire->Label = pw->Label;   // label goes to the new wire
            pw->Label = 0;
            newWire->Label->pOwner = newWire;
        }

    return newWire;
}

// ---------------------------------------------------
// If possible, make one wire out of two wires.
bool Schematic::oneTwoWires(Node *n)
{
    Wire *e3;
    Wire *e1 = (Wire*)n->Connections.front();  // two wires -> one wire
    Wire *e2 = (Wire*)n->Connections.back();

    if(e1->Type == isWire) if(e2->Type == isWire)
            if(e1->isHorizontal() == e2->isHorizontal())
            {
                if(e1->x1 == e2->x2) if(e1->y1 == e2->y2)
                    {
                        e3 = e1;
                        e1 = e2;
                        e2 = e3;    // e1 must have lesser coordinates
                    }
                if(e2->Label)     // take over the node name label ?
                {
                    e1->Label = e2->Label;
                    e1->Label->pOwner = e1;
                }
                else if(n->Label)
                {
                    e1->Label = n->Label;
                    e1->Label->pOwner = e1;
                    if(e1->isHorizontal())
                        e1->Label->Type = isHWireLabel;
                    else
                        e1->Label->Type = isVWireLabel;
                }

                e1->x2 = e2->x2;
                e1->y2 = e2->y2;
                e1->Port2 = e2->Port2;
                Nodes->erase(n);    // delete node (is auto delete)
                e1->Port2->removeConnection(e2);
                e1->Port2->appendConnection(e1);
                Wires->erase(e2);
                return true;
            }
    return false;
}

// ---------------------------------------------------
// Deletes the wire 'w'.
void Schematic::deleteWire(const WireList::iterator &w)
{
    if(w->Port1->Connections.count() == 1)
    {
        if(w->Port1->Label) delete w->Port1->Label;
        Nodes->erase(w->Port1);     // delete node 1 if open
    }
    else
    {
        w->Port1->removeConnection(w.operator->());   // remove connection
        if(w->Port1->Connections.count() == 2)
            oneTwoWires(w->Port1);  // two wires -> one wire
    }

    if(w->Port2->Connections.count() == 1)
    {
        if(w->Port2->Label) delete w->Port2->Label;
        Nodes->erase(w->Port2);     // delete node 2 if open
    }
    else
    {
        w->Port2->removeConnection(w.operator->());   // remove connection
        if(w->Port2->Connections.count() == 2)
            oneTwoWires(w->Port2);  // two wires -> one wire
    }

    if(w->Label)
    {
        delete w->Label;
        w->Label = 0;
    }
    Wires->erase(w);
}

// ---------------------------------------------------
int Schematic::copyWires(int& x1, int& y1, int& x2, int& y2,
                         QVector<Element *> &ElementCache)
{
    int count=0;
    Node *pn;
    for(auto pwn = Wires->begin(); pwn != Wires->end(); ) { // find bounds of all selected wires
        auto pw = pwn;
        ++pwn;
        if(pw->isSelected)
        {
            if(pw->x1 < x1) x1 = pw->x1;
            if(pw->x2 > x2) x2 = pw->x2;
            if(pw->y1 < y1) y1 = pw->y1;
            if(pw->y2 > y2) y2 = pw->y2;

            count++;
            ElementCache.append(pw.operator->());

            // rescue non-selected node labels
            pn = pw->Port1;
            if(pn->Label)
                if(pn->Connections.count() < 2)
                {
                    ElementCache.append(pn->Label);

                    // Don't set pn->Label->pOwner=0 , so text position stays unchanged.
                    // But remember its wire.
                    pn->Label->pOwner = pw.operator->();
                    pn->Label = 0;
                }
            pn = pw->Port2;
            if(pn->Label)
                if(pn->Connections.count() < 2)
                {
                    ElementCache.append(pn->Label);

                    // Don't set pn->Label->pOwner=0 , so text position stays unchanged.
                    // But remember its wire.
                    pn->Label->pOwner = pw.operator->();
                    pn->Label = 0;
                }

            // @@@pl = pw->Label;
            pw->Label = 0; // @@@ bug?
            deleteWire(pw);
            // @@@ did access deleted object?
            // @@@pw->Label = pl;    // restore wire label
        }
    }

    return count;
}


/* *******************************************************************
   *****                                                         *****
   *****                  Actions with markers                   *****
   *****                                                         *****
   ******************************************************************* */

Marker* Schematic::setMarker(int x, int y)
{
  // only diagrams ...
  for(auto pd = Diagrams->end(); pd != Diagrams->begin(); ) {
    --pd;
    if(Marker* m=pd->setMarker(x,y)){
      setChanged(true, true);
      return m;
    }
  }
  return NULL;
}

// ---------------------------------------------------
// Moves the marker pointer left/right on the graph.
void Schematic::markerLeftRight(bool left, const QVector<Element *> &Elements)
{
    bool acted = false;
    for(auto i : Elements) {
        Marker* pm = prechecked_cast<Marker*>(i);
        assert(pm);
        if(pm->moveLeftRight(left))
            acted = true;
    }

    if(acted)  setChanged(true, true, 'm');
}

// ---------------------------------------------------
// Moves the marker pointer up/down on the more-dimensional graph.
void Schematic::markerUpDown(bool up, const QVector<Element *> &Elements)
{
    bool acted = false;
    for(auto i : Elements) {
        Marker* pm = prechecked_cast<Marker*>(i);
        assert(pm);
        if(pm->moveUpDown(up))
            acted = true;
    }

    if(acted)  setChanged(true, true, 'm');
}


/* *******************************************************************
   *****                                                         *****
   *****               Actions with all elements                 *****
   *****                                                         *****
   ******************************************************************* */

/* Selects the element that contains the coordinates x/y.
   Returns the pointer to the element.

   If 'flag' is true, the element can be deselected. If
   'flag' is false the element cannot be deselected. The
   purpose of this is to prevent deselection in cases such
   as right-clicking on a selected element to get a context
   menu.
*/
Element* Schematic::selectElement(float fX, float fY, bool flag, int *index)
{
    int n, x = int(fX), y = int(fY);
    Element *pe_1st = 0;
    Element *pe_sel = 0;
    WireLabel *pl = 0;
    float Corr = textCorr(); // for selecting text

    // test all nodes and their labels
    for(auto pn = Nodes->end(); pn != Nodes->begin(); )
    {
        --pn;
        if(!flag)
        {
            // The element cannot be deselected
            if(index)
            {
                // 'index' is only true if called from MouseActions::MPressSelect()
                if(pn->getSelected(x, y))
                {
                    // Return the node pointer, as the selection cannot change
                    return pn.operator->();
                }
            }
        }

        pl = pn->Label; // Get any wire label associated with the Node
        if(pl)
        {
            if(pl->getSelected(x, y))
            {
                if(flag)
                {
                    // The element can be deselected, so toggle its isSelected member
                    // TODO: I don't see a need for the xor here, a simple ! on the current value
                    // would be clearer and have the same effect?
                    pl->isSelected ^= flag;
                    return pl;
                }
                if(pe_sel)
                {
                    // There is another currently
                    pe_sel->isSelected = false;
                    return pl;
                }
                if(pe_1st == 0)
                {
                    // give access to elements lying beneath by storing this label.
                    // If no label pointer (or other element) has previously been
                    // stored, the current label pointer is stored here.
                    // pe_1st is returned if no other selected element
                    pe_1st = pl;
                }
                if(pl->isSelected)
                {
                    // if current label is already selected, store a pointer to it.
                    // This can be used to cycle through
                    pe_sel = pl;
                }
            }
        }
    }

    // test all wires and wire labels
    for(auto pw = Wires->end(); pw != Wires->begin(); )
    {
        --pw;
        if(pw->getSelected(x, y))
        {
            if(flag)
            {
                // The element can be deselected
                pw->isSelected ^= flag;
                return pw.operator->();
            }
            if(pe_sel)
            {
                pe_sel->isSelected = false;
                return pw.operator->();
            }
            if(pe_1st == 0)
            {
                pe_1st = pw.operator->();   // give access to elements lying beneath
            }
            if(pw->isSelected)
            {
                pe_sel = pw.operator->();
            }
        }
        pl = pw->Label; // test any label associated with the wire
        if(pl)
        {
            if(pl->getSelected(x, y))
            {
                if(flag)
                {
                    // The element can be deselected
                    pl->isSelected ^= flag;
                    return pl;
                }
                if(pe_sel)
                {
                    pe_sel->isSelected = false;
                    return pl;
                }
                if(pe_1st == 0)
                {
                    // give access to elements lying beneath
                    pe_1st = pl;
                }
                if(pl->isSelected)
                {
                    pe_sel = pl;
                }
            }
        }
    }

    // test all components
    for(auto pc = Components->end(); pc != Components->begin(); )
    {
        --pc;
        if(pc->getSelected(x, y))
        {
            if(flag)
            {
                // The element can be deselected
                pc->isSelected ^= flag;
                return pc.operator->();
            }
            if(pe_sel)
            {
                pe_sel->isSelected = false;
                return pc.operator->();
            }
            if(pe_1st == 0)
            {
                pe_1st = pc.operator->();
            }  // give access to elements lying beneath
            if(pc->isSelected)
            {
                pe_sel = pc.operator->();
            }
        }
        else
        {
            n = pc->getTextSelected(x, y, Corr);
            if(n >= 0)     // was property text clicked ?
            {
                pc->Type = isComponentText;
                if(index)  *index = n;
                return pc.operator->();
            }
        }
    }

    Corr = 5.0 / Scale;  // size of line select and area for resizing
    // test all diagrams
    for(auto pd = Diagrams->end(); pd != Diagrams->begin(); )
    {
        --pd;
        for(auto pg = pd->Graphs.begin(); pg != pd->Graphs.end(); ++pg)
        {
            // test markers of graphs
            foreach(Marker *pm, pg->Markers)
            {
                if(pm->getSelected(x-pd->cx, y-pd->cy))
                {
                    if(flag)
                    {
                        // The element can be deselected
                        pm->isSelected ^= flag;
                        return pm;
                    }
                    if(pe_sel)
                    {
                        pe_sel->isSelected = false;
                        return pm;
                    }
                    if(pe_1st == 0)
                    {
                        pe_1st = pm;   // give access to elements beneath
                    }
                    if(pm->isSelected)
                    {
                        pe_sel = pm;
                    }
                }
            }
        }

        // resize area clicked ?
        if(pd->isSelected)
        {
            if(pd->resizeTouched(fX, fY, Corr))
            {
                if(pe_1st == 0)
                {
                    pd->Type = isDiagramResize;
                    return pd.operator->();
                }
            }
        }

        if(pd->getSelected(x, y))
        {
            if(pd->Name[0] == 'T')     // tabular, timing diagram or truth table ?
            {
                if(pd->Name[1] == 'i')
                {
                    if(y > pd->cy)
                    {
                        if(x < pd->cx+pd->xAxis.numGraphs) continue;
                        pd->Type = isDiagramHScroll;
                        return pd.operator->();
                    }
                }
                else
                {
                    if(x < pd->cx)        // clicked on scroll bar ?
                    {
                        pd->Type = isDiagramVScroll;
                        return pd.operator->();
                    }
                }
            }

            // test graphs of diagram
            for(auto pg = pd->Graphs.begin(); pg != pd->Graphs.end(); ++pg)
            {
                if(pg->getSelected(x-pd->cx, pd->cy-y) >= 0)
                {
                    if(flag)
                    {
                        // The element can be deselected
                        pg->isSelected ^= flag;
                        return pg.operator->();
                    }
                    if(pe_sel)
                    {
                        pe_sel->isSelected = false;
                        return pg.operator->();
                    }
                    if(pe_1st == 0)
                    {
                        pe_1st = pg.operator->();   // access to elements lying beneath
                    }
                    if(pg->isSelected)
                    {
                        pe_sel = pg.operator->();
                    }
                }
            }

            if(flag)
            {
                // The element can be deselected
                pd->isSelected ^= flag;
                return pd.operator->();;
            }
            if(pe_sel)
            {
                pe_sel->isSelected = false;
                return pd.operator->();;
            }
            if(pe_1st == 0)
            {
                pe_1st = pd.operator->();;    // give access to elements lying beneath
            }
            if(pd->isSelected)
            {
                pe_sel = pd.operator->();;
            }
        }
    }

    // test all paintings
    for(auto pp = Paintings->end(); pp != Paintings->begin(); )
    {
        --pp;
        if(pp->isSelected)
        {
            if(pp->resizeTouched(fX, fY, Corr))
            {
                if(pe_1st == 0)
                {
                    pp->Type = isPaintingResize;
                    return pp.operator->();;
                }
            }
        }

        if(pp->getSelected(fX, fY, Corr))
        {
            if(flag)
            {
                // The element can be deselected
                pp->isSelected ^= flag;
                return pp.operator->();;
            }
            if(pe_sel)
            {
                pe_sel->isSelected = false;
                return pp.operator->();;
            }
            if(pe_1st == 0)
            {
                pe_1st = pp.operator->();;    // give access to elements lying beneath
            }
            if(pp->isSelected)
            {
                pe_sel = pp.operator->();;
            }
        }
    }

    return pe_1st;
}

void Schematic::highlightWireLabels ()
{
    WireLabel *pltestinner = 0;
    WireLabel *pltestouter = 0;

    // First set highlighting for all wire and nodes labels to false
    for(auto pwouter = Wires->end(); pwouter != Wires->begin(); )
    {
        --pwouter;
        pltestouter = pwouter->Label; // test any label associated with the wire
        if (pltestouter)
        {
            pltestouter->setHighlighted (false);
        }
    }

    for(auto pnouter = Nodes->end(); pnouter != Nodes->begin(); )
    {
        --pnouter;
        pltestouter = pnouter->Label; // test any label associated with the node
        if (pltestouter)
        {
            pltestouter->setHighlighted (false);
        }
    }
	
    // Then test every wire's label to see if we need to highlight it
    // and matching labels on wires and nodes
    for (auto pwouter = Wires->begin(); pwouter != Wires->end(); ++pwouter) {
        // get any label associated with the wire
        pltestouter = pwouter->Label;
        if (pltestouter)
        {
            if (pltestouter->isSelected)
            {
                bool hiLightOuter = false;
                // Search for matching labels on wires
                for (auto pwinner = Wires->begin(); pwinner != Wires->end(); ++pwinner) {
                    pltestinner = pwinner->Label; // test any label associated with the wire
                    if (pltestinner)
                    {
                        // Highlight the label if it has the same name as the selected label
                        // if only one wire has this label, do not highlight it
                        if (pltestinner != pltestouter)
                        {
                            if (pltestouter->Name == pltestinner->Name)
                            {
                                pltestinner->setHighlighted (true);
                                hiLightOuter = true;
                            }
                        }
                    }
                }
                // Search for matching labels on nodes
                for (auto pninner = Nodes->begin(); pninner != Nodes->end(); ++pninner) {
                    pltestinner = pninner->Label; // test any label associated with the node
                    if (pltestinner)
                    {
                        if (pltestouter->Name == pltestinner->Name)
                        {
                            // node label matches wire label
                            pltestinner->setHighlighted (true);
                            hiLightOuter = true;
                        }
                    }
                }
                // highlight if at least two different wires/nodes with the same label found
                pltestouter->setHighlighted (hiLightOuter);
            }
        }
    }
    // Same as above but for nodes labels:
    // test every node label to see if we need to highlight it
    // and matching labels on wires and nodes
    for (auto pnouter = Nodes->begin(); pnouter != Nodes->end(); ++pnouter) {
        // get any label associated with the node
        pltestouter = pnouter->Label;
        if (pltestouter)
        {
            if (pltestouter->isSelected)
            {
                bool hiLightOuter = false;
                // Search for matching labels on wires
                for (auto pwinner = Wires->begin(); pwinner != Wires->end(); ++pwinner) {
                    pltestinner = pwinner->Label; // test any label associated with the wire
                    if (pltestinner)
                    {
                        if (pltestouter->Name == pltestinner->Name)
                        {
                            // wire label matches node label
                            pltestinner->setHighlighted (true);
                            hiLightOuter = true;
                        }
                    }
                }
                // Search for matching labels on nodes
                for (auto pninner = Nodes->begin(); pninner != Nodes->end(); ++pninner) {
                    pltestinner = pninner->Label; // test any label associated with the node
                    if (pltestinner)
                    {
                        // Highlight the label if it has the same name as the selected label
                        // if only one node has this label, do not highlight it
                        if (pltestinner != pltestouter)
                        {
                            if (pltestouter->Name == pltestinner->Name)
                            {
                                pltestinner->setHighlighted (true);
                                hiLightOuter = true;
                            }
                        }
                    }
                }
                // highlight if at least two different wires/nodes with the same label found
                pltestouter->setHighlighted (hiLightOuter);
            }
        }
    }
}

// ---------------------------------------------------
// Deselects all elements except 'e'.
void Schematic::deselectElements(Element *e)
{
    // test all components
    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
        if(e != pc.operator->())  pc->isSelected = false;

    // test all wires
    for(auto pw = Wires->begin(); pw != Wires->end(); ++pw)
    {
        if(e != pw.operator->())  pw->isSelected = false;
        if(pw->Label) if(pw->Label != e)  pw->Label->isSelected = false;
    }

    // test all node labels
    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn)
        if(pn->Label) if(pn->Label != e)  pn->Label->isSelected = false;

    // test all diagrams
    for(auto pd = Diagrams->begin(); pd != Diagrams->end(); ++pd)
    {
        if(e != pd.operator->())  pd->isSelected = false;

        // test graphs of diagram
        for(auto pg = pd->Graphs.begin(); pg != pd->Graphs.end(); ++pg)
        {
            if(e != pg.operator->()) pg->isSelected = false;

            // test markers of graph
            foreach(Marker *pm, pg->Markers)
                if(e != pm) pm->isSelected = false;
        }

    }

    // test all paintings
    for(auto pp = Paintings->begin(); pp != Paintings->end(); ++pp)
        if(e != pp.operator->())  pp->isSelected = false;
}

// ---------------------------------------------------
// Selects elements that lie within the rectangle x1/y1, x2/y2.
int Schematic::selectElements(int x1, int y1, int x2, int y2, bool flag)
{
    int  z=0;   // counts selected elements
    int  cx1, cy1, cx2, cy2;

    // exchange rectangle coordinates to obtain x1 < x2 and y1 < y2
    cx1 = (x1 < x2) ? x1 : x2;
    cx2 = (x1 > x2) ? x1 : x2;
    cy1 = (y1 < y2) ? y1 : y2;
    cy2 = (y1 > y2) ? y1 : y2;
    x1 = cx1;
    x2 = cx2;
    y1 = cy1;
    y2 = cy2;

    // test all components
    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
    {
        pc->Bounding(cx1, cy1, cx2, cy2);
        if(cx1 >= x1) if(cx2 <= x2) if(cy1 >= y1) if(cy2 <= y2)
                    {
                        pc->isSelected = true;
                        z++;
                        continue;
                    }
        if(pc->isSelected &= flag) z++;
    }


    for(auto pw = Wires->begin(); pw != Wires->end(); ++pw)
    {
        if(pw->x1 >= x1) if(pw->x2 <= x2) if(pw->y1 >= y1) if(pw->y2 <= y2)
                    {
                        pw->isSelected = true;
                        z++;
                        continue;
                    }
        if(pw->isSelected &= flag) z++;
    }


    // test all wire labels *********************************
    WireLabel *pl=0;
    for(auto pw = Wires->begin(); pw != Wires->end(); ++pw)
    {
        if(pw->Label)
        {
            pl = pw->Label;
            if(pl->x1 >= x1) if((pl->x1+pl->x2) <= x2)
                    if(pl->y1 >= y1) if((pl->y1+pl->y2) <= y2)
                        {
                            pl->isSelected = true;
                            z++;
                            continue;
                        }
            if(pl->isSelected &= flag) z++;
        }
    }


    // test all node labels *************************************
    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn)
    {
        pl = pn->Label;
        if(pl)
        {
            if(pl->x1 >= x1) if((pl->x1+pl->x2) <= x2)
                    if((pl->y1-pl->y2) >= y1) if(pl->y1 <= y2)
                        {
                            pl->isSelected = true;
                            z++;
                            continue;
                        }
            if(pl->isSelected &= flag) z++;
        }
    }


    // test all diagrams *******************************************
    for(auto pd = Diagrams->begin(); pd != Diagrams->end(); ++pd)
    {
        // test graphs of diagram
        for(auto pg = pd->Graphs.begin(); pg != pd->Graphs.end(); ++pg)
        {
            if(pg->isSelected &= flag) z++;

            // test markers of graph
            foreach(Marker *pm, pg->Markers)
            {
                pm->Bounding(cx1, cy1, cx2, cy2);
                if(cx1 >= x1) if(cx2 <= x2) if(cy1 >= y1) if(cy2 <= y2)
                            {
                                pm->isSelected = true;
                                z++;
                                continue;
                            }
                if(pm->isSelected &= flag) z++;
            }
        }

        // test diagram itself
        pd->Bounding(cx1, cy1, cx2, cy2);
        if(cx1 >= x1) if(cx2 <= x2) if(cy1 >= y1) if(cy2 <= y2)
                    {
                        pd->isSelected = true;
                        z++;
                        continue;
                    }
        if(pd->isSelected &= flag) z++;
    }

    // test all paintings *******************************************
    for(auto pp = Paintings->begin(); pp != Paintings->end(); ++pp)
    {
        pp->Bounding(cx1, cy1, cx2, cy2);
        if(cx1 >= x1) if(cx2 <= x2) if(cy1 >= y1) if(cy2 <= y2)
                    {
                        pp->isSelected = true;
                        z++;
                        continue;
                    }
        if(pp->isSelected &= flag) z++;
    }

    return z;
}

// ---------------------------------------------------
// Selects all markers.
void Schematic::selectMarkers()
{
    for(auto pd = Diagrams->begin(); pd != Diagrams->end(); ++pd)
        for(auto pg = pd->Graphs.begin(); pg != pd->Graphs.end(); ++pg)
            foreach(Marker *pm, pg->Markers)
                pm->isSelected = true;
}

// ---------------------------------------------------
// For moving elements: If the moving element is connected to a not
// moving element, insert two wires. If the connected element is already
// a wire, use this wire. Otherwise create new wire.
void Schematic::newMovingWires(QVector<Element *> &p, Node *pn, int pos)
{
    Element *pe;

    if(pn->State & 8)  // Were new wires already inserted ?
        return;
    pn->State |= 8;

    for (;;)
    {
        if(pn->State & 16)  // node was already worked on
            break;

        pe = pn->Connections.front();
        if(pe == 0)  return;

        if(pn->Connections.count() > 1)
            break;
        if(pe->Type != isWire)  // is it connected to exactly one wire ?
            break;

        // .................................................
        long  mask = 1, invMask = 3;
        Wire *pw2=0, *pw = (Wire*)pe;

        Node *pn2 = pw->Port1;
        if(pn2 == pn) pn2 = pw->Port2;

        if(pn2->Connections.count() == 2) // two existing wires connected ?
            if((pn2->State & (8+4)) == 0)
            {
                Element *pe2 = pn2->Connections.front();
                if(pe2 == pe) pe2 = pn2->Connections.back();
                // connected wire connected to exactly one wire ?
                if(pe2->Type == isWire)
                    pw2  = (Wire*)pe2;
            }

        // .................................................
        // reuse one wire

        Wires->release(pw);
        p.insert(p.begin() + pos, pw);
        pw->Port1->removeConnection(pw);   // remove connection 1
        pw->Port1->State |= 16+4;
        pw->Port2->removeConnection(pw);   // remove connection 2
        pw->Port2->State |= 16+4;

        if(pw->isHorizontal()) mask = 2;

        if(pw2 == 0)    // place new wire between component and old wire
        {
            pn = pn2;
            mask ^= 3;
            invMask = 0;
        }

        if(pw->Port1 != pn)
        {
            pw->Port1->State |= mask;
            pw->Port1 = (Node*)(uintptr_t)mask;
            pw->Port2->State |= invMask;
            pw->Port2 = (Node*)(uintptr_t)invMask;  // move port 2 completely
        }
        else
        {
            pw->Port1->State |= invMask;
            pw->Port1 = (Node*)(uintptr_t)invMask;
            pw->Port2->State |= mask;
            pw->Port2 = (Node*)(uintptr_t)mask;
        }

        invMask ^= 3;
        // .................................................
        // create new wire ?
        if(pw2 == 0)
        {
            if(pw->Port1 != (Node*)(uintptr_t)mask)
                p.insert(p.begin() + pos,
                         new Wire(pw->x2, pw->y2, pw->x2, pw->y2, (Node*)(uintptr_t)mask, (Node*)(uintptr_t)invMask));
            else
                p.insert(p.begin() + pos,
                         new Wire(pw->x1, pw->y1, pw->x1, pw->y1, (Node*)(uintptr_t)mask, (Node*)(uintptr_t)invMask));
            return;
        }


        // .................................................
        // reuse a second wire
        Wires->release (pw2);
        p.insert(p.begin() + pos, pw2);
        pw2->Port1->removeConnection(pw2);   // remove connection 1
        pw2->Port1->State |= 16+4;
        pw2->Port2->removeConnection(pw2);   // remove connection 2
        pw2->Port2->State |= 16+4;

        if(pw2->Port1 != pn2)
        {
            pw2->Port1 = (Node*)0;
            pw2->Port2->State |= mask;
            pw2->Port2 = (Node*)(uintptr_t)mask;
        }
        else
        {
            pw2->Port1->State |= mask;
            pw2->Port1 = (Node*)(uintptr_t)mask;
            pw2->Port2 = (Node*)0;
        }
        return;
    }

    // only x2 moving
    p.insert(p.begin() + pos, new Wire(pn->cx, pn->cy, pn->cx, pn->cy, (Node*)0, (Node*)1));
    // x1, x2, y2 moving
    p.insert(p.begin() + pos, new Wire(pn->cx, pn->cy, pn->cx, pn->cy, (Node*)1, (Node*)3));
}

// ---------------------------------------------------
// For moving of elements: Copies all selected elements into the
// list 'p' and deletes them from the document.
// BUG: does not (only) copy, as the name suggests.
//      cannot be used to make copies.
// returns the number of "copied" _Markers_ only
int Schematic::copySelectedElements(QVector<Element *> &p)
{
    int i, count = 0;


    // test all components *********************************
    // Insert components before wires in order to prevent short-cut removal.
    for(auto pcn = Components->begin(); pcn != Components->end(); ) {
        auto pc = pcn++;
        if(pc->isSelected)
        {
            p.append(pc.operator->());
            count++;

            // delete all port connections
            for (auto pp = pc->Ports.begin(); pp != pc->Ports.end(); ++pp) {
                pp->Connection->removeConnection(pc.operator->());
                pp->Connection->State = 4;
            }

            Components->release(pc);   // take component out of the document
        }
    }

    // test all wires and wire labels ***********************
    for(auto pwn = Wires->begin(); pwn != Wires->end(); )
    {
        auto pw = pwn++;
        if(pw->Label) if(pw->Label->isSelected)
                p.append(pw->Label);

        if(pw->isSelected)
        {
            p.append(pw.operator->());

            pw->Port1->removeConnection(pw.operator->());   // remove connection 1
            pw->Port1->State = 4;
            pw->Port2->removeConnection(pw.operator->());   // remove connection 2
            pw->Port2->State = 4;
            Wires->release(pw);
        }
    }

    // ..............................................
    // Inserts wires, if a connection to a not moving element is found.
    // The order of the "for"-loops is important to guarantee a stable
    // operation: components, new wires, old wires
    for(i = 0; i < count; i++)
    {
        Component *pc = (Component *)p[i];
        for(auto pp = pc->Ports.begin(); pp != pc->Ports.end(); ++pp)
            newMovingWires(p, pp->Connection, count);
    }

    for( ; i < p.count(); ++i) {
      if (p[i]->isSelected) {
        break;
      }
    }

    QVector<Wire *> wiresToHandle;
    for ( ; i < p.count(); ++i) {
        if (p[i]->Type == isWire) {
            wiresToHandle.append ((Wire *)p[i]);
        }
    }
    foreach (Wire *pw, wiresToHandle) {
        newMovingWires(p, pw->Port1, count);
        newMovingWires(p, pw->Port2, count);
    }

    // ..............................................
    // delete the unused nodes
    for(auto pnn = Nodes->begin(); pnn != Nodes->end(); )
    {
        auto pn = pnn++;
        if(pn->State & 8)
            if(pn->Connections.count() == 2)
                if(oneTwoWires(pn.operator->()))    // if possible, connect two wires to one
                {
                    continue;
                }

        if(pn->Connections.count() == 0)
        {
            if(pn->Label)
            {
                pn->Label->Type = isMovingLabel;
                if(pn->State & 1)
                {
                    if(!(pn->State & 2)) pn->Label->Type = isHMovingLabel;
                }
                else if(pn->State & 2) pn->Label->Type = isVMovingLabel;
                p.append(pn->Label);    // do not forget the node labels
            }
            Nodes->erase(pn);
            continue;
        }

        pn->State = 0;
    }

    // test all node labels
    // do this last to avoid double copying
    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn)
        if(pn->Label) if(pn->Label->isSelected)
                p.append(pn->Label);


    // test all paintings **********************************
    for(auto ppan = Paintings->begin(); ppan != Paintings->end(); ) {
        auto ppa = ppan++;
        if(ppa->isSelected)
        {
            p.append(ppa.operator->());
            Paintings->release(ppa);
        }
    }

    count = 0;  // count markers now
    // test all diagrams **********************************
    for(auto pdn = Diagrams->begin(); pdn != Diagrams->end(); ) {
        auto pd = pdn++;
        if(pd->isSelected)
        {
            p.append(pd.operator->());
            Diagrams->release(pd);
        }
        else
        {
            for(auto pg = pd->Graphs.begin(); pg != pd->Graphs.end(); ++pg)
            {
                QMutableListIterator<Marker *> im(pg->Markers);
                Marker *pm;
                while (im.hasNext())
                {
                    pm = im.next();
                    if(pm->isSelected)
                    {
                        count++;
                        p.append(pm);
                    }
                }
            }
        }
    }

    return count;
}

// ---------------------------------------------------
bool Schematic::copyComps2WiresPaints(int& x1, int& y1, int& x2, int& y2,
                                      QVector<Element *> &ElementCache)
{
    x1=INT_MAX;
    y1=INT_MAX;
    x2=INT_MIN;
    y2=INT_MIN;
    copyLabels(x1, y1, x2, y2, ElementCache);   // must be first of all !
    copyComponents2(x1, y1, x2, y2, ElementCache);
    copyWires(x1, y1, x2, y2, ElementCache);
    copyPaintings(x1, y1, x2, y2, ElementCache);

    if(y1 == INT_MAX) return false;  // no element selected
    return true;
}

// ---------------------------------------------------
// Used in "aligning()", "distributeHorizontal()", "distributeVertical()".
int Schematic::copyElements(int& x1, int& y1, int& x2, int& y2,
                            QVector<Element *> &ElementCache)
{
    int bx1, by1, bx2, by2;

    x1=INT_MAX;
    y1=INT_MAX;
    x2=INT_MIN;
    y2=INT_MIN;
    // take components and wires out of list, check their boundings
    int number = copyComponents(x1, y1, x2, y2, ElementCache);
    number += copyWires(x1, y1, x2, y2, ElementCache);

    // find upper most selected diagram
    for(auto pd = Diagrams->end(); pd != Diagrams->begin(); ) {
        --pd;
        if(pd->isSelected)
        {
            pd->Bounding(bx1, by1, bx2, by2);
            if(bx1 < x1) x1 = bx1;
            if(bx2 > x2) x2 = bx2;
            if(by1 < y1) y1 = by1;
            if(by2 > y2) y2 = by2;
            ElementCache.append(pd.operator->());
            number++;
        }
    }

    // find upper most selected painting
    for(auto pp = Paintings->end(); pp != Paintings->begin(); ) {
        --pp;
        if(pp->isSelected)
        {
            pp->Bounding(bx1, by1, bx2, by2);
            if(bx1 < x1) x1 = bx1;
            if(bx2 > x2) x2 = bx2;
            if(by1 < y1) y1 = by1;
            if(by2 > y2) y2 = by2;
            ElementCache.append(pp.operator->());
            number++;
        }
    }

    return number;
}

// ---------------------------------------------------
// Deletes all selected elements.
bool Schematic::deleteElements()
{
    bool sel = false;

    for (auto pcn = Components->begin(); pcn != Components->end(); )
    {
        auto pc = pcn++;
        if(pc->isSelected)
        {
            deleteComp(pc);
            sel = true;
        }
    }

    for (auto pwn = Wires->begin(); pwn != Wires->end(); )
    {
        auto pw = pwn++;
        if(pw->Label)
            if(pw->Label->isSelected)
            {
                delete pw->Label;
                pw->Label = 0;
                sel = true;
            }

        if(pw->isSelected)
        {
            deleteWire(pw);
            sel = true;
        }
    }

    // all selected labels on nodes ***************************
    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn) {
        if(pn->Label)
            if(pn->Label->isSelected)
            {
                delete pn->Label;
                pn->Label = 0;
                sel = true;
            }
    }

    for (auto pdn = Diagrams->begin(); pdn != Diagrams->end(); )
    {
        auto pd = pdn++;
        if(pd->isSelected)
        {
            Diagrams->erase(pd);
            sel = true;
        }
        else
        {
            bool wasGraphDeleted = false;
            // all graphs of diagram

            auto ign = pd->Graphs.begin();

            for (; ign != pd->Graphs.end(); )
            {
                auto ig = ++ign;
                // all markers of diagram
                QMutableListIterator<Marker *> im(ig->Markers);
                Marker *pm;
                while (im.hasNext())
                {
                    pm = im.next();
                    if(pm->isSelected)
                    {
                        im.remove();
                        sel = true;
                    }
                }

                if(ig->isSelected)
                {
                    pd->Graphs.erase(ig);
                    sel = wasGraphDeleted = true;
                }
            }
            if(wasGraphDeleted)
                pd->recalcGraphData();  // update diagram (resize etc.)

        } //else
    }

    for (auto ppn = Paintings->begin(); ppn != Paintings->end(); )
    {
        auto pp = ppn++;
        if(pp->isSelected)
            if(pp->Name.at(0) != '.')    // do not delete "PortSym", "ID_text"
            {
                sel = true;
                Paintings->erase(pp);
            }
    }

    if(sel)
    {
        sizeOfAll(UsedX1, UsedY1, UsedX2, UsedY2);   // set new document size
        setChanged(sel, true);
    }
    return sel;
}

// ---------------------------------------------------
/*!
 * \brief Schematic::aligning align selected elements.
 * \param Mode: top, bottom, left, right, center vertical, center horizontal
 * \return True if aligned
 */
bool Schematic::aligning(int Mode)
{
    int x1, y1, x2, y2;
    int bx1, by1, bx2, by2, *bx=0, *by=0, *ax=0, *ay=0;
    QVector<Element *> ElementCache;
    int count = copyElements(x1, y1, x2, y2, ElementCache);
    if(count < 1) return false;


    ax = ay = &x2;  // = 0
    switch(Mode)
    {
    case 0:  // align top
        bx = &x1;
        by = &by1;
        y2 = 1;
        break;
    case 1:  // align bottom
        bx = &x1;
        y1 = y2;
        by = &by2;
        y2 = 1;
        break;
    case 2:  // align left
        by = &y1;
        bx = &bx1;
        y2 = 1;
        break;
    case 3:  // align right
        by = &y1;
        x1 = x2;
        bx = &bx2;
        y2 = 1;
        break;
    case 4:  // center horizontally
        x1 = (x2+x1) / 2;
        by = &x2;  // = 0
        ax = &bx1;
        bx = &bx2;
        y1 = 0;
        y2 = 2;
        break;
    case 5:  // center vertically
        y1 = (y2+y1) / 2;
        bx = &x2;  // = 0
        ay = &by1;
        by = &by2;
        x1 = 0;
        y2 = 2;
        break;
    }
    x2 = 0;

    Wire      *pw;
    Component *pc;
    // re-insert elements
    // Go backwards in order to insert node labels before its component.
    for (auto i = ElementCache.end(); i != ElementCache.begin(); ) {
        --i;
        Element *pe = *i;
        switch(pe->Type)
        {
        case isComponent:
        case isAnalogComponent:
        case isDigitalComponent:
            pc = (Component*)pe;
            pc->Bounding(bx1, by1, bx2, by2);
            pc->setCenter(x1-((*bx)+(*ax))/y2, y1-((*by)+(*ay))/y2, true);
            insertRawComponent(ComponentList::holder(pc));
            break;

        case isWire:
            pw = (Wire*)pe;
            bx1 = pw->x1;
            by1 = pw->y1;
            bx2 = pw->x2;
            by2 = pw->y2;
            pw->setCenter(x1-((*bx)+(*ax))/y2, y1-((*by)+(*ay))/y2, true);
//        if(pw->Label) {  }
            insertWire(WireList::holder(pw));
            break;

        case isDiagram:
            // Should the axis label be counted for ? I guess everyone
            // has a different opinion.
//        ((Diagram*)pe)->Bounding(bx1, by1, bx2, by2);

            // Take size without axis label.
            bx1 = ((Diagram*)pe)->cx;
            by2 = ((Diagram*)pe)->cy;
            bx2 = bx1 + ((Diagram*)pe)->x2;
            by1 = by2 - ((Diagram*)pe)->y2;
            ((Diagram*)pe)->setCenter(x1-((*bx)+(*ax))/y2, y1-((*by)+(*ay))/y2, true);
            break;

        case isPainting:
            ((Painting*)pe)->Bounding(bx1, by1, bx2, by2);
            ((Painting*)pe)->setCenter(x1-((*bx)+(*ax))/y2, y1-((*by)+(*ay))/y2, true);
            break;

        case isNodeLabel:
            if(((Element*)(((WireLabel*)pe)->pOwner))->Type & isComponent)
            {
                pc = (Component*)(((WireLabel*)pe)->pOwner);
                pc->Bounding(bx1, by1, bx2, by2);
            }
            else
            {
                pw = (Wire*)(((WireLabel*)pe)->pOwner);
                bx1 = pw->x1;
                by1 = pw->y1;
                bx2 = pw->x2;
                by2 = pw->y2;
            }
            ((WireLabel*)pe)->cx += x1-((*bx)+(*ax))/y2;
            ((WireLabel*)pe)->cy += y1-((*by)+(*ay))/y2;
            insertNodeLabel((WireLabel*)pe);
            break;

        default:
            ;
        }
    }

    ElementCache.clear();
    if(count < 2) return false;

    setChanged(true, true);
    return true;
}

namespace
{
    struct SortElementPointerByX
    {
        bool operator() (Element *a, Element *b) const
        {
            int bx1, by1, bx2, by2;
            a->getCenter(bx1, by1);
            b->getCenter(bx2, by2);
            return bx1 < bx2 || (bx1 == bx2 && by1 < by2);
        }
    };

    struct SortElementPointerByY
    {
        bool operator() (Element *a, Element *b) const
        {
            int bx1, by1, bx2, by2;
            a->getCenter(bx1, by1);
            b->getCenter(bx2, by2);
            return by1 < by2 || (by1 == by2 && bx1 < bx2);
        }
    };
}

/*!
 * \brief Schematic::distributeHorizontal sort selection horizontally
 * \return True if sorted
 */
bool Schematic::distributeHorizontal()
{
    int x1, y1, x2, y2;
    QVector<Element *> ElementCache;
    int count = copyElements(x1, y1, x2, y2, ElementCache);
    if(count < 1) return false;

    WireLabel *pl;
    // Node labels are not counted for, so put them to the end.
    /*  for(pe = ElementCache.last(); pe != 0; pe = ElementCache.prev())
        if(pe->Type == isNodeLabel) {
          ElementCache.append(pe);
          ElementCache.removeRef(pe);
        }*/

    //  sort the elements by x position
    std::sort(ElementCache.begin(), ElementCache.end(), SortElementPointerByX());

    ElementCache.last()->getCenter(x2, y2);
    ElementCache.first()->getCenter(x1, y1);
    Wire *pw;
    int x = x2;
    int dx=0;
    if(count > 1) dx = (x2-x1)/(count-1);
    // re-insert elements and put them at right position
    // Go backwards in order to insert node labels before its component.
    for (auto el = ElementCache.end(); el != ElementCache.begin(); ) {
        --el;
        Element *pe = *el;
        switch(pe->Type)
        {
        case isComponent:
        case isAnalogComponent:
        case isDigitalComponent:
            pe->cx = x;
            insertRawComponent(ComponentList::holder((Component*)pe));
            break;

        case isWire:
            pw = (Wire*)pe;
            if(pw->isHorizontal())
            {
                x1 = pw->x2 - pw->x1;
                pw->x1 = x - (x1 >> 1);
                pw->x2 = pw->x1 + x1;
            }
            else  pw->x1 = pw->x2 = x;
//        if(pw->Label) {	}
            insertWire(WireList::holder(pw));
            break;

        case isDiagram:
            pe->cx = x - (pe->x2 >> 1);
            break;

        case isPainting:
            {
                int bx1, by1;
                pe->getCenter(bx1, by1);
                pe->setCenter(x, by1, false);
            }
            break;

        case isNodeLabel:
            pl = (WireLabel*)pe;
            if(((Element*)(pl->pOwner))->Type & isComponent)
                pe->cx += x - ((Component*)(pl->pOwner))->cx;
            else
            {
                pw = (Wire*)(pl->pOwner);
                if(pw->isHorizontal())
                {
                    x1 = pw->x2 - pw->x1;
                    pe->cx += x - (x1 >> 1) - pw->x1;
                }
                else  pe->cx += x - pw->x1;
            }
            insertNodeLabel(pl);
            x += dx;
            break;

        default:
            ;
        }
        x -= dx;
    }

    ElementCache.clear();
    if(count < 2) return false;

    setChanged(true, true);
    return true;
}

/*!
 * \brief Schematic::distributeVertical sort selection vertically.
 * \return True if sorted
 */
bool Schematic::distributeVertical()
{
    int x1, y1, x2, y2;
    QVector<Element *> ElementCache;

    int count = copyElements(x1, y1, x2, y2, ElementCache);
    if(count < 1) return false;

    //  sort the elements by y coordinate
    std::sort(ElementCache.begin(), ElementCache.end(), SortElementPointerByY());

    ElementCache.last()->getCenter(x2, y2);
    ElementCache.first()->getCenter(x1, y1);
    Wire *pw;
    int y  = y2;
    int dy=0;
    if(count > 1) dy = (y2-y1)/(count-1);
    // re-insert elements and put them at right position
    // Go backwards in order to insert node labels before its component.
    for (auto el = ElementCache.end(); el != ElementCache.begin(); )
    {
        --el;
        Element *pe = *el;
        switch(pe->Type)
        {
        case isComponent:
        case isAnalogComponent:
        case isDigitalComponent:
            pe->cy = y;
            insertRawComponent(ComponentList::holder((Component*)pe));
            break;

        case isWire:
            pw = (Wire*)pe;
            if(pw->isHorizontal())  pw->y1 = pw->y2 = y;
            else
            {
                y1 = pw->y2 - pw->y1;
                pw->y1 = y - (y1 >> 1);
                pw->y2 = pe->y1 + y1;
            }
//        if(pw->Label) {	}
            insertWire(WireList::holder(pw));
            break;

        case isDiagram:
            pe->cy = y + (pe->y2 >> 1);
            break;

        case isPainting:
            {
                int bx1, by1;
                pe->getCenter(bx1, by1);
                pe->setCenter(bx1, y, false);
            }
            break;

        case isNodeLabel:
            if(((Element*)(((WireLabel*)pe)->pOwner))->Type & isComponent)
                pe->cy += y - ((Component*)(((WireLabel*)pe)->pOwner))->cy;
            else
            {
                pw = (Wire*)(((WireLabel*)pe)->pOwner);
                if(!pw->isHorizontal())
                {
                    y1 = pw->y2 - pw->y1;
                    pe->cy += y - (y1 >> 1) - pw->y1;
                }
                else  pe->cy += y - pw->y1;
            }
            insertNodeLabel((WireLabel*)pe);
            y += dy;
            break;

        default:
            ;
        }
        y -= dy;
    }

    ElementCache.clear();
    if(count < 2) return false;

    setChanged(true, true);
    return true;
}


/* *******************************************************************
   *****                                                         *****
   *****                Actions with components                  *****
   *****                                                         *****
   ******************************************************************* */

// Finds the correct number for power sources, subcircuit ports and
// digital sources and sets them accordingly.
void Schematic::setComponentNumber(Component *c)
{
    if (c->Props.empty()) {
      return;
    }

    Property &pp = c->Props.front();
    if(pp.Name != "Num") return;

    int n=1;
    QString s = pp.Value;
    QString cSign = c->obsolete_model_hack();
    auto pc = Components->begin();
    // First look, if the port number already exists.
    for( ; pc != Components->end(); )
        if(pc->obsolete_model_hack() == cSign)
            if(pc->Props.front().Value == s) break;
    if (pc == Components->end()) return;   // was port number not yet in use ?

    // Find the first free number.
    do
    {
        s  = QString::number(n);
        // look for existing ports and their numbers
        for(pc = Components->begin(); pc != Components->end(); ++pc)
            if(pc->obsolete_model_hack() == cSign)
                if(pc->Props.front().Value == s) break;

        n++;
    }
    while(pc != Components->end());     // found not used component number
    pp.Value = s; // set new number
}

// ---------------------------------------------------
void Schematic::insertComponentNodes(Component *c, bool noOptimize)
{
    // simulation components do not have ports
    if (c->Ports.empty()) return;

    // connect every node of the component to corresponding schematic node
    for (auto pp = c->Ports.begin(); pp != c->Ports.end(); ++pp)
        pp->Connection = insertNode(pp->x+c->cx, pp->y+c->cy, c);

    if(noOptimize)  return;

    // if component over wire then delete this wire
    auto pp = c->Ports.begin();
    // omit the first element
    ++pp;
    while (pp != c->Ports.end())
    {
        Node *pn = pp->Connection;
        for(auto i = pn->Connections.begin(); i != pn->Connections.end(); ++i)
        {
            Element *pe = *i;
            if(pe->Type == isWire)
            {
                QVector<Element *> *pL;
                if(((Wire*)pe)->Port1 == pn)  pL = &(((Wire*)pe)->Port2->Connections);
                else  pL = &(((Wire*)pe)->Port1->Connections);

                for(auto pe1 = pL->begin(); pe1 != pL->end(); ++pe1)
                    if(*pe1 == c)
                    {
                        auto pw = Wires->find((Wire *)pe);
                        assert(pw != Wires->end());
                        deleteWire(pw);
                        break;
                    }
            }
        }
        ++pp;
    }
}

// ---------------------------------------------------
// Used for example in moving components.
void Schematic::insertRawComponent(const ComponentList::holder &c, bool noOptimize)
{
    // connect every node of component to corresponding schematic node
    insertComponentNodes(c.get(), noOptimize);
    Components->append(c);

    // a ground symbol erases an existing label on the wire line
    if(c->obsolete_model_hack() == "GND") { // BUG.
        c->gnd_obsolete_model_override_hack("x");
        Element *pe = getWireLabel(c->Ports.first().Connection);
        if(pe) if((pe->Type & isComponent) == 0)
            {
                delete ((Conductor*)pe)->Label;
                ((Conductor*)pe)->Label = 0;
            }
        c->gnd_obsolete_model_override_hack("GND");
    }
}

// ---------------------------------------------------
void Schematic::recreateComponent(Component *Comp)
{
    QVector<WireLabel *> plMem;
    int PortCount = Comp->Ports.count();

    if(PortCount > 0)
    {
        // Save the labels whose node is not connected to somewhere else.
        // Otherwise the label would be deleted.
        for (auto pp = Comp->Ports.begin(); pp != Comp->Ports.end(); ++pp)
            if(pp->Connection->Connections.count() < 2)
            {
                plMem.push_back(pp->Connection->Label);
                pp->Connection->Label = 0;
            }
            else plMem.push_back(0);
    }


    int x = Comp->tx, y = Comp->ty;
    int x1 = Comp->x1, x2 = Comp->x2, y1 = Comp->y1, y2 = Comp->y2;
    QString tmp = Comp->name();    // is sometimes changed by "recreate"
    Comp->recreate(this);   // to apply changes to the schematic symbol
    Comp->obsolete_name_override_hack(tmp);
    if(x < x1)
        x += Comp->x1 - x1;
    else if(x > x2)
        x += Comp->x2 - x2;
    if(y < y1)
        y += Comp->y1 - y1;
    else if(y > y2)
        y += Comp->y2 - y2;
    Comp->tx = x;
    Comp->ty = y;


    if(PortCount > 0)
    {
        // restore node labels
        auto pl = plMem.begin();
        for (auto pp = Comp->Ports.begin(); pp != Comp->Ports.end(); ++pp)
        {
            if(*pl != 0)
            {
                (*pl)->cx = pp->Connection->cx;
                (*pl)->cy = pp->Connection->cy;
                placeNodeLabel(*pl);
            }
            ++pl;
            if((--PortCount) < 1)  break;
        }
        for( ; PortCount > 0; PortCount--)
        {
            delete (*pl);  // delete not needed labels
            ++pl;
        }
    }
}

// ---------------------------------------------------
void Schematic::insertComponent(Component *c)
{
    // connect every node of component to corresponding schematic node
    insertComponentNodes(c, false);

    bool ok;
    QString s;
    int  max=1, len = c->name().length(), z;
    if(c->name().isEmpty()) { // BUG
        // a ground symbol erases an existing label on the wire line
        if(c->obsolete_model_hack() == "GND") { // BUG
            c->gnd_obsolete_model_override_hack("x");
            Element *pe = getWireLabel(c->Ports.first().Connection);
            if(pe) if((pe->Type & isComponent) == 0)
                {
                    delete ((Conductor*)pe)->Label;
                    ((Conductor*)pe)->Label = 0;
                }
            c->gnd_obsolete_model_override_hack("GND");
        }
    }
    else
    {
        // determines the name by looking for names with the same
        // prefix and increment the number
        for(auto pc = Components->begin(); pc != Components->end(); ++pc)
            if(pc->name().left(len) == c->name())
            {
                s = pc->name().right(pc->name().length()-len);
                z = s.toInt(&ok);
                if(ok) if(z >= max) max = z + 1;
            }
        c->obsolete_name_override_hack(
	    c->name() + QString::number(max));  // create name with new number
    }

    setComponentNumber(c); // important for power sources and subcircuit ports
    Components->append(c);
}

// ---------------------------------------------------
void Schematic::activateCompsWithinRect(int x1, int y1, int x2, int y2)
{
    bool changed = false;
    int  cx1, cy1, cx2, cy2, a;
    // exchange rectangle coordinates to obtain x1 < x2 and y1 < y2
    cx1 = (x1 < x2) ? x1 : x2;
    cx2 = (x1 > x2) ? x1 : x2;
    cy1 = (y1 < y2) ? y1 : y2;
    cy2 = (y1 > y2) ? y1 : y2;
    x1 = cx1;
    x2 = cx2;
    y1 = cy1;
    y2 = cy2;


    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
    {
        pc->Bounding(cx1, cy1, cx2, cy2);
        if(cx1 >= x1) if(cx2 <= x2) if(cy1 >= y1) if(cy2 <= y2)
                    {
                        a = pc->isActive - 1;

                        if(pc->Ports.count() > 1)
                        {
                            if(a < 0)  a = 2;
                            pc->isActive = a;    // change "active status"
                        }
                        else
                        {
                            a &= 1;
                            pc->isActive = a;    // change "active status"
                            if(a == COMP_IS_ACTIVE)  // only for active (not shorten)
                                if(pc->obsolete_model_hack() == "GND"){
                                    // if existing, delete label on wire line
                                    oneLabel(pc->Ports.first().Connection);
				}
                        }
                        changed = true;
                    }
    }

    if(changed)  setChanged(true, true);
}

// ---------------------------------------------------
bool Schematic::activateSpecifiedComponent(int x, int y)
{
    int x1, y1, x2, y2, a;
    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
    {
        pc->Bounding(x1, y1, x2, y2);
        if(x >= x1) if(x <= x2) if(y >= y1) if(y <= y2)
                    {
                        a = pc->isActive - 1;

                        if(pc->Ports.count() > 1)
                        {
                            if(a < 0)  a = 2;
                            pc->isActive = a;    // change "active status"
                        }
                        else
                        {
                            a &= 1;
                            pc->isActive = a;    // change "active status"
                            if(a == COMP_IS_ACTIVE)  // only for active (not shorten)
                                if(pc->obsolete_model_hack() == "GND"){
				  // if existing, delete label on wire line
                                    oneLabel(pc->Ports.first().Connection);
				}
                        }
                        setChanged(true, true);
                        return true;
                    }
    }
    return false;
}

// ---------------------------------------------------
bool Schematic::activateSelectedComponents()
{
    int a;
    bool sel = false;
    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
        if(pc->isSelected)
        {
            a = pc->isActive - 1;

            if(pc->Ports.count() > 1)
            {
                if(a < 0)  a = 2;
                pc->isActive = a;    // change "active status"
            }
            else
            {
                a &= 1;
                pc->isActive = a;    // change "active status"
                if(a == COMP_IS_ACTIVE)  // only for active (not shorten)
                    if(pc->obsolete_model_hack() == "GND"){
		      // if existing, delete label on wire line
                        oneLabel(pc->Ports.first().Connection);
		    }
            }
            sel = true;
        }

    if(sel) setChanged(true, true);
    return sel;
}

// ---------------------------------------------------
// Sets the component ports anew. Used after rotate, mirror etc.
void Schematic::setCompPorts(Component *pc)
{
    WireLabel *pl;
    QVector<WireLabel *> LabelCache;

    for (auto pp = pc->Ports.begin(); pp != pc->Ports.end(); ++pp)
    {
        pp->Connection->removeConnection((Element*)pc);// delete connections
        switch(pp->Connection->Connections.count())
        {
        case 0:
            pl = pp->Connection->Label;
            if(pl)
            {
                LabelCache.append(pl);
                pl->cx = pp->x + pc->cx;
                pl->cy = pp->y + pc->cy;
            }
            Nodes->erase(pp->Connection);
            break;
        case 2:
            oneTwoWires(pp->Connection); // try to connect two wires to one
        default:
            ;
        }
    }

    // Re-connect component node to schematic node. This must be done completely
    // after the first loop in order to avoid problems with node labels.
    for (auto pp = pc->Ports.begin(); pp != pc->Ports.end(); ++pp)
        pp->Connection = insertNode(pp->x+pc->cx, pp->y+pc->cy, pc);

    for(auto pl = LabelCache.begin(); pl != LabelCache.end(); ++pl)
        insertNodeLabel(*pl);
}

// ---------------------------------------------------
// Returns a pointer of the component on whose text x/y points.
Component* Schematic::selectCompText(int x_, int y_, int& w, int& h)
{
    int a, b, dx, dy;
    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
    {
        a = pc->cx + pc->tx;
        if(x_ < a)  continue;
        b = pc->cy + pc->ty;
        if(y_ < b)  continue;

        pc->textSize(dx, dy);
        if(x_ > a+dx)  continue;
        if(y_ > b+dy)  continue;

        w = dx;
        h = dy;
        return pc.operator->();
    }

    return 0;
}

// ---------------------------------------------------
Component* Schematic::searchSelSubcircuit()
{
    Component *sub=0;
    // test all components
    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
    {
        if(!pc->isSelected) continue;

        if(pc->obsolete_model_hack() != "Sub"){
            if(pc->obsolete_model_hack() != "VHDL")
                if(pc->obsolete_model_hack() != "Verilog") continue;
	}

        if(sub != 0) return 0;    // more than one subcircuit selected
        sub = pc.operator->();
    }
    return sub;
}

// ---------------------------------------------------
Component* Schematic::selectedComponent(int x, int y)
{
    // test all components
    for(auto pc = Components->begin(); pc != Components->end(); ++pc)
        if(pc->getSelected(x, y))
            return pc.operator->();

    return 0;
}

// ---------------------------------------------------
// Deletes the component 'c'.
void Schematic::deleteComp(const ComponentList::iterator &c)
{
    // delete all port connections
    for (auto pn = c->Ports.begin(); pn != c->Ports.end(); ++pn)
        switch(pn->Connection->Connections.count())
        {
        case 1  :
            if(pn->Connection->Label) delete pn->Connection->Label;
            Nodes->erase(pn->Connection);  // delete open nodes
            pn->Connection = 0;		  //  (auto-delete)
            break;
        case 3  :
            pn->Connection->removeConnection(c.operator->());// delete connection
            oneTwoWires(pn->Connection);  // two wires -> one wire
            break;
        default :
            pn->Connection->removeConnection(c.operator->());// remove connection
            break;
        }

    Components->erase(c);
}

// ---------------------------------------------------
int Schematic::copyComponents(int& x1, int& y1, int& x2, int& y2,
                              QVector<Element *> &ElementCache)
{
    int bx1, by1, bx2, by2, count=0;
    // find bounds of all selected components
    for(auto pcn = Components->begin(); pcn != Components->end(); )
    {
        auto pc = pcn++;
        if(pc->isSelected)
        {
            pc->Bounding(bx1, by1, bx2, by2);  // is needed because of "distribute
            if(bx1 < x1) x1 = bx1;             // uniformly"
            if(bx2 > x2) x2 = bx2;
            if(by1 < y1) y1 = by1;
            if(by2 > y2) y2 = by2;

            count++;
            ElementCache.append(pc.operator->());

            // rescue non-selected node labels
            for (auto pp = pc->Ports.begin(); pp != pc->Ports.end(); ++pp)
                if(pp->Connection->Label)
                    if(pp->Connection->Connections.count() < 2)
                    {
                        ElementCache.append(pp->Connection->Label);

                        // Don't set pp->Connection->Label->pOwner=0,
                        // so text position stays unchanged, but
                        // remember component for align/distribute.
                        pp->Connection->Label->pOwner = (Node*)pc.operator->();

                        pp->Connection->Label = 0;
                    }

            deleteComp(pc);
        }
    }
    return count;
}

// ---------------------------------------------------
void Schematic::copyComponents2(int& x1, int& y1, int& x2, int& y2,
                                QVector<Element *> &ElementCache)
{
    // find bounds of all selected components
    for(auto pcn = Components->begin(); pcn != Components->end(); )
    {
        auto pc = pcn++;
        if(pc->isSelected)
        {
            // is better for unsymmetrical components
            if(pc->cx < x1)  x1 = pc->cx;
            if(pc->cx > x2)  x2 = pc->cx;
            if(pc->cy < y1)  y1 = pc->cy;
            if(pc->cy > y2)  y2 = pc->cy;

            ElementCache.append(pc.operator->());

            // rescue non-selected node labels
            for (auto pp = pc->Ports.begin(); pp != pc->Ports.end(); ++pp)
                if(pp->Connection->Label)
                    if(pp->Connection->Connections.count() < 2)
                    {
                        ElementCache.append(pp->Connection->Label);
                        pp->Connection->Label = 0;
                        // Don't set pp->Connection->Label->pOwner=0,
                        // so text position stays unchanged.
                    }

            deleteComp(pc);
        }
    }
}


/* *******************************************************************
   *****                                                         *****
   *****                  Actions with labels                    *****
   *****                                                         *****
   ******************************************************************* */

// Test, if wire connects wire line with more than one label and delete
// all further labels. Also delete all labels if wire line is grounded.
void Schematic::oneLabel(Node *n1)
{
    WireLabel *pl = 0;
    bool named=false;   // wire line already named ?
    QVector<Node *> Cons;

    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn)
        pn->y1 = 0;   // mark all nodes as not checked

    Cons.append(n1);
    n1->y1 = 1;  // mark Node as already checked
    for(int i = 0; i < Cons.size(); ++i)
    {
        Node *pn = Cons[i];
        if(pn->Label)
        {
            if(named)
            {
                delete pn->Label;
                pn->Label = 0;    // erase double names
            }
            else
            {
                named = true;
                pl = pn->Label;
            }
        }

        for(auto j = pn->Connections.begin(); j != pn->Connections.end(); ++j)
        {
            Element *pe = *j;
            if(pe->Type != isWire)
            {
                Component *pc = (Component*)pe;
                if(pc->isActive == COMP_IS_ACTIVE)
                    if(pc->obsolete_model_hack() == "GND")
                    {
                        named = true;
                        if(pl)
                        {
                            pl->pOwner->Label = 0;
                            delete pl;
                        }
                        pl = 0;
                    }
                continue;
            }
            Wire *pw = (Wire*)pe;

            Node *pNode;
            if(pn != pw->Port1) pNode = pw->Port1;
            else pNode = pw->Port2;

            if(pNode->y1) continue;
            pNode->y1 = 1;  // mark Node as already checked
            Cons.append(pNode);

            if(pw->Label)
            {
                if(named)
                {
                    delete pw->Label;
                    pw->Label = 0;    // erase double names
                }
                else
                {
                    named = true;
                    pl = pw->Label;
                }
            }
        }
    }
}

// ---------------------------------------------------
int Schematic::placeNodeLabel(WireLabel *pl)
{
    int x = pl->cx;
    int y = pl->cy;

    // check if new node lies upon an existing node
    auto pn = Nodes->begin();
    for( ; pn != Nodes->end(); ++pn)
        if(pn->cx == x) if(pn->cy == y) break;

    if(pn == Nodes->end())  return -1;

    Element *pe = getWireLabel(pn.operator->());
    if(pe)      // name found ?
    {
        if(pe->Type & isComponent)
        {
            delete pl;
            return -2;  // ground potential
        }

        delete ((Conductor*)pe)->Label;
        ((Conductor*)pe)->Label = 0;
    }

    pn->Label = pl;   // insert node label
    pl->Type = isNodeLabel;
    pl->pOwner = pn.operator->();
    return 0;
}

// ---------------------------------------------------
// Test, if wire line is already labeled and returns a pointer to the
// labeled element.
Element* Schematic::getWireLabel(Node *pn_)
{
    QVector<Node *> Cons;

    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn)
        pn->y1 = 0;   // mark all nodes as not checked

    Cons.append(pn_);
    pn_->y1 = 1;  // mark Node as already checked
    for(int i = 0; i < Cons.size(); ++i)
    {
        Node *pn = Cons[i];
        if(pn->Label) return pn;
        else
            for(auto j = pn->Connections.begin(); j != pn->Connections.end(); ++j)
            {
                Element *pe = *j;
                if(pe->Type != isWire)
                {
                    if(((Component*)pe)->isActive == COMP_IS_ACTIVE)
                        if(((Component*)pe)->obsolete_model_hack() == "GND") return pe;
                    continue;
                }

                Wire *pw = (Wire*)pe;
                if(pw->Label) return pw;

                Node *pNode;
                if(pn != pw->Port1) pNode = pw->Port1;
                else pNode = pw->Port2;

                if(pNode->y1) continue;
                pNode->y1 = 1;  // mark Node as already checked
                Cons.append(pNode);
            }
    }
    return 0;   // no wire label found
}

// ---------------------------------------------------
// Inserts a node label.
void Schematic::insertNodeLabel(WireLabel *pl)
{
    if(placeNodeLabel(pl) != -1)
        return;

    // Go on, if label don't lie on existing node.

    Wire *pw = selectedWire(pl->cx, pl->cy);
    if(pw)    // lies label on existing wire ?
    {
        if(getWireLabel(pw->Port1) == 0)  // wire not yet labeled ?
            pw->setName(pl->Name, pl->initValue, 0, pl->cx, pl->cy);

        delete pl;
        return;
    }


    Node *pn = new Node(pl->cx, pl->cy);
    Nodes->append(pn);

    pn->Label = pl;
    pl->Type  = isNodeLabel;
    pl->pOwner = pn;
}

// ---------------------------------------------------
void Schematic::copyLabels(int& x1, int& y1, int& x2, int& y2,
                           QVector<Element *> &ElementCache)
{
    WireLabel *pl;
    // find bounds of all selected wires
    for(auto pw = Wires->begin(); pw != Wires->end(); ++pw)
    {
        pl = pw->Label;
        if(pl) if(pl->isSelected)
            {
                if(pl->x1 < x1) x1 = pl->x1;
                if(pl->y1-pl->y2 < y1) y1 = pl->y1-pl->y2;
                if(pl->x1+pl->x2 > x2) x2 = pl->x1+pl->x2;
                if(pl->y1 > y2) y2 = pl->y1;
                ElementCache.append(pl);
            }
    }

    for(auto pn = Nodes->begin(); pn != Nodes->end(); ++pn)
    {
        pl = pn->Label;
        if(pl) if(pl->isSelected)
            {
                if(pl->x1 < x1) x1 = pl->x1;
                if(pl->y1-pl->y2 < y1) y1 = pl->y1-pl->y2;
                if(pl->x1+pl->x2 > x2) x2 = pl->x1+pl->x2;
                if(pl->y1 > y2) y2 = pl->y1;
                ElementCache.append(pl);
                pl->pOwner->Label = 0;   // erase connection
                pl->pOwner = 0;
            }
    }
}


/* *******************************************************************
   *****                                                         *****
   *****                Actions with paintings                   *****
   *****                                                         *****
   ******************************************************************* */

Painting* Schematic::selectedPainting(float fX, float fY)
{
    float Corr = 5.0 / Scale; // size of line select

    for(auto pp = Paintings->begin(); pp != Paintings->end(); ++pp)
        if(pp->getSelected(fX, fY, Corr))
            return pp.operator->();

    return 0;
}

// ---------------------------------------------------
void Schematic::copyPaintings(int& x1, int& y1, int& x2, int& y2,
                              QVector<Element *> &ElementCache)
{
    int bx1, by1, bx2, by2;
    // find boundings of all selected paintings
    for(auto ppn = Paintings->begin(); ppn != Paintings->end(); ) {
        auto pp = ppn++;
        if(pp->isSelected)
        {
            pp->Bounding(bx1, by1, bx2, by2);
            if(bx1 < x1) x1 = bx1;
            if(bx2 > x2) x2 = bx2;
            if(by1 < y1) y1 = by1;
            if(by2 > y2) y2 = by2;

            ElementCache.append(pp.operator->());
            Paintings->release(pp);
        }
    }
}

// vim:ts=8:sw=2:noet
