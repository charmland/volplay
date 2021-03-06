# volplay

_volplay_ is a library for creating, manipulating and interacting with volumetric data. Focus of this library is clean, reference code. All computations carried out on CPU with minimal focus on parallelism.

## Creating and Manipulating Signed Distance Fields

A signed distance field in _volplay_ is represented by a hierarchy of `volplay::SDFNode`. Leaf nodes represent primitives such as spheres, boxes and planes. Intermediate nodes encapsulate functions on other nodes such as intersection, union, difference, repetition and transformation. 

To start creating a signed distance field use `volplay::make()`.

```cpp
#include <volplay/volplay.h>

namespace vp = volplay;

vp::SDFNodePtr u = vp::make()
    .join()
        .sphere().radius(0.5)
        .transform().translate(vp::Vector(5, 0, 0))
            .sphere().radius(0.2)
        .end()
    .end();
```

Here the `join()` member method creates a new `volplay::SDFUnion`. Its children are an `volplay::SDFSphere` node with radius 0.5 and a transformed `volplay::SDFSphere` with radius 0.2. The complete graph is shown below. The domain specific language chosen comes quite close to a natural language one would prefer to create hierarchies.

![Image](etc/images/samplediagram.png?raw=true)

## Ray-tracing Signed Distance Fields

The namespace `volplay::rendering` provides methods to directly visualize the signed distance field by ray-tracing it. Besides the signed distance field itself, rendering can make use of material properties of individual nodes. The ray-tracing pipeline is implemented in `volplay::rendering::Renderer` and involves the following other entities: 
 - A scene `volplay::SDFNode` to be rendered. Each node can be attributed with materials `volplay::rendering::Material`
 - A camera `volplay::rendering::Camera` defining the viewpoint and lens parameters.
 - A set of lights `volplay::rendering::Lights`.
 - A set of image generators `volplay::rendering::ImageGenerator` encapsulating the types of images to be generated.

The example [example_raytracer.cpp](examples/example_raytracer.cpp) demonstrates rendering of two spheres resting on a plane.
When executed the following set of images is generated. From left to right: Blinn-Phong shaded image with shadows and materials applied, depth image of scene and heat image of scene showing hotspots of raytracing.

![BlinnPhong shaded image](etc/images/raytrace.png?raw=true)

## Surface Reconstruction from Signed Distance Fields

The namespace `volplay::surface` provides methods to explicitly generate a polygonal mesh from an iso-surface in a signed distance field. Currently an implementation of Dual Contouring `volplay::surface::DualContouring` is available. Results can be exported in .OFF format. Here is an example

```cpp
#include <volplay/volplay.h>

namespace vp = volplay;
namespace vps = volplay::surface;

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
dc.setLowerBounds(vp::Vector(-2,-2,-2));
dc.setUpperBounds(vp::Vector(2,2,2));
dc.setResolution(vp::Vector::Constant(vp::S(0.05)));
vps::IndexedSurface surface = dc.compute(scene);

vps::OFFExport off; 
off.exportSurface("surface.off", surface);
```

This produces the surface below. One of the big benefits of Dual Contouring  is that it naturally preserves sharp features.

![Dual Contouring](etc/images/dualcontouring.png?raw=true)


# References

http://aka-san.halcy.de/distance_fields_prefinal.pdf
http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
http://www.ronaldperry.org/SIG2006Course_FriskenPerryDistFields.pdf
http://cg.ibds.kit.edu/downloads/IntModelingSDF.pdf
http://iquilezles.org/www/material/nvscene2008/rwwtt.pdf
http://elib.uni-stuttgart.de/opus/volltexte/2010/5229/pdf/DIP_2970.pdf
http://csokavar.hu/raytrace/imm6392.pdf
http://9bitscience.blogspot.co.at/2013/07/raymarching-distance-fields_14.html
http://www.arcsynthesis.org/gltut/Illumination/Tut12%20Monitors%20and%20Gamma.html
http://www.cambridgeincolour.com/tutorials/gamma-correction.htm
