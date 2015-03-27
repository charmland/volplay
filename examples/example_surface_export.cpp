// This file is part of volplay, a library for interacting with volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"

#include <volplay/volplay.h>

namespace vp = volplay;
namespace vps = volplay::surface;

TEST_CASE("surface_export")
{
    vp::SDFNodePtr scene = vp::make()
        .difference()
            .join()
                .sphere().radius(1)
                .transform().translate(vp::Vector(0.8f, 0.8f, 0.8f))
                    .sphere().radius(1)
                .end()
            .end()
            .plane().normal(vp::Vector::UnitZ())
        .end();

    vps::DualContouring dc;
    vps::IndexedSurface surface = dc.extractSurface(scene, vp::Vector(-2,-2,-2), vp::Vector(2,2,2), vp::Vector::Constant(0.05f));
    vps::OFFExport off; 
    off.exportSurface("surface.off", surface);    

}