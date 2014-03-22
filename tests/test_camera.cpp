// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include "float_comparison.hpp"
#include <volplay/camera.h>

namespace vp = volplay;

TEST_CASE("Camera projection transform")
{
    vp::Camera c;
    vp::AffineTransform t = vp::AffineTransform::Identity();
    t.translate(vp::Vector(10, 10, 10));
    
    c.setCameraToImage(vp::Vector2(100, 100), vp::Vector2(320, 240));
    c.setCameraToWorld(t);
    
    vp::Camera::Matrix34 w2c = c.worldToCamera();
    vp::Camera::Matrix34 w2i = c.worldToImage();
    
    vp::Vector x = w2c * vp::Vector(0,0,50).homogeneous();
    
    REQUIRE_CLOSE(x(0), -10);
    REQUIRE_CLOSE(x(1), -10);
    REQUIRE_CLOSE(x(2), 40);
    
    x = w2i * vp::Vector(0,0,50).homogeneous();
    
    REQUIRE_CLOSE(x(0), 100 * -10 + 320 * 40);
    REQUIRE_CLOSE(x(1), 100 * -10 + 240 * 40);
    REQUIRE_CLOSE(x(2), 40);
    
    vp::Vector2 i = x.hnormalized();
    REQUIRE_CLOSE(i(0), (100 * -10 + 320 * 40) / vp::Scalar(40));
    REQUIRE_CLOSE(i(1), (100 * -10 + 240 * 40) / vp::Scalar(40));
}