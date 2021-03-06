// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_repetition.h>
#include <volplay/sdf_node_visitor.h>
#include <limits>

namespace volplay {
    
    SDFRepetition::SDFRepetition()
    : _cellSizes(0, 0, 0)
    {}
    
    SDFRepetition::SDFRepetition(const Vector &cellSizes)
    : _cellSizes(cellSizes)
    {}
    
    SDFRepetition::SDFRepetition(const Vector &cellSizes, const SDFNodePtr &n)
    : _cellSizes(cellSizes)
    {
		this->add(n);
	}
    
    const Vector &
    SDFRepetition::cellSizes() const {
        return _cellSizes;
    }
    
    void
    SDFRepetition::setCellSizes(const Vector &cellSizes)
    {
        _cellSizes = cellSizes;
    }

    /** Compatibility isfinite for MSVC10 */
    template<typename T> bool isfinite(T arg)
    {
        return arg == arg && 
               arg != std::numeric_limits<T>::infinity() &&
               arg != -std::numeric_limits<T>::infinity();
    }
    
    SDFResult
    SDFRepetition::fullEval(const Vector &x) const
    {      
        const Vector halfCell = _cellSizes / 2;
		
		const Vector modX(
			isfinite(_cellSizes(0)) ? (fmod(fabs(x(0)) + halfCell(0), _cellSizes(0)) - halfCell(0)) : x(0),
			isfinite(_cellSizes(1)) ? (fmod(fabs(x(1)) + halfCell(1), _cellSizes(1)) - halfCell(1)) : x(1),
			isfinite(_cellSizes(2)) ? (fmod(fabs(x(2)) + halfCell(2), _cellSizes(2)) - halfCell(2)) : x(2)
		);
        
        return SDFUnion::fullEval(modX);
    }

	void
	SDFRepetition::accept(SDFNodeVisitor &nv)
	{
		nv.visit(this);
        acceptChildren(nv);
	}
    
}