# SoftSkin

![vimeo banner](https://polymorph.cool/wp-content/uploads/2018/06/softskin_vimeo_thumb.jpg)

Soft skin library for godot engine.

First prototype demo (openframeworks): 

* [soft skin #003](https://vimeo.com/255103472)
* [soft skin #002](https://vimeo.com/255073574)
* [soft skin #001](https://vimeo.com/254765875)

## remotes

* https://gitlab.com/frankiezafe/SoftSkin - origin
* https://github.com/frankiezafe/SoftSkin - mirror
* https://bitbucket.org/frankiezafe/softskin - hg legacy repo, [OF](http://openframeworks.cc) version

## notes

* main object should inherits [GeometryInstance](http://docs.godotengine.org/en/3.0/classes/class_geometryinstance.html#class-geometryinstance) instead of [ImmediateGeometry](http://docs.godotengine.org/en/3.0/classes/class_immediategeometry.html#class-immediategeometry) with a fixed memory allocation - data managed by SkinDot especially should be pointers on arrays of floats, in a glBufferData for instance (if working) -> saving process and memory
* using glBufferData for better performance https://stackoverflow.com/questions/14155615/opengl-updating-vertex-buffer-with-glbufferdata#14156027
* glBufferData for Opengl ES3: https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glBufferData.xhtml
* maybe switch a part of the process to C, no real need of c++ here, if std vector3 methods like distance and normalized are easy to implement efficiently
