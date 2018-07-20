import bpy

export_path = "/home/frankiezafe/forge.godot/softskin_demo/softskin/cube.softskin"
# export_path = "/home/frankiezafe/forge.godot/softskin_demo/softskin/cylinder.softskin"

export = open(export_path, "w")
mesh = bpy.data.meshes["dyncube-mesh"]
# mesh = bpy.data.meshes["dyncylinder-mesh"]
export.write( "++vertices++" + str(len(mesh.vertices)) + "\n" )

uvlayer = mesh.uv_layers[0]

i = 0
for v in mesh.vertices:
    export.write( str(i) + " " + str(v.co[0]) + " " + str(v.co[1]) + " " + str(v.co[2]) + " " + str(v.normal[0]) + " " + str(v.normal[1]) + " " + str(v.normal[2]) + "\n" )
    export.flush()
    i += 1

export.write( "++edges++" + str(len(mesh.edges)) + "\n" )
for e in mesh.edges:
    s = 0
    if e.use_seam == True:
        s += 1
    if e.use_edge_sharp == True:
        s += 2
    export.write( str(e.vertices[0]) + " " + str(e.vertices[1]) + " " + str(s) + "\n" )
    export.flush()
    
export.write( "++faces++" + str(len(mesh.polygons)) + "\n" )
for p in mesh.polygons:
    for i in range(0,len(p.vertices)):
        if i > 0:
            export.write( " " )
        export.write( str(p.vertices[i]) )
    # link uv data for this face
    for i in range(0,p.loop_total):
        export.write( " " )
        uv = uvlayer.data[ p.loop_start + i ].uv
        export.write( str(uv.x) + " " + str(uv.y) )
    export.write( "\n" )
    export.flush()
    
export.close()
print( mesh, len(mesh.vertices) )