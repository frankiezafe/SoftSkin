/*
 * 
 *  
 *  _________ ____  .-. _________/ ____ .-. ____ 
 *  __|__  (_)_/(_)(   )____<    \|    (   )  (_)
 *                  `-'                 `-'      
 *  
 *  
 *  art & game engine
 *  
 *  ____________________________________  ?   ____________________________________
 *                                      (._.)
 *  
 *  
 *  This file is part of softskin library
 *  For the latest info, see http://polymorph.cool/
 *  
 *  Copyright (c) 2018 polymorph.cool
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *  
 *  ___________________________________( ^3^)_____________________________________
 *  
 *  ascii font: rotated by MikeChat & myflix
 *  have fun and be cool :)
 */

#include "skin.h"
#include "skindot.h"

Skin::Skin() :
SkinNotifierListener(),
dots_num(0),
fibers_num(0),
dots(0),
forces(0),
ligaments_heads(0),
fibers(0),
_local_gravity(0, 0, 0),
_gravity(0, 0, 0),
_update_gravity(false),
ligament_strength(1),
tensor_frequency(0.5),
tensor_mult_min(0.2),
tensor_mult_max(1.4),
main_display(true),
fiber_display(false),
tensor_display(false),
ligament_display(false),
muscle_display(false) {

    _skl_type = sn_SKIN;
    SkinNotifier::subscribe(this);

}

Skin::~Skin() {

    purge();

    SkinNotifier::unsubscribe(this);
    SkinNotifier::notify(sno_SKIN_DELETED);

}

void Skin::purge() {

    if (dots) memdelete_arr(dots);
    if (forces) memdelete_arr(forces);
    if (ligaments_heads) memdelete_arr(ligaments_heads);
    if (fibers) memdelete_arr(fibers);

    surfaces.clear();

    dots = 0;
    forces = 0;
    ligaments_heads = 0;
    fibers = 0;
    dots_num = 0;
    fibers_num = 0;
    faces_num = 0;

}

void Skin::unbind_root() {

    set_base(RID());
    root_mesh.unref(); //byebye root mesh

}

void Skin::bind_root() {

    root_mesh.instance();

    for (uint32_t i = 0; i < surfaces.size(); ++i) {

        surfaces[i].verticesw = PoolVector<Vector3>::Write();
        surfaces[i].normalsw = PoolVector<Vector3>::Write();
        surfaces[i].uvsw = PoolVector<Vector2>::Write();
        surfaces[i].indicesw = PoolVector<int>::Write();

        if (
                !surfaces[i].enabled ||
                surfaces[i].vertices.size() == 0 ||
                surfaces[i].indices.size() == 0
                ) {
            continue;
        }

        int idx = root_mesh->get_surface_count();
        switch (surfaces[i].type) {
            case Mesh::PRIMITIVE_TRIANGLES:
            {
                Array array;
                array.resize(Mesh::ARRAY_MAX);
                array[Mesh::ARRAY_VERTEX] = surfaces[i].vertices;
                array[Mesh::ARRAY_NORMAL] = surfaces[i].normals;
                //array[Mesh::ARRAY_TEX_UV] = surfaces[i].uvs;
                array[Mesh::ARRAY_INDEX] = surfaces[i].indices;
                root_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, array);
            }
                break;
            case Mesh::PRIMITIVE_LINES:
            {
                Array array;
                array.resize(Mesh::ARRAY_MAX);
                array[Mesh::ARRAY_VERTEX] = surfaces[i].vertices;
                array[Mesh::ARRAY_INDEX] = surfaces[i].indices;
                root_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, array);
            }
                break;
        }
        root_mesh->surface_set_material(idx, surfaces[i].material);

    }

    set_base(root_mesh->get_rid());

}

void Skin::generate(SkinRaw& raw) {

    dots_num = raw.verts.size();
    fibers_num = raw.edges.size();
    uint32_t faces_num = raw.faces.size();

    unbind_root();

    surfaces.resize(surf_COUNT);

    // ********** MAIN MESH **********
    surfaces[surf_MAIN].type = Mesh::PRIMITIVE_TRIANGLES;
    surfaces[surf_MAIN].enabled = main_display;
    surfaces[surf_MAIN].material = main_material;
    for (uint32_t i = 0; i < dots_num; ++i) {
        Vector<float>& vert = raw.verts[i];
        surfaces[surf_MAIN].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
        surfaces[surf_MAIN].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
    }
    for (uint32_t i = 0; i < faces_num; ++i) {
        Vector<float>& fids = raw.faces[i];
        int j = fids.size() - 1;
        switch (j) {
                // no uv info
            case 2: // tri
                while (j >= 0) {
                    surfaces[surf_MAIN].indices.push_back(int(fids[j]));
                    --j;
                }
                break;
                surfaces[surf_MAIN].indices.push_back(int(fids[3]));
                surfaces[surf_MAIN].indices.push_back(int(fids[2]));
                surfaces[surf_MAIN].indices.push_back(int(fids[1]));
                surfaces[surf_MAIN].indices.push_back(int(fids[1]));
                surfaces[surf_MAIN].indices.push_back(int(fids[0]));
                surfaces[surf_MAIN].indices.push_back(int(fids[3]));
            case 8: // tri with uv
            {
                int ids = fids.size() / 3;
                int h = ids - 1;
                while (h >= 0) {
                    surfaces[surf_MAIN].indices.push_back(int(fids[h]));
                    --h;
                }
                while (j > ids) {
                    surfaces[surf_MAIN].uvs.push_back(Vector2(fids[j - 1], fids[j]));
                    j -= 2;
                }
            }
                break;
            case 3: // quad
            case 11: // quad with uv
                surfaces[surf_MAIN].indices.push_back(int(fids[3]));
                surfaces[surf_MAIN].indices.push_back(int(fids[2]));
                surfaces[surf_MAIN].indices.push_back(int(fids[1]));
                surfaces[surf_MAIN].indices.push_back(int(fids[1]));
                surfaces[surf_MAIN].indices.push_back(int(fids[0]));
                surfaces[surf_MAIN].indices.push_back(int(fids[3]));
            default:
                break;
        }

    }

    // ********** DEBUG FIBERS **********
    surfaces[surf_FIBER].type = Mesh::PRIMITIVE_LINES;
    surfaces[surf_FIBER].enabled = fiber_display;
    surfaces[surf_FIBER].material = fiber_material;
    for (uint32_t i = 0; i < dots_num; ++i) {
        Vector<float>& vert = raw.verts[i];
        surfaces[surf_FIBER].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
        surfaces[surf_FIBER].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
    }
    for (uint32_t i = 0; i < fibers_num; ++i) {
        Vector<int>& vs = raw.edges[i];
        if (vs[2] == 0) {
            surfaces[surf_FIBER].indices.push_back(vs[0]);
            surfaces[surf_FIBER].indices.push_back(vs[1]);
        }
    }

    // ********** DEBUG TENSORS **********
    surfaces[surf_TENSOR].type = Mesh::PRIMITIVE_LINES;
    surfaces[surf_TENSOR].enabled = tensor_display;
    surfaces[surf_TENSOR].material = tensor_material;
    for (uint32_t i = 0; i < dots_num; ++i) {
        Vector<float>& vert = raw.verts[i];
        surfaces[surf_TENSOR].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
        surfaces[surf_TENSOR].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
    }
    for (uint32_t i = 0; i < fibers_num; ++i) {
        Vector<int>& vs = raw.edges[i];
        if (vs[2] != 0) {
            surfaces[surf_TENSOR].indices.push_back(vs[0]);
            surfaces[surf_TENSOR].indices.push_back(vs[1]);
        }
    }

    // ********** DEBUG LIGAMENTS **********
    surfaces[surf_LIGAMENT].enabled = false;
    if (!raw.ligaments.empty()) {
        surfaces[surf_LIGAMENT].type = Mesh::PRIMITIVE_LINES;
        surfaces[surf_LIGAMENT].enabled = ligament_display;
        surfaces[surf_LIGAMENT].material = ligament_material;
        uint32_t ligmax = raw.ligaments.size();
        for (uint32_t i = 0; i < ligmax; ++i) {
            Vector<float>& vert = raw.verts[ raw.ligaments[i] ];
            surfaces[surf_LIGAMENT].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
            surfaces[surf_LIGAMENT].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
            surfaces[surf_LIGAMENT].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
            surfaces[surf_LIGAMENT].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
            surfaces[surf_LIGAMENT].indices.push_back((i * 2) + 0);
            surfaces[surf_LIGAMENT].indices.push_back((i * 2) + 1);
        }
    }

    // ********** DEBUG RAY **********
    //    surfaces[surf_RAY].enabled = true;
    //    surfaces[surf_RAY].type = Mesh::PRIMITIVE_LINES;
    //    surfaces[surf_RAY].vertices.push_back(Vector3(0,0,0));
    //    surfaces[surf_RAY].vertices.push_back(Vector3(0,0,0));
    //    surfaces[surf_RAY].indices.push_back(0);
    //    surfaces[surf_RAY].indices.push_back(1);

    // linking all poolvector writters
    for (uint32_t i = 0; i < surf_COUNT; ++i) {
        surfaces[i].verticesw = surfaces[i].vertices.write();
        surfaces[i].normalsw = surfaces[i].normals.write();
        surfaces[i].uvsw = surfaces[i].uvs.write();
        surfaces[i].indicesw = surfaces[i].indices.write();
    }

    // ceation of skin objects
    dots = memnew_arr(SkinDot, dots_num);
    forces = memnew_arr(Vector3, dots_num);

    for (uint32_t i = 0; i < dots_num; ++i) {
        forces[i][0] = 0;
        forces[i][1] = 0;
        forces[i][2] = 0;
        dots[i].init(
                &surfaces[surf_MAIN].verticesw[i],
                &surfaces[surf_MAIN].normalsw[i],
                &forces[i]
                );
        dots[i].register_vert(&surfaces[surf_FIBER].verticesw[ i ]);
        dots[i].register_vert(&surfaces[surf_TENSOR].verticesw[ i ]);
        forces[1].x = -1 + 2 * (rand() * 1.f / RAND_MAX);
        forces[1].y = -1 + 2 * (rand() * 1.f / RAND_MAX);
        forces[1].z = -1 + 2 * (rand() * 1.f / RAND_MAX);
        dots[i].gravity(&_gravity);

    }

    uint32_t fibid = 0;
    uint32_t fibmax = fibers_num + raw.ligaments.size();
    fibers = memnew_arr(SkinFiber, fibmax);

    // generate fibers and tensors
    for (int i = 0; i < fibers_num; ++i, ++fibid) {
        Vector<int>& vs = raw.edges[i];
        fibers[fibid].fiber(&dots[vs[0]], &dots[vs[1]]);
        if (vs[2] != 0) {
            fibers[fibid].musclise(
                    fibers[fibid].init_rest_len() * tensor_mult_min,
                    fibers[fibid].init_rest_len() * tensor_mult_max,
                    tensor_frequency, 0
                    );
        }
    }

    if (!raw.ligaments.empty()) {
        // generate ligaments
        uint32_t ligmax = raw.ligaments.size();
        ligaments_heads = memnew_arr(Vector3*, ligmax);
        for (int i = 0; i < ligmax; ++i, ++fibid) {
            uint32_t vid = raw.ligaments[i];
            dots[vid].register_vert(
                    &surfaces[surf_LIGAMENT].verticesw[ (i * 2) + 1 ]
                    );
            ligaments_heads[i] = &surfaces[surf_LIGAMENT].verticesw[ (i * 2) ];
            fibers[fibid].ligament(ligaments_heads[i], &dots[i]);
            fibers[fibid].stiffness(ligament_strength);
        }
    }
    fibers_num = fibmax;

//    std::cout << "Skin::generate report" << std::endl <<
//            "\traw verts: " << raw.verts.size() << std::endl <<
//            "\traw edges: " << raw.edges.size() << std::endl <<
//            "\traw ligaments: " << raw.ligaments.size() << std::endl <<
//            "\traw faces: " << raw.faces.size() << std::endl <<
//            "\tsurfaces: " << surfaces.size() << std::endl;
//    for (int i = 0; i < surfaces.size(); ++i) {
//        std::cout << "\tsurfaces[" << i << "] vertices: " << surfaces[i].vertices.size() << std::endl;
//        std::cout << "\tsurfaces[" << i << "] normals: " << surfaces[i].normals.size() << std::endl;
//        std::cout << "\tsurfaces[" << i << "] indices: " << surfaces[i].indices.size() << std::endl;
//        std::cout << "\tsurfaces[" << i << "] uvs: " << surfaces[i].uvs.size() << std::endl;
//        std::cout << "\tsurfaces[" << i << "] type: " << surfaces[i].type << std::endl;
//    }

    bind_root();

}

void Skin::grid(uint32_t divx, uint32_t divy) {

    purge();

    SkinRaw raw;

    float xgap = 1.f / divx;
    float ygap = 1.f / divy;

    for (uint32_t y = 0; y <= divy; ++y) {
        for (uint32_t x = 0; x <= divx; ++x) {

            if (
                    (x == 0 && y == 0) ||
                    (x == divx && y == 0)
                    ) {
                raw.ligaments.push_back(x + y * (divx + 1));
            }

            Vector<float> v;
            v.push_back(x + y * (divx + 1));
            v.push_back(1 - xgap * 2 * x);
            v.push_back(1 - ygap * 2 * y);
            v.push_back(0);
            v.push_back(1);
            v.push_back(0);
            v.push_back(0);
            raw.verts.push_back(v);

        }
    }

    for (uint32_t y = 1; y <= divy; ++y) {
        for (uint32_t x = 0; x <= divx; ++x) {

            if (x == 0) {

                Vector<int> v_left;
                v_left.push_back(x + (y - 1) * (divx + 1));
                v_left.push_back(x + y * (divx + 1));
                v_left.push_back(0);
                raw.edges.push_back(v_left);

            } else {

                Vector<int> v_right;
                v_right.push_back(x + (y - 1) * (divx + 1));
                v_right.push_back(x + y * (divx + 1));
                v_right.push_back(0);
                raw.edges.push_back(v_right);

                Vector<int> v_top;
                v_top.push_back((x - 1) + (y - 1) * (divx + 1));
                v_top.push_back(x + (y - 1) * (divx + 1));
                v_top.push_back(0);
                raw.edges.push_back(v_top);

                Vector<int> v_bottom;
                v_bottom.push_back((x - 1) + y * (divx + 1));
                v_bottom.push_back(x + y * (divx + 1));
                v_bottom.push_back(0);
                raw.edges.push_back(v_bottom);

            }

        }
    }

    for (uint32_t y = 1; y <= divy; ++y) {
        for (uint32_t x = 1; x <= divx; ++x) {

            Vector<float> f1;
            f1.push_back((x - 1) + (y - 1) * (divx + 1));
            f1.push_back(x + (y - 1) * (divx + 1));
            f1.push_back(x + y * (divx + 1));
            raw.faces.push_back(f1);

            Vector<float> f2;
            f2.push_back(x + y * (divx + 1));
            f2.push_back((x - 1) + y * (divx + 1));
            f2.push_back((x - 1) + (y - 1) * (divx + 1));
            raw.faces.push_back(f2);

        }
    }

    generate(raw);

}

void Skin::parse(const String& path) {

    purge();

    _File f;

    if (path == "" || !f.file_exists(path)) {
        return;
    }

    SkinRaw decompressed;


    f.open(path, _File::READ);

    String l = f.get_line();
    uint32_t i = 0;
    uint32_t index = 0;

    while (!f.eof_reached()) {

        if (l.begins_with("++vertices++")) {

            decompressed.vpass = true;
            decompressed.epass = false;
            decompressed.fpass = false;
            index = 0;
            int num = l.right(12).to_int();
            decompressed.verts.resize(num);

        } else if (l.begins_with("++edges++")) {

            decompressed.vpass = false;
            decompressed.epass = true;
            decompressed.fpass = false;
            index = 0;
            int num = l.right(9).to_int();
            decompressed.edges.resize(num);

        } else if (l.begins_with("++faces++")) {

            decompressed.vpass = false;
            decompressed.epass = false;
            decompressed.fpass = true;
            index = 0;
            int num = l.right(9).to_int();
            decompressed.faces.resize(num);

        } else if (decompressed.vpass) {

            Vector<float> vs = l.split_floats(" ");
            if (vs.size() >= 7) {
                decompressed.verts[index] = vs;
                decompressed.ligaments.push_back(index);
                ++index;
            } else {
                std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
                return;
            }

        } else if (decompressed.epass) {

            Vector<int> vs = l.split_ints(" ");
            if (vs.size() >= 3) {
                decompressed.edges[index] = vs;
                ++index;
            } else {
                std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
                return;
            }

        } else if (decompressed.fpass) {

            Vector<float> vs = l.split_floats(" ");
            if (
                    vs.size() == 3 || vs.size() == 4 || // tri or quad, no uv info
                    vs.size() == 9 || vs.size() == 12 // tri or quad with uv data
                    ) {
                decompressed.faces[index] = vs;
                ++index;
            } else {
                std::cout << "Skin::parse, failed to decompress line " << i << std::endl;
                return;
            }

        }

        l = f.get_line();
        ++i;
    }

    f.close();

    if (
            decompressed.verts.empty() ||
            decompressed.edges.empty() ||
            decompressed.faces.empty()
            ) {
        std::cout << "Skin::parse, inconsistent data: " <<
                "verts: " << decompressed.verts.size() << ". " <<
                "edges: " << decompressed.edges.size() << ". " <<
                "faces: " << decompressed.faces.size() << ". " <<
                std::endl;
        return;
    }

    decompressed.vpass = false;
    decompressed.epass = false;
    decompressed.fpass = false;

    generate(decompressed);

}

AABB Skin::get_aabb() const {

    return aabb;

}

PoolVector<Face3> Skin::get_faces(uint32_t p_usage_flags) const {

    return PoolVector<Face3>();

}

void Skin::update(const float& delta) {

    if (!dots || !is_visible()) {
        return;
    }

    if (_update_gravity) {
        Vector3 gqaxis;
        float gqangl;
        get_global_transform().basis.get_rotation_axis_angle(gqaxis, gqangl);
        _gravity = _local_gravity.rotated(gqaxis, -gqangl);
    }

    unbind_root();

    for (uint32_t i = 0; i < dots_num; ++i) {
        if ( i == 0 ) {
            aabb.position = dots[i].update(delta);
            aabb.size = Vector3(0,0,0);
        } else {
            aabb.expand_to( dots[i].update(delta) );
        }
    }

    for (uint32_t i = 0; i < fibers_num; ++i) {
        fibers[i].update(delta);
    }

    //    if ( !surfaces.empty() ) {
    //        surfaces[surf_RAY].verticesw = surfaces[surf_RAY].vertices.write();
    //        surfaces[surf_RAY].indicesw = surfaces[surf_RAY].indices.write();
    //        surfaces[surf_RAY].verticesw[0].x = hit_from.x;
    //        surfaces[surf_RAY].verticesw[0].y = hit_from.y;
    //        surfaces[surf_RAY].verticesw[0].z = hit_from.z;
    //        surfaces[surf_RAY].verticesw[1].x = hit_to.x;
    //        surfaces[surf_RAY].verticesw[1].y = hit_to.y;
    //        surfaces[surf_RAY].verticesw[1].z = hit_to.z;
    //    }

    bind_root();

}

void Skin::set_ligament_strength(const float& s) {

    ligament_strength = s;

    if (dots && fibers) {
        for (int i = 0; i < fibers_num; ++i) {
            if (fibers[i].type() == sf_LIGAMENT) {
                fibers[i].stiffness(ligament_strength);
            }
        }
    }

}

void Skin::set_tensor_frequency(const float& s) {

    tensor_frequency = s;

    if (dots && fibers) {
        for (int i = 0; i < fibers_num; ++i) {
            if (fibers[i].type() == sf_TENSOR) {
                fibers[i].muscle_frequency(tensor_frequency);
            }
        }
    }

}

void Skin::set_tensor_mult_min(const float& s) {

    tensor_mult_min = s;
    apply_tensor_mults();

}

void Skin::set_tensor_mult_max(const float& s) {

    tensor_mult_max = s;
    apply_tensor_mults();

}

void Skin::apply_tensor_mults() {

    if (dots && fibers) {
        for (int i = 0; i < fibers_num; ++i) {
            if (fibers[i].type() == sf_TENSOR) {
                fibers[i].muscle_min_max(
                        fibers[i].init_rest_len() * tensor_mult_min,
                        fibers[i].init_rest_len() * tensor_mult_max
                        );
            }
        }
    }

}

float Skin::get_ligament_strength() const {

    return ligament_strength;

}

float Skin::get_tensor_frequency() const {

    return tensor_frequency;

}

float Skin::get_tensor_mult_min() const {

    return tensor_mult_min;

}

float Skin::get_tensor_mult_max() const {

    return tensor_mult_max;

}

void Skin::set_main_material(const Ref<Material> &material) {

    main_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_MAIN].material = main_material;
    }

}

Ref<Material> Skin::get_main_material() const {

    return main_material;

}

Ref<Material> Skin::get_fiber_material() const {

    return fiber_material;

}

Ref<Material> Skin::get_tensor_material() const {

    return tensor_material;

}

Ref<Material> Skin::get_ligament_material() const {

    return ligament_material;

}

Ref<Material> Skin::get_muscle_material() const {

    return muscle_material;

}

void Skin::set_fiber_material(const Ref<Material> &material) {

    fiber_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_FIBER].material = fiber_material;
    }

}

void Skin::set_tensor_material(const Ref<Material> &material) {

    tensor_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_TENSOR].material = tensor_material;
    }

}

void Skin::set_ligament_material(const Ref<Material> &material) {

    ligament_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_LIGAMENT].material = ligament_material;
    }

}

void Skin::set_muscle_material(const Ref<Material> &material) {

    muscle_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_MUSCLE].material = muscle_material;
        //        surfaces[surf_RAY].material = muscle_material;
    }

}

void Skin::set_type(const int& type) {

    purge();

    if (type == 1) {
        parse(soft_skin_path);
    } else if (type == 2) {
        grid(10, 10);
    }

}

int Skin::get_type() const {

    if (!dots && soft_skin_path == "") {

        return 0;

    } else if (soft_skin_path != "") {

        return 1;

    } else {

        return 2;

    }

}

void Skin::set_soft_skin_path(const String& path) {

    soft_skin_path = path;
    parse(path);

}

String Skin::get_soft_skin_path() const {

    return soft_skin_path;

}

void Skin::set_main_display(const bool& display) {

    main_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_MAIN].enabled = main_display;
    }

}

void Skin::set_fiber_display(const bool& display) {

    fiber_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_FIBER].enabled = fiber_display;
    }

}

void Skin::set_tensor_display(const bool& display) {

    tensor_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_TENSOR].enabled = tensor_display;
    }

}

void Skin::set_ligament_display(const bool& display) {

    ligament_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_LIGAMENT].enabled = ligament_display;
    }

}

void Skin::set_muscle_display(const bool& display) {

    muscle_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_MUSCLE].enabled = ligament_display;
    }

}

void Skin::set_local_gravity(const Vector3& g) {

    _local_gravity = g;

    if (_local_gravity.length() != 0) {
        _update_gravity = true;
    } else {
        _update_gravity = false;
        _gravity.x = 0;
        _gravity.y = 0;
        _gravity.z = 0;
    }

}

bool Skin::get_main_display() const {
    return main_display;
}

bool Skin::get_fiber_display() const {
    return fiber_display;
}

bool Skin::get_tensor_display() const {
    return tensor_display;
}

bool Skin::get_ligament_display() const {
    return ligament_display;
}

bool Skin::get_muscle_display() const {
    return muscle_display;
}

Vector3 Skin::get_local_gravity() const {
    return _local_gravity;
}

void Skin::_bind_methods() {

    ClassDB::bind_method(D_METHOD("render_skin", "delta"), &Skin::update);
    ClassDB::bind_method(D_METHOD("parse", "path"), &Skin::parse);
    ClassDB::bind_method(D_METHOD("grid", "divx", "divy"), &Skin::grid);

    ClassDB::bind_method(D_METHOD("set_type", "type"), &Skin::set_type);
    ClassDB::bind_method(D_METHOD("get_type"), &Skin::get_type);

    ClassDB::bind_method(D_METHOD("set_soft_skin_path", "path"), &Skin::set_soft_skin_path);
    ClassDB::bind_method(D_METHOD("get_soft_skin_path"), &Skin::get_soft_skin_path);

    ClassDB::bind_method(D_METHOD("set_ligament_strength", "s"), &Skin::set_ligament_strength);
    ClassDB::bind_method(D_METHOD("get_ligament_strength"), &Skin::get_ligament_strength);

    ClassDB::bind_method(D_METHOD("set_tensor_frequency", "s"), &Skin::set_tensor_frequency);
    ClassDB::bind_method(D_METHOD("get_tensor_frequency"), &Skin::get_tensor_frequency);
    ClassDB::bind_method(D_METHOD("set_tensor_mult_min", "s"), &Skin::set_tensor_mult_min);
    ClassDB::bind_method(D_METHOD("get_tensor_mult_min"), &Skin::get_tensor_mult_min);
    ClassDB::bind_method(D_METHOD("set_tensor_mult_max", "s"), &Skin::set_tensor_mult_max);
    ClassDB::bind_method(D_METHOD("get_tensor_mult_max"), &Skin::get_tensor_mult_max);

    ClassDB::bind_method(D_METHOD("set_main_material", "material"), &Skin::set_main_material);
    ClassDB::bind_method(D_METHOD("get_main_material"), &Skin::get_main_material);
    ClassDB::bind_method(D_METHOD("set_fiber_material", "material"), &Skin::set_fiber_material);
    ClassDB::bind_method(D_METHOD("get_fiber_material"), &Skin::get_fiber_material);
    ClassDB::bind_method(D_METHOD("set_tensor_material", "material"), &Skin::set_tensor_material);
    ClassDB::bind_method(D_METHOD("get_tensor_material"), &Skin::get_tensor_material);
    ClassDB::bind_method(D_METHOD("set_ligament_material", "material"), &Skin::set_ligament_material);
    ClassDB::bind_method(D_METHOD("get_ligament_material"), &Skin::get_ligament_material);
    ClassDB::bind_method(D_METHOD("set_muscle_material", "material"), &Skin::set_muscle_material);
    ClassDB::bind_method(D_METHOD("get_muscle_material"), &Skin::get_muscle_material);

    ClassDB::bind_method(D_METHOD("set_main_display", "display"), &Skin::set_main_display);
    ClassDB::bind_method(D_METHOD("get_main_display"), &Skin::get_main_display);
    ClassDB::bind_method(D_METHOD("set_fiber_display", "display"), &Skin::set_fiber_display);
    ClassDB::bind_method(D_METHOD("get_fiber_display"), &Skin::get_fiber_display);
    ClassDB::bind_method(D_METHOD("set_tensor_display", "display"), &Skin::set_tensor_display);
    ClassDB::bind_method(D_METHOD("get_tensor_display"), &Skin::get_tensor_display);
    ClassDB::bind_method(D_METHOD("set_ligament_display", "display"), &Skin::set_ligament_display);
    ClassDB::bind_method(D_METHOD("get_ligament_display"), &Skin::get_ligament_display);
    ClassDB::bind_method(D_METHOD("set_muscle_display", "display"), &Skin::set_muscle_display);
    ClassDB::bind_method(D_METHOD("get_muscle_display"), &Skin::get_muscle_display);


    ClassDB::bind_method(D_METHOD("set_local_gravity", "g"), &Skin::set_local_gravity);
    ClassDB::bind_method(D_METHOD("get_local_gravity"), &Skin::get_local_gravity);

    ADD_GROUP("Configuration", "conf_");

    ADD_PROPERTY(PropertyInfo(Variant::INT,
            "conf_type", PROPERTY_HINT_ENUM,
            "undefined,custom,grid"),
            "set_type",
            "get_type");

    ADD_PROPERTY(PropertyInfo(Variant::STRING,
            "conf_softskin file", PROPERTY_HINT_NONE),
            "set_soft_skin_path",
            "get_soft_skin_path");

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3,
            "conf_gravity", PROPERTY_HINT_NONE),
            "set_local_gravity",
            "get_local_gravity");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_ligament_strength", PROPERTY_HINT_RANGE,
            "0.0,2.0,0.00001"),
            "set_ligament_strength", "get_ligament_strength");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_tensor_frequency", PROPERTY_HINT_RANGE,
            "-10.0,10.0,0.00001"),
            "set_tensor_frequency", "get_tensor_frequency");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_tensor_mult_min", PROPERTY_HINT_RANGE,
            "0.0,2.0,0.00001"),
            "set_tensor_mult_min", "get_tensor_mult_min");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_tensor_mult_max", PROPERTY_HINT_RANGE,
            "0.0,4.0,0.00001"),
            "set_tensor_mult_max", "get_tensor_mult_max");

    ADD_GROUP("Display", "display_");


    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_main", PROPERTY_HINT_NONE),
            "set_main_display",
            "get_main_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_fiber", PROPERTY_HINT_NONE),
            "set_fiber_display",
            "get_fiber_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_tensor", PROPERTY_HINT_NONE),
            "set_tensor_display",
            "get_tensor_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_ligament", PROPERTY_HINT_NONE),
            "set_ligament_display",
            "get_ligament_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_muscle", PROPERTY_HINT_NONE),
            "set_muscle_display",
            "get_muscle_display");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_main_material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_main_material", "get_main_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_fiber_material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_fiber_material", "get_fiber_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_tensor material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_tensor_material", "get_tensor_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_ligament_material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_ligament_material", "get_ligament_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_muscle material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_muscle_material", "get_muscle_material");

}

void Skin::_notification(int p_what) {

    switch (p_what) {
        case NOTIFICATION_ENTER_TREE:
            SkinNotifier::notify(sno_SKIN_CREATED);
            break;
        case NOTIFICATION_PARENTED:
        case NOTIFICATION_PATH_CHANGED:
        case NOTIFICATION_MOVED_IN_PARENT:
        case NOTIFICATION_UNPARENTED:
            SkinNotifier::notify(sno_SKIN_MOVED);
            break;
        default:
            break;

    }
    //std::cout << p_what << std::endl;

}

void Skin::hit(
        const Vector3& from,
        const Vector3& to,
        const real_t& radius,
        SkinRay& ray
        ) {

    if (!is_visible()) {
        return;
    }

    // transformation of global coords to local
    Vector3 local_from = to_local(from);
    Vector3 local_to = to_local(to);

    Vector3 ray_dir = local_to - local_from;
    ray_dir.normalize();

    hit_from = local_from;
    hit_to = local_to;

    for (uint32_t i = 0; i < dots_num; ++i) {

        SkinRayResult srr;
        dots[i].ray_distance(local_from, ray_dir, srr);

        if (
                srr.dot_to_ray >= 0 &&
                srr.distance_to_ray <= radius
                ) {
            
            srr.skin_ptr = this;
            srr.dot_index = i;
            ray.results[this].push_back(srr);
            
        } else {
            
            continue;
            
        }

        if (
                !ray.closest_result ||
                ray.closest_result->distance_to_origin >
                srr.distance_to_origin
                ) {

            if ( !ray.closest_result ) {
                ray.closest_result = new SkinRayResult();
            }

            (*ray.closest_result) = srr;
            ray.world_position = to_global(
                    local_from +
                    ray_dir * ray.closest_result->distance_to_origin
                    );
            ray.success = true;

        }

    }

}

void Skin::drag(SkinRay& ray) {

    // getting local info
    skinresult_vector& res = ray.results[this];
    skinresult_vector_iterator itr = res.begin();
    skinresult_vector_iterator itre = res.end();

    Vector3 target = to_local(ray.world_position);

    for (; itr != itre; ++itr) {

        SkinRayResult* srr = &(*itr);
        if (srr->dot_index >= dots_num) {
            continue;
        }
        SkinDot& sd = dots[ srr->dot_index ];
        sd.push((target - sd.vert().ref()) * srr->influence);

    }

}

void Skin::ray_world_position(
        const Vector3& from,
        const Vector3& to,
        SkinRay& ray
        ) {

    if (
            ray.closest_result &&
            ray.closest_result->skin_ptr == this
            ) {

        Vector3 local_from = to_local(from);
        Vector3 local_to = to_local(to);

        Vector3 ray_dir = local_to - local_from;
        ray_dir.normalize();

        ray.world_position = to_global(
                local_from +
                ray_dir * ray.closest_result->distance_to_origin
                );

    }

}