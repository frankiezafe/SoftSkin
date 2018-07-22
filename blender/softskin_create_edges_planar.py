import bmesh
import bpy
import mathutils

mesh = bpy.context.active_object.data

bpy.ops.object.mode_set(mode='OBJECT')

sv = list(filter(lambda v: v.select, mesh.vertices))
svl = len(sv)

if svl != 0:
    
    print( "==============", svl )

    # rendering center of the selection
    selected_verts = []
    selected_vert_ids = []
    selection_center = mathutils.Vector((0.0, 0.0, 0.0))
    for i in range(0,len(mesh.vertices)):
        v = mesh.vertices[i]
        if v.select:
            selected_vert_ids.append(i)
            selected_verts.append(v)
            selection_center.x += v.co[0] / svl
            selection_center.y += v.co[1] / svl
            selection_center.z += v.co[2] / svl
            
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='DESELECT')
    bpy.ops.object.mode_set(mode='OBJECT')
    
    bpy.context.scene.cursor_location = (selection_center.to_tuple())
    
    dirs = []
    for v in selected_verts:
        vec = mathutils.Vector((v.co))
        vec -= selection_center
        vec.normalize()
        dirs.append( vec )
        
    # make 2 groups of vertices: FRONT & BACK
    FRONT = []
    BACK = []
    for i in range(0,len(dirs)):
        d = dirs[i]
        if d.dot( dirs[0] ) >= 0:
            FRONT.append( selected_vert_ids[i] )
        else:
            BACK.append( selected_vert_ids[i] )
    
    # creating edges between FRONT and BACK vertices
    for i in FRONT:
        closest = 0
        vec = mathutils.Vector((mesh.vertices[i].co))
        best = -1
        for j in BACK:
            bvec = mathutils.Vector((mesh.vertices[j].co))
            d = ( bvec - vec ).length
            if closest == 0 or closest > d:
                best = j
                closest = d
            pass
        if best != -1:
            mesh.vertices[i].select = True
            mesh.vertices[best].select = True
            bpy.ops.object.mode_set(mode='EDIT')
            bpy.ops.mesh.edge_face_add()
            bpy.ops.mesh.select_all(action='DESELECT')
            bpy.ops.object.mode_set(mode='OBJECT')
            mesh.vertices[i].select = False
            mesh.vertices[best].select = False
        print( "match", i, best )


    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='DESELECT')
    bpy.ops.object.mode_set(mode='OBJECT')
    for i in selected_vert_ids:
        mesh.vertices[i].select = True
    bpy.ops.object.mode_set(mode='EDIT')