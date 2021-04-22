/*
   File List.cpp

   Class List stores Nodes (class Node) in a linked list.
   
   This file has the implementation for class List

   Class list is definition for a linked list, having the following operations:

   1. Initialize the list to an empty list.
   2. Free the nodes of a list.
   3. Determine whether a list is empty.
   4. Add a node with a given value into the list following
      the first node with another given value.   
   5. Add a node with a given value to the end of the list.
   6. Delete the first node from the list.

   Eduardo Augusto Bezerra <eduardo.bezerra@ufsc.br>
   Departamento de Engenharia Eletrica

   Data da criacao: Abril de 2006.
   Data da ultima alteracao: 8 de outubro de 2015.
   
   Adaptado por: Alexandro Vandeley dos Santos
   Data da ultima alteracao: 11 de abril de 2021. 

*/

#include "List.h"

List::List() {
   head = 0;   
}

List::~List() {
  Node* cursor = head;
  while(head) {
    cursor = cursor->getNext();
    delete head;
    head = cursor;
  }
  head = 0; // Officially empty
}

void List::insertAfterLast(Mens dat) {
  Node* p = head;
  Node* q = head;

  if (head == 0)
     head = new Node(dat, head);
  else {
     while (q != 0) {
        p = q;
        q = p->getNext();
     }
     p->setNext(new Node(dat,0));
  }  
}

Mens List::readFirst() {
   return head->getVal(); 
}

Mens List::removeFirst() {
  Mens retval;  
  retval.ID=0;
  if (head != 0){
      retval = head->getVal();
     Node* oldHead = head;
     head = head->getNext();
     delete oldHead;
  }
  return retval;
}

