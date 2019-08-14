//
//  Face.hpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Face_hpp
#define Face_hpp

#include <vector>
#include <stdexcept>
#include "Edge.hpp"

using std::vector;

class Face {
    int index;
    vector<Edge*>edges;
    bool ambiguous;
    
    public:
    
    Face(int index, vector<Edge*>edges, bool ambiguous) {
        if (index < 0 || index > 5) {
            throw std::invalid_argument("index must be in the range between 0 and 5");
        }
        this->index = index;
        
        this->edges = edges;
        this->ambiguous = ambiguous;
    }
    
    int getIndex() {
        return index;
    }
    
    Edge *getEdge(int index)  {
        return edges[index];
    }
    
    size_t getEdgeCount() {
        return edges.size();
    }
    
    bool isAmbiguous() {
        return ambiguous;
    }
    
    bool contains(Edge edge) {
        return (edge == *edges[0] || edge == *edges[1] ||
                edge == *edges[2] || edge == *edges[3]);
    }
    
};

#endif /* Face_hpp */
