//
//  EdgeKey.hpp
//  Polygonizer
//
//  Created by asd on 10/08/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef EdgeKey_hpp
#define EdgeKey_hpp

/**
 * EdgeKey class is based on the code found in Jules Bloomenthal's
 * implicit.c:
 * <pre>
 * implicit.c
 *     an implicit surface polygonizer, translated from Mesa
 *     applications should call polygonize()
 *
 * to compile a test program for ASCII output:
 *     cc implicit.c -o implicit -lm
 *
 * to compile a test program for display on an SGI workstation:
 *     cc -DSGIGFX implicit.c -o implicit -lgl_s -lm
 *
 * Authored by Jules Bloomenthal, Xerox PARC.
 * Copyright (c) Xerox Corporation, 1991.  All rights reserved.
 * Permission is granted to reproduce, use and distribute this code for
 * any and all purposes, provided that this notice appears in all copies.
 * </pre>
 */
class EdgeKey {
    const int BIT_SHIFT = 10;
    const int BIT_MASK = (1<<BIT_SHIFT)-1;
    
    int i0, j0, k0;
    int i1, j1, k1;
    
public:
    
    EdgeKey(int *p0, int *p1) {
        init(p0[0], p0[1], p0[2],
                p1[0], p1[1], p1[2]);
    }
    void init(int i0, int j0, int k0,
            int i1, int j1, int k1) {
        if (i0 < i1 || (i0 == i1 && (j0 < j1 || (j0 == j1 && k0 < k1)))) {
            this->i0 = i0; this->j0 = j0; this->k0 = k0;
            this->i1 = i1; this->j1 = j1; this->k1 = k1;
        } else {
            this->i0 = i1; this->j0 = j1; this->k0 = k1;
            this->i1 = i0; this->j1 = j0; this->k1 = k0;
        }
    }
    bool operator==(EdgeKey &other) const {
        return  (i0 == other.i0 && j0 == other.j0 && k0 == other.k0 &&
                 i1 == other.i1 && j1 == other.j1 && k1 == other.k1);
    }
    int hashCode() const {
        return (((((i0&BIT_MASK)<<BIT_SHIFT)|(j0&BIT_MASK))<<BIT_SHIFT)|(k0&BIT_MASK)) +
        (((((i1&BIT_MASK)<<BIT_SHIFT)|(j1&BIT_MASK))<<BIT_SHIFT)|(k1&BIT_MASK));
    }
    
    bool operator<(const EdgeKey& rhs) const {
        return hashCode() < rhs.hashCode();
    }
};
#endif /* EdgeKey_hpp */
