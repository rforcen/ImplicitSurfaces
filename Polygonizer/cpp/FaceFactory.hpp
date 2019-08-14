//
//  FaceFactory.hpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef FaceFactory_hpp
#define FaceFactory_hpp

#include "Face.hpp"
#include <assert.h>

class FaceFactory {
    
    private:
     static const int CW  = 1;  // Clockwise
     static const int CCW = 0; // Counter clockwise
    
    public:
    
    static bool isAmbiguousBitPattern(int bitPatternOnFace) {
        return (bitPatternOnFace == 5 || bitPatternOnFace == 10) ? true : false;
    }
    
    static bool isBitOn(int bitPatternOnCube, int vertexIndex) {
        return ((bitPatternOnCube & (1 << vertexIndex)) != 0) ? true : false;
    }
    
    static int buildBitPatternOnFace(int bitPatternOnCube, int faceIndex) {
        static const int FACE_VERTICES[6][4] = {
            {0, 1, 2, 3},            {0, 1, 5, 4},            {0, 3, 7, 4},
            {4, 5, 6, 7},            {3, 2, 6, 7},            {1, 2, 6, 5}
        };
        int bitPatternOnFace = 0;
        for (int vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
            if (isBitOn(bitPatternOnCube, FACE_VERTICES[faceIndex][vertexIndex])) {
                bitPatternOnFace |= 1 << vertexIndex;
            }
        }
        return bitPatternOnFace;
    }
    
    static Face createFace(int faceIndex, int bitPatternOnCube, vector<Edge*>edges) {
        assert(!(faceIndex < 0 || faceIndex > 5) && "faceIndex must be in the range between 0 and 5");
        assert( !(bitPatternOnCube < 0 || bitPatternOnCube > 255) && "bitPatternOnCube must be in the range between 0 and 255");
        assert( !(edges.size() != 12) && "length of edges must be 12");
        
        static const int EDGE_CONNECTIVITY_ON_FACE[16][4][4] = {
            {{-1,-1,-1,-1}, {}},            {{-1,-1,-1, 0}, {}},            {{ 1,-1,-1,-1}, {}},
            {{-1,-1,-1, 1}, {}},            {{-1, 2,-1,-1}, {}},            {{-1, 0,-1, 2}, {-1, 2,-1, 0}},
            {{ 2,-1,-1,-1}, {}},            {{-1,-1,-1, 2}, {}},            {{-1,-1, 3,-1}, {}},
            {{-1,-1, 0,-1}, {}},            {{ 1,-1, 3,-1}, { 3,-1, 1,-1}}, {{-1,-1, 1,-1}, {}},
            {{-1, 3,-1,-1}, {}},            {{-1, 0,-1,-1}, {}},            {{ 3,-1,-1,-1}, {}},
            {{-1,-1,-1,-1}, {}}
        };
        
        static const int FACE_EDGES[6][4] = {
            {0,  1,  2,  3},            {0,  9,  4,  8},            {3, 11,  7,  8},
            {4,  5,  6,  7},            {2, 10,  6, 11},            {1, 10,  5,  9}
        };
        
        static const int FACE_ORIENTATION[6] = {CW, CCW, CW, CCW, CW, CCW};

        int bitPatternOnFace = buildBitPatternOnFace(bitPatternOnCube, faceIndex);
        
        Face face =  Face(faceIndex,
                          vector<Edge*>{
                            edges[FACE_EDGES[faceIndex][0]],            edges[FACE_EDGES[faceIndex][1]],
                            edges[FACE_EDGES[faceIndex][2]],            edges[FACE_EDGES[faceIndex][3]]},
                          isAmbiguousBitPattern(bitPatternOnFace));
        
        auto connectivity = EDGE_CONNECTIVITY_ON_FACE[bitPatternOnFace];
        for (int i = 0; i < 2; i++) {
            if (connectivity[i][0] != 0) {
                for (int vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
                    if (connectivity[i][vertexIndex] != -1) {
                        if (FACE_ORIENTATION[faceIndex] == CW) {
                            Edge*edge = face.getEdge(vertexIndex);
                            edge->setConnectedEdge(i, face.getEdge(connectivity[i][vertexIndex]));
                        } else {
                            Edge*edge = face.getEdge(connectivity[i][vertexIndex]);
                            edge->setConnectedEdge(i, face.getEdge(vertexIndex));
                        }
                    }
                }
            }
        }
        return face;
    }
   
};

#endif /* FaceFactory_hpp */
