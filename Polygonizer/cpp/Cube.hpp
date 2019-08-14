//
//  Cube.hpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Cube_hpp
#define Cube_hpp

#include "Edge.hpp"
#include "Face.hpp"
#include "FaceFactory.hpp"

#include <vector>

using std::vector;

class Cube {
    private:
    int index;
    vector<Edge*> edges;
    vector<Face> faces;
    
    public:
    Cube(){}
    Cube(int index) {
        assert((index >=0 || index < 255) && "index must be in the range between 0 and 255");
        this->index = index;
        
        for (int edgeIndex = 0; edgeIndex < 12; edgeIndex++) {
            edges.push_back( new Edge(edgeIndex) );
        }

        for (int faceIndex = 0; faceIndex < 6; faceIndex++)
            faces.push_back( FaceFactory::createFace(faceIndex, index, edges) );
    }
    
    int getIndex() {
        return index;
    }
    
    Edge *getEdge(int index) {
        return edges[index];
    }
    
    size_t getEdgeCount() {
        return edges.size();
    }
    
    Face &getFace(int index) {
        return faces[index];
    }
    
    size_t getFaceCount() {
        return faces.size();
    }
    
    vector<int> getEdgeConnectivity(vector<int> connectionSwitches){
        vector<int>connectivity = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        };
        for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
            Face face = faces[faceIndex];
            assert(!(face.isAmbiguous() == false && connectionSwitches[faceIndex] != 0)  && /*faceIndex +
                                                    ((faceIndex == 1) ? "st" :
                                                     ((faceIndex == 2) ? "nd" :
                                                      ((faceIndex == 3) ? "rd" : "th"))) + " face of the cube " +
                                                    index + " (" + indexToString(index) + */") is not ambiguous");

            for (int edgeIndex = 0; edgeIndex < 4; edgeIndex++) {
                Edge *edge = face.getEdge(edgeIndex);
                if (edge->getConnectedEdge(0) != nullptr && face.contains(*(edge->getConnectedEdge(0)))) {
                    connectivity[edge->getIndex()] =
                    edge->getConnectedEdge(connectionSwitches[faceIndex])->getIndex();
                }
            }
        }
        return connectivity;
    }
    
   
};

#endif /* Cube_hpp */
