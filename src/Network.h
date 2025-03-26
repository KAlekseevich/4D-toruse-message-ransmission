#pragma once
#include<vector>
#include <iostream>
#include <unordered_map>
#include <limits> 
#include <string>
#include "checkInput.h"

class Network;

class Node {

    int x, y, z, w;  // координаты узла
    bool busy;  // статус узла (занят или нет)

public:

    friend class Network;
    Node() {};
    Node(int x_, int y_, int z_, int w_);

    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }
    int getW() const { return w; }
    bool isBusy() const { return busy; }
    void setBusy(bool status) { busy = status; }

    // получение соседнего узла по заданной оси 
    Node* getNeighbor(int direction, Network* network);
   
};

class Network {

    Node**** nodes;  // сетка узлов (4D - тор)
    int bandwidth[4];  // пропускная способность каждого соединения (3/3/3/3)
    int DimX, DimY, DimZ, DimW; // количество узлов по осям x,y,z,w

public:

    Network(int DimX_, int DimY_, int DimZ_, int DimW_);

    int getDimX() const { return DimX; }
    int getDimY() const { return DimY; }
    int getDimZ() const { return DimZ; }
    int getDimW() const { return DimW; }

    Node* getNode(int x, int y, int z, int w);

    // поиск кратчайшего пути между отправителем и получателем
    std::vector<Node*> routeSearch(Node* SourceAddress, Node* RecipientAddress);

    // отправка сообщения
    void sendMessage(Network netw, Node* SourceAddress, int MessageLength, Node* RecipientAddress);
  
};




