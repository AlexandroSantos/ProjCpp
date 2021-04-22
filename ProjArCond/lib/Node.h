#ifndef NODE_H_
#define NODE_H_

/*
   File Node.h

   Class Node

   Class Node stores integer values for a linked list.
   
   This file has the Node's interface (header file).

   Eduardo Augusto Bezerra <eduardo.bezerra@ufsc.br>
   Departamento de Engenharia Eletrica

   Data da criacao: Abril de 2006.
   Data da ultima alteracao: 8 de outubro de 2015.
   
   Adaptado por: Alexandro Vandeley dos Santos
   Data da ultima alteracao: 11 de abril de 2021. 

*/
#include "header.h"

struct Mens{
	int ID;
	DtTimecc dt;	
	int oper;
}; 

class Node {

    Mens val;
    Node* next;

  public:
         
    Node(Mens dat, Node* nxt);
    Mens getVal();
    Node* getNext();
    void setVal(Mens dat);
    void setNext(Node* nxt);
};

#endif //NODE_H_
