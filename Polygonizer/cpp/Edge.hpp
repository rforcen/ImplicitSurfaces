//
//  Edge.hpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <assert.h>

class Edge {
    private:
    int index=0;
    int startVertexIndex=0;
    int endVertexIndex=0;
    Edge *connectedEdge0;
    Edge *connectedEdge1;
    
    public:
    Edge() {}
    Edge(int index) {
        static int EDGE_VERTICES[12][2] = {
            {0, 1}, {1, 2}, {3, 2}, {0, 3},
            {4, 5}, {5, 6}, {7, 6}, {4, 7},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };
        
        assert(!(index < 0 || index > 11) && "index must be in the range between 0 and 11");
        
        this->index = index;
        this->startVertexIndex = EDGE_VERTICES[index][0];
        this->endVertexIndex = EDGE_VERTICES[index][1];
        this->connectedEdge0 = nullptr;
        this->connectedEdge1 = nullptr;
    }
    
    int getIndex() {
        return index;
    }
    
    int getStartVertexIndex() {
        return startVertexIndex;
    }
    
    int getEndVertexIndex() {
        return endVertexIndex;
    }
    
    void setConnectedEdge(int index, Edge *edge) {
        assert(!(index != 0 && index != 1)  && "Index out of bounds");
        if (index == 0)  connectedEdge0=edge;
        else             connectedEdge1=edge;
    }
    
    Edge *getConnectedEdge(int index) {
        assert(!(index != 0 && index != 1)  && "Index out of bounds");
        return (index == 0) ? connectedEdge0 : connectedEdge1;
    }
    
    bool operator==(Edge&other) {
        return
        this->index==other.index &&
        this->startVertexIndex==other.startVertexIndex &&
        this->endVertexIndex==other.endVertexIndex &&
        this->connectedEdge0==other.connectedEdge0 &&
        this->connectedEdge1==other.connectedEdge1;
    }
};

#endif /* Edge_hpp */
