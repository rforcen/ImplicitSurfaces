//
//  Poligonizer.cpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#include "Polygonizer.hpp"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace p = boost::python;
namespace np = boost::python::numpy;

static void initPython() {
    static bool must_init=true;
    if (must_init) {
     Py_Initialize(); // init boost & numpy boost
     np::initialize();
     must_init=false;
    }
}

template <class T>
void*clone_data(vector<T>v) { //  vector data clone
    return memcpy(malloc(v.size()*sizeof(T)), v.data(), v.size()*sizeof(T));
}

// create a cloned data numpy array
template <class T>
static np::ndarray vector2numpy(vector<T>v) {
    return np::from_data(clone_data(v),     // data -> clone
            np::dtype::get_builtin<T>(),  // dtype -> double
            p::make_tuple(v.size()),    // shape -> size
            p::make_tuple(sizeof(T)), p::object()); // stride
}

template <class T>
static np::ndarray vector2Coords(vector<T>v) {
    return np::from_data(clone_data(v),     // data -> clone
            np::dtype::get_builtin<T>(),  // dtype -> double
            p::make_tuple(v.size()/3, 3),    // shape
            p::make_tuple(sizeof(T)*3, sizeof(T)), p::object()); // stride
}

//
static p::list polygonize(int nDiv, int nFunc) {
    initPython();

    Polygonizer plg(nDiv, funcs[nFunc % funcs.size()]);
    plg.polygonize();

    p::list l;

    l.append(vector2Coords(plg.get_vertices()));
    l.append(vector2Coords(plg.get_normals()));
    l.append(vector2numpy(plg.get_indices()));

    return l;
}

static p::object n_funcs() {
    return p::object(funcs.size());
}

static p::object func_name(int i) {
    return p::object(fNames[i % funcs.size()]);
}

BOOST_PYTHON_MODULE(Polygonizer) {
    def("polygonize", polygonize, (p::arg("nDiv"), p::arg("nFunc")));
    def("func_name", func_name, (p::arg("n_func")));
    def("n_funcs", n_funcs, "number of predef funcs");

}
