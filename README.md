# SoftSkin

![vimeo banner](https://polymorph.cool/wp-content/uploads/2018/06/softskin_vimeo_thumb.jpg)

Soft skin library for godot engine.

First prototype demo (openframeworks): 

* [soft skin #003](https://vimeo.com/255103472)
* [soft skin #002](https://vimeo.com/255073574)
* [soft skin #001](https://vimeo.com/254765875)

## demo project

* https://gitlab.com/frankiezafe/SoftSkin-demo

## remotes

* https://gitlab.com/frankiezafe/SoftSkin - origin
* https://github.com/frankiezafe/SoftSkin - mirror
* https://bitbucket.org/frankiezafe/softskin - hg legacy repo, [OF](http://openframeworks.cc) version

## notes

### 11/07/2018

Integration in godot is becoming stable:

* Skin object can be linked to a server to manage them by group
* A custom notification system enables reparenting of servers and skins once placed in the scene tree
* Ray projection are working well

Still to be done:

* importation of uvs (mandatory!)
* collisions with solid parallelipipeds
* creation of fibers group:
* * requires an addon in blender
* * adaptation of the export format
* * specific methods to interacts with groups
* breakeable fibers
* splittable skins, along seams

### 06/06/2018

after serveral hours of engine hell:

* investigate CSG module > Immediate geometries have no shadows and Mesh can not be updated...
* transform Skin object into Geometries manager -> enabling several meshes (debug views, wireframe + tri mesh)

### 03/06/2018

* NOPE - ~~main object should inherits [GeometryInstance](http://docs.godotengine.org/en/3.0/classes/class_geometryinstance.html#class-geometryinstance) instead of [ImmediateGeometry](http://docs.godotengine.org/en/3.0/classes/class_immediategeometry.html#class-immediategeometry) with a fixed memory allocation - data managed by SkinDot especially should be pointers on arrays of floats, in a glBufferData for instance (if working) -> saving process and memory~~
* using glBufferData for better performance https://stackoverflow.com/questions/14155615/opengl-updating-vertex-buffer-with-glbufferdata#14156027
* glBufferData for Opengl ES3: https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glBufferData.xhtml
* maybe switch a part of the process to C, no real need of c++ here, if std vector3 methods like distance and normalized are easy to implement efficiently
