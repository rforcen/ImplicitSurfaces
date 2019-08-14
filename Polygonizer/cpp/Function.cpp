//
//  Function.cpp
//  Polygonizer
//
//  Created by asd on 12/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#include <vector>
#include <string>

using std::vector;
using std::string;

#include "Function.hpp"

vector<string> fNames={"Sphere", "Blob", "NordstarndWeird", "DecoCube", "Cassini", "Orth", "Orth3",
    "Pretzel", "Tooth", "Pilz", "Bretzel" , "BarthDecic", "Clebsch0", "Clebsch",
    "Chubs", "Chair", "Roman", "TangleCube", "Goursat"};

vector<Function*>funcs={
    new Sphere(), new Blob(), new NordstarndWeird(), new DecoCube(), new Cassini(), new Orth(), new Orth3(),
    new Pretzel(), new Tooth(), new Pilz(), new Bretzel() , new BarthDecic(), new Clebsch0(), new Clebsch(),
    new Chubs(), new Chair(), new Roman(), new TangleCube(), new Goursat()
};
