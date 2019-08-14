//
//  LookupTable.hpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef LookupTable_hpp
#define LookupTable_hpp

#include "Cube.hpp"

class LookupTable {
public:
    const int max_cubes=256;
    Cube **cubes=nullptr;

    LookupTable() {
        cubes=new Cube*[max_cubes];
        for (int i=0; i<max_cubes; i++)
            cubes[i]=new Cube(i);
    }
    ~LookupTable() {
        for (int i=0; i<max_cubes; i++) delete cubes[i];
        delete[]cubes;
    }

    Cube *getCube(int index) {
        return cubes[index];
    }
    int getCubeCount()      {   return max_cubes;      }
};

#endif /* LookupTable_hpp */
