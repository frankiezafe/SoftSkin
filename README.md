# SoftSkin

Soft skin library for godot engine

## origin

* https://gitlab.com/frankiezafe/SoftSkin - main repo
* https://github.com/frankiezafe/SoftSkin - mirror repo

## notes

* main object should inherits [GeometryInstance](http://docs.godotengine.org/en/3.0/classes/class_geometryinstance.html#class-geometryinstance) instead of [ImmediateGeometry](http://docs.godotengine.org/en/3.0/classes/class_immediategeometry.html#class-immediategeometry) with a fixed memory allocation - data managed by SkinDot especially should be pointers on arrays of floats, in a glBufferData for instance (if working) -> saving process and memory
* using glBufferData for better performance https://stackoverflow.com/questions/14155615/opengl-updating-vertex-buffer-with-glbufferdata#14156027
* glBufferData for Opengl ES3: https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glBufferData.xhtml
* maybe switch a part of the process to C, no real need of c++ here, if std vector3 methods like distance and normalized are easy to implement efficiently
