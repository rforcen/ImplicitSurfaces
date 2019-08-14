//
//  Poligonizer.hpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Poligonizer_hpp
#define Poligonizer_hpp

#include <vector>
#include <set>
#include <string>
#include <math.h>

using std::vector;
using std::set;
using std::string;

#include "Cube.hpp"
#include "Edge.hpp"
#include "Face.hpp"
#include "LookupTable.hpp"
#include "FaceFactory.hpp"
#include "EdgeKey.hpp"
#include "Function.hpp"



class Polygonizer {
    // Lower left front corner of the bounding box
    double xMin, yMin, zMin;
    // Upper right back corner of the bounding box
    double xMax, yMax, zMax;
    // Number of divisions along each axis of the bounding box
    int xDiv, yDiv, zDiv;
    // Isovalue of the isosurface
    double isovalue;
    // Function defining the isosurface
    Function *function=nullptr;
    double dx, dy, dz;
    double ndx, ndy, ndz;

    LookupTable lookuptable;
    
    vector<double>vertices;
    vector<double>normals;
    vector<int>indices;
    
public:
    Polygonizer() {
        init(vector<double>{-1.0, -1.0, -1.0}, vector<double>{1.0, 1.0, 1.0},
             vector<int>{1, 1, 1}, 0.0, nullptr);
    }
    Polygonizer(int allDivisions, Function *function) {
        init(vector<double>{-1.0, -1.0, -1.0}, vector<double>{1.0, 1.0, 1.0},
             vector<int>{allDivisions, allDivisions, allDivisions}, 0.0, function);
    }
    
    void init(vector<double>min, vector<double>max,
                vector<int>div, double isovalue, Function *function) {
        setBoundingBox(min, max);
        setDivisions(div);
        setIsovalue(isovalue);
        setFunction(function);
    }
    void getBoundingBox(vector<double>&min, vector<double>&max) {
        min[0] = xMin; min[1] = yMin; min[2] = zMin;
        max[0] = xMax; max[1] = yMax; max[2] = zMax;
    }
    
    void setBoundingBox(double xMin, double yMin, double zMin,
                        double xMax, double yMax, double zMax) {
        if (xMin < xMax) {
            this->xMin = xMin;
            this->xMax = xMax;
        } else if (xMin > xMax) {
            this->xMin = xMax;
            this->xMax = xMin;
        } else {
            this->xMin = -1.0;
            this->xMax =  1.0;
        }
        if (yMin < yMax) {
            this->yMin = yMin;
            this->yMax = yMax;
        } else if (yMin > yMax) {
            this->yMin = yMax;
            this->yMax = yMin;
        } else {
            this->yMin = -1.0;
            this->yMax =  1.0;
        }
        if (zMin < zMax) {
            this->zMin = zMin;
            this->zMax = zMax;
        } else if (zMin > zMax) {
            this->zMin = zMax;
            this->zMax = zMin;
        } else {
            this->zMin = -1.0;
            this->zMax =  1.0;
        }
    }
    
    void setBoundingBox(vector<double> min, vector<double> max) {
        setBoundingBox(min[0], min[1], min[2],
                       max[0], max[1], max[2]);
    }
    
    void getDivisions(vector<int>div) {
        div[0] = xDiv;
        div[1] = yDiv;
        div[2] = zDiv;
    }
    
    void setDivisions(int xDiv, int yDiv, int zDiv) {
        this->xDiv = (xDiv > 0) ? xDiv : 1;
        this->yDiv = (yDiv > 0) ? yDiv : 1;
        this->zDiv = (zDiv > 0) ? zDiv : 1;
    }
    void setDivisions(int allDiv) {
        this->xDiv = (allDiv > 0) ? allDiv : 1;
        this->yDiv = (allDiv > 0) ? allDiv : 1;
        this->zDiv = (allDiv > 0) ? allDiv : 1;
    }
    
    void setDivisions(vector<int> div) {
        setDivisions(div[0], div[1], div[2]);
    }
    
    double getIsovalue() {
        return isovalue;
    }
    
    void setIsovalue(double isovalue) {
        this->isovalue = isovalue;
    }
    
    Function *getFunction() {
        return function;
    }
    
    void setFunction(Function *function) {
        this->function = function;
    }
    
    void clearVectors() {
        vertices.clear();
        normals.clear();
        indices.clear();
    }
    
    vector<double>&get_vertices() {    return vertices;   }
    vector<double>&get_normals()  {    return normals;    }
    vector<int>&get_indices()     {    return indices;    }
    
    void polygonize() {
        
        if (function == nullptr) return;
        
        vector<int> connectionSwitches(6);
        int edgeToIndex[12];
        
        vector<vector<double>> upperPlane(yDiv+1, vector<double>(xDiv+1));
        vector<vector<double>> lowerPlane(yDiv+1, vector<double>(xDiv+1));
        double eps = (isovalue == 0.0) ? 1.0E-5 : isovalue * 1.0E-5;
        
        dx = (xMax - xMin) / xDiv;
        dy = (yMax - yMin) / yDiv;
        dz = (zMax - zMin) / zDiv;
        
        ndx = 0.001 * dx;
        ndy = 0.001 * dy;
        ndz = 0.001 * dz;
        
        clearVectors();
        
        sample(lowerPlane, zMin);
        
        for (int k = 0; k < zDiv; k++) {
            double z1 = zMin + k * dz;
            double z2 = zMin + (k + 1) * dz;
            
            sample(upperPlane, z2);
            
            for (int j = 0; j < yDiv; j++) {
                double y1 = yMin + j * dy;
                double y2 = yMin + (j + 1) * dy;
                
                for (int i = 0; i < xDiv; i++) {
                    double x1 = xMin + i * dx;
                    double x2 = xMin + (i + 1) * dx;
                    
                    // Set sampled function values on each corner of the cube
                    double values[8]={
                        lowerPlane[j  ][i  ], lowerPlane[j+1][i  ], lowerPlane[j+1][i+1], lowerPlane[j  ][i+1],
                        upperPlane[j  ][i  ], upperPlane[j+1][i  ], upperPlane[j+1][i+1], upperPlane[j  ][i+1]
                    };

                    
                    // Adjust the function values which are almost same as the isovalue
                    for (int i=0; i<8; i++)
                        if (abs(values[i] - isovalue) < eps) {values[i] += 10.0 * eps;}

                    // Calculate index into the lookup table
                    int cubeIndex = 0;
                    for (int i=0, isw=1; i<8; i++, isw<<=1)
                        if (values[i] > isovalue) cubeIndex +=  isw;

                    if (cubeIndex == 0 || cubeIndex == 255) continue; // Skip the empty cube

                    Cube *cube = lookuptable.getCube(cubeIndex);
                    
                    // Set up corner positions of the cube
                    double positionsD[8][3] =  {{x1,y1,z1},{x1,y2,z1},{x2,y2,z1},{x2,y1,z1},
                        {x1,y1,z2},{x1,y2,z2},{x2,y2,z2},{x2,y1,z2}};
                    
                    int positionsI[8][3] = {{i,j,k},{i,j+1,k},{i+1,j+1,k},{i+1,j,k},
                        {i,j,k+1},{i,j+1,k+1},{i+1,j+1,k+1},{i+1,j,k+1}};
                    
                    
                    // Find the cube edges which have intersection points with
                    // the isosurface
                    set<EdgeKey> indexTable;
                    for (int edgeIndex = 0; edgeIndex < 12; edgeIndex++) {
                        Edge *edge = cube->getEdge(edgeIndex);
                        if (edge->getConnectedEdge(0) != nullptr) {
                            EdgeKey key = EdgeKey(positionsI[edge->getStartVertexIndex()],
                                                  positionsI[edge->getEndVertexIndex()]);
                            auto fnd=indexTable.find(key);
                            if (fnd!=indexTable.end()) {
                                edgeToIndex[edgeIndex] = (int)std::distance(indexTable.begin(), fnd);
                            } else {
                                double t = (isovalue - values[edge->getStartVertexIndex()]) /
                                (values[edge->getEndVertexIndex()] - values[edge->getStartVertexIndex()]);
                                
                                vector<double>vert = lerp(t, positionsD[edge->getStartVertexIndex()],
                                                       positionsD[edge->getEndVertexIndex()]);
                                vertices.insert(vertices.end(), vert.begin(), vert.end());
                                
                                auto nrm = calcNormal(vert);
                                normals.insert(normals.end(), nrm.begin(), nrm.end());
                                
                                indexTable.insert(key);
                                
                                edgeToIndex[edgeIndex] = (int)(vertices.size()/3 - 1); // in coords
                            }
                        }
                    }
                    
                    // Resolve topological ambiguity on cube faces
                    for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
                        Face face = cube->getFace(faceIndex);
                        if (face.isAmbiguous()) {
                            double d0 = values[face.getEdge(0)->getEndVertexIndex()] -
                            values[face.getEdge(0)->getStartVertexIndex()];
                            double d1 = values[face.getEdge(2)->getEndVertexIndex()] -
                            values[face.getEdge(2)->getStartVertexIndex()];
                            double t = (isovalue - values[face.getEdge(1)->getStartVertexIndex()]) /
                            (values[face.getEdge(1)->getEndVertexIndex()] -
                             values[face.getEdge(1)->getStartVertexIndex()]);
                            connectionSwitches[faceIndex] = (t > -d0 / (d1 - d0)) ? 1 : 0;
                        } else {
                            connectionSwitches[faceIndex] = 0;
                        }
                    }
                    
                    // Get the connectivity graph of the cube edges and trace
                    // it to generate triangles
                    auto connectivity = cube->getEdgeConnectivity(connectionSwitches);
                    
                    for (int edgeIndex = 0; edgeIndex < 12;) {
                        if (connectivity[edgeIndex] != -1) {
                            int index0 = edgeIndex;
                            int index1 = connectivity[index0];
                            int index2 = connectivity[index1];
                            
                            indices.push_back(edgeToIndex[index0]);
                            indices.push_back(edgeToIndex[index1]);
                            indices.push_back(edgeToIndex[index2]);
                            
                            connectivity[index0] = -1;
                            connectivity[index1] = -1;

                            if (connectivity[index2] != index0) {
                                connectivity[index0] = index2;
                                continue;
                            }
                            connectivity[index2] = -1;
                        }
                        edgeIndex++;
                    }
                }
            }
            // Swap the lower and upper plane
            std::swap(lowerPlane, upperPlane);
        }
    }
private:
    void sample(vector<vector<double>>&plane, double z) {
        for (int j = 0; j <= yDiv; j++) {
            double y = yMin + j * dy;
            for (int i = 0; i <= xDiv; i++) {
                double x = xMin + i * dx;
                plane[j][i] = function->evaluate(x, y, z);
            }
        }
    }
    
    static vector<double>lerp(double t, double*v0, double*v1) {
        return vector<double> { v0[0] + t * (v1[0] - v0[0]),
                                v0[1] + t * (v1[1] - v0[1]),
                                v0[2] + t * (v1[2] - v0[2]) };
    }

    vector<float> calcNormal(vector<double>&v) {
        double x = v[0];
        double y = v[1];
        double z = v[2];
        
        double f = function->evaluate(x, y, z);
        double nx = -(function->evaluate(x+ndx, y,     z)     - f) / ndx;
        double ny = -(function->evaluate(x,     y+ndy, z)     - f) / ndy;
        double nz = -(function->evaluate(x,     y,     z+ndz) - f) / ndz;
        
        double len = sqrt(nx*nx + ny*ny + nz*nz);
        if (len > 0.0) {
            nx /= len;
            ny /= len;
            nz /= len;
        }
        return vector<float> {(float)nx, (float)ny, (float)nz};
    }
};

#endif /* Poligonizer_hpp */
