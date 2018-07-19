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
_ligament_strength(1),
_tensor_frequency(0.5),
_tensor_mult_min(0.2),
_tensor_mult_max(1.4),
_feedback_force(0, 0, 0),
_feedback_consumed(0, 0, 0),
_feedback_damping(0.5),
_feedback_softness(0.5),
_feedback(0, 0, 0),
_main_display(true),
_fiber_display(false),
_tensor_display(false),
_ligament_display(false),
_muscle_display(false) {

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

    for (uint32_t i = 0; i < surf_COUNT; ++i) {

        surfaces[i].verticesw = PoolVector<Vector3>::Write();
        surfaces[i].normalsw = PoolVector<Vector3>::Write();
        surfaces[i].uvsw = PoolVector<Vector2>::Write();
        surfaces[i].uvs2w = PoolVector<Vector2>::Write();
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
                array[Mesh::ARRAY_INDEX] = surfaces[i].indices;
                if (surfaces[i].normal_enabled) {
                    array[Mesh::ARRAY_NORMAL] = surfaces[i].normals;
                }
                if (surfaces[i].uv_enabled) {
                    array[Mesh::ARRAY_TEX_UV] = surfaces[i].uvs;
                }
                if (surfaces[i].uv2_enabled) {
                    array[Mesh::ARRAY_TEX_UV2] = surfaces[i].uvs2;
                }
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
    surfaces[surf_MAIN].enabled = _main_display;
    surfaces[surf_MAIN].material = _main_material;

    uint32_t indices = 0;

    for (uint32_t i = 0; i < faces_num; ++i) {

        Vector<float>& fids = raw.faces[i];
        int j = fids.size() - 1;
        switch (j) {
            case 2: // tri, no uv info
                for (uint8_t k = 0; k < 3; ++k) {
                    Vector<float>& vert = raw.verts[int(fids[k])];
                    surfaces[surf_MAIN].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
                    surfaces[surf_MAIN].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
                    surfaces[surf_MAIN].normal_enabled = true;
                    surfaces[surf_MAIN].indices.push_back(indices);
                    ++indices;
                }
                break;
            case 8: // tri with uv
            {
                for (uint8_t k = 0; k < 3; ++k) {
                    Vector<float>& vert = raw.verts[int(fids[k])];
                    surfaces[surf_MAIN].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
                    surfaces[surf_MAIN].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
                    surfaces[surf_MAIN].normal_enabled = true;
                    surfaces[surf_MAIN].indices.push_back(indices);
                    ++indices;
                }
                for (uint8_t k = 3; k < 8; k += 2) {
                    surfaces[surf_MAIN].uvs.push_back(Vector2(fids[k], fids[k + 1]));
                    surfaces[surf_MAIN].uvs2.push_back(Vector2(0, 0));
                }
                surfaces[surf_MAIN].uv_enabled = true;
                surfaces[surf_MAIN].uv2_enabled = true;
            }
                break;
            default:
                break;
        }


    }

    // ********** DEBUG FIBERS **********
    surfaces[surf_FIBER].type = Mesh::PRIMITIVE_LINES;
    surfaces[surf_FIBER].enabled = _fiber_display;
    surfaces[surf_FIBER].material = _fiber_material;
    for (uint32_t i = 0; i < dots_num; ++i) {
        Vector<float>& vert = raw.verts[i];
        surfaces[surf_FIBER].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
        surfaces[surf_FIBER].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
        surfaces[surf_TENSOR].normal_enabled = true;
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
    surfaces[surf_TENSOR].enabled = _tensor_display;
    surfaces[surf_TENSOR].material = _tensor_material;
    for (uint32_t i = 0; i < dots_num; ++i) {
        Vector<float>& vert = raw.verts[i];
        surfaces[surf_TENSOR].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
        surfaces[surf_TENSOR].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
        surfaces[surf_TENSOR].normal_enabled = true;
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
        surfaces[surf_LIGAMENT].enabled = _ligament_display;
        surfaces[surf_LIGAMENT].material = _ligament_material;
        uint32_t ligmax = raw.ligaments.size();
        for (uint32_t i = 0; i < ligmax; ++i) {
            Vector<float>& vert = raw.verts[ raw.ligaments[i] ];
            surfaces[surf_LIGAMENT].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
            surfaces[surf_LIGAMENT].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
            surfaces[surf_LIGAMENT].vertices.push_back(Vector3(vert[1], vert[2], vert[3]));
            surfaces[surf_LIGAMENT].normals.push_back(Vector3(vert[4], vert[5], vert[6]));
            surfaces[surf_LIGAMENT].normal_enabled = true;
            surfaces[surf_LIGAMENT].indices.push_back((i * 2) + 0);
            surfaces[surf_LIGAMENT].indices.push_back((i * 2) + 1);
        }
    }

    // linking all poolvector writters
    for (uint32_t i = 0; i < surf_COUNT; ++i) {
        surfaces[i].verticesw = surfaces[i].vertices.write();
        surfaces[i].indicesw = surfaces[i].indices.write();
        surfaces[i].normalsw = surfaces[i].normals.write();
        surfaces[i].uvsw = surfaces[i].uvs.write();
        surfaces[i].uvs2w = surfaces[i].uvs2.write();
    }

    // ceation of skin objects
    dots = memnew_arr(SkinDot, dots_num);
    forces = memnew_arr(Vector3, dots_num);

    indices = 0;

    for (uint32_t i = 0; i < faces_num; ++i) {

        Vector<float>& fids = raw.faces[i];
        int j = fids.size() - 1;
        switch (j) {
            case 2:
            case 8:
                for (uint8_t k = 0; k < 3; ++k) {
                    int doti = int(fids[k]);
                    if (dots[doti].is_initialised()) {
                        dots[doti].register_vert(&surfaces[surf_MAIN].verticesw[ indices ]);
                        dots[doti].register_normal(&surfaces[surf_MAIN].normalsw[ indices ]);
                    } else {
                        forces[doti][0] = 0;
                        forces[doti][1] = 0;
                        forces[doti][2] = 0;
                        dots[doti].init(
                                &surfaces[surf_MAIN].verticesw[indices],
                                &surfaces[surf_MAIN].normalsw[indices],
                                &forces[doti]
                                );
                        dots[doti].register_vert(&surfaces[surf_FIBER].verticesw[ doti ]);
                        dots[doti].register_vert(&surfaces[surf_TENSOR].verticesw[ doti ]);
                    }
                    ++indices;
                }
                break;
            default:
                break;
        }

    }

    for (uint32_t i = 0; i < dots_num; ++i) {
        if (!dots[i].is_initialised()) {
            std::cout <<
                    "Skin::generate, "
                    "there is something terribly WRONG in your file!" <<
                    std::endl;
            purge();
            return;
        }
        //        forces[i][0] = 0;
        //        forces[i][1] = 0;
        //        forces[i][2] = 0;
        //        dots[i].init(
        //                &surfaces[surf_MAIN].verticesw[i],
        //                &surfaces[surf_MAIN].normalsw[i],
        //                &forces[i]
        //                );
        //        dots[i].register_vert(&surfaces[surf_FIBER].verticesw[ i ]);
        //        dots[i].register_vert(&surfaces[surf_TENSOR].verticesw[ i ]);
        //        forces[1].x = -1 + 2 * (rand() * 1.f / RAND_MAX);
        //        forces[1].y = -1 + 2 * (rand() * 1.f / RAND_MAX);
        //        forces[1].z = -1 + 2 * (rand() * 1.f / RAND_MAX);
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
                    fibers[fibid].init_rest_len() * _tensor_mult_min,
                    fibers[fibid].init_rest_len() * _tensor_mult_max,
                    _tensor_frequency, 0
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
            fibers[fibid].stiffness(_ligament_strength);
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

void Skin::update(const float& delta) {

    // from global space to local space

    if (!dots || !is_visible_in_tree()) {
        return;
    }

    Transform glob_t = get_global_transform();
    Vector3 glob_p = glob_t.get_origin();
    _delta_position = _previous_position - glob_p;
    _previous_position = glob_p;

    Transform loc_t;
    loc_t.basis = glob_t.basis;
    _delta_position = loc_t.basis.inverse().xform(_delta_position);

    if (_update_gravity) {
        Vector3 gqaxis;
        float gqangl;
        get_global_transform().basis.get_rotation_axis_angle(gqaxis, gqangl);
        _gravity = _local_gravity.rotated(gqaxis, -gqangl);
    }

    unbind_root();

    for (uint32_t i = 0; i < dots_num; ++i) {
        if (i == 0) {
            aabb.position = dots[i].update(delta);
            aabb.size = Vector3(0, 0, 0);
        } else {
            aabb.expand_to(dots[i].update(delta));
        }
    }

    for (uint32_t i = 0; i < fibers_num; ++i) {
        _feedback_force += fibers[i].update(delta);
    }

    _feedback_force *= (1 - _feedback_softness);
    _feedback_consumed = _feedback_force * _feedback_damping;
    _feedback_force -= _feedback_consumed;

    bind_root();

    _feedback = loc_t.xform(_feedback_force);

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

            if (!ray.closest_result) {
                ray.closest_result = new SkinRayResult();
            }

            (*ray.closest_result) = srr;
            ray.world_position = to_global(
                    local_from +
                    ray_dir * ray.closest_result->distance_to_origin
                    );
            ray.world_distance = ray.world_position.distance_to(from);
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

void Skin::_notification(int p_what) {

    switch (p_what) {
        case NOTIFICATION_ENTER_TREE:
            SkinNotifier::notify(sno_SKIN_CREATED);
            _previous_position = get_global_transform().origin;
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

//******************** SETTERS ********************//

void Skin::set_ligament_strength(const float& s) {

    _ligament_strength = s;

    if (dots && fibers) {
        for (int i = 0; i < fibers_num; ++i) {
            if (fibers[i].type() == sf_LIGAMENT) {
                fibers[i].stiffness(_ligament_strength);
            }
        }
    }

}

void Skin::set_tensor_frequency(const float& s) {

    _tensor_frequency = s;

    if (dots && fibers) {
        for (int i = 0; i < fibers_num; ++i) {
            if (fibers[i].type() == sf_TENSOR) {
                fibers[i].muscle_frequency(_tensor_frequency);
            }
        }
    }

}

void Skin::set_tensor_mult_min(const float& s) {

    _tensor_mult_min = s;
    apply_tensor_mults();

}

void Skin::set_tensor_mult_max(const float& s) {

    _tensor_mult_max = s;
    apply_tensor_mults();

}

void Skin::apply_tensor_mults() {

    if (dots && fibers) {
        for (int i = 0; i < fibers_num; ++i) {
            if (fibers[i].type() == sf_TENSOR) {
                fibers[i].muscle_min_max(
                        fibers[i].init_rest_len() * _tensor_mult_min,
                        fibers[i].init_rest_len() * _tensor_mult_max
                        );
            }
        }
    }

}

void Skin::set_main_material(const Ref<Material> &material) {

    _main_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_MAIN].material = _main_material;
    }

}

void Skin::set_fiber_material(const Ref<Material> &material) {

    _fiber_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_FIBER].material = _fiber_material;
    }

}

void Skin::set_tensor_material(const Ref<Material> &material) {

    _tensor_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_TENSOR].material = _tensor_material;
    }

}

void Skin::set_ligament_material(const Ref<Material> &material) {

    _ligament_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_LIGAMENT].material = _ligament_material;
    }

}

void Skin::set_muscle_material(const Ref<Material> &material) {

    _muscle_material = material;
    if (!surfaces.empty()) {
        surfaces[surf_MUSCLE].material = _muscle_material;
    }

}

void Skin::set_type(const int& type) {

    purge();

    if (type == 1) {
        parse(_soft_skin_path);
    } else if (type == 2) {
        grid(10, 10);
    }

}

void Skin::set_soft_skin_path(const String& path) {

    _soft_skin_path = path;
    parse(path);

}

void Skin::set_main_display(const bool& display) {

    _main_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_MAIN].enabled = _main_display;
    }

}

void Skin::set_fiber_display(const bool& display) {

    _fiber_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_FIBER].enabled = _fiber_display;
    }

}

void Skin::set_tensor_display(const bool& display) {

    _tensor_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_TENSOR].enabled = _tensor_display;
    }

}

void Skin::set_ligament_display(const bool& display) {

    _ligament_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_LIGAMENT].enabled = _ligament_display;
    }

}

void Skin::set_muscle_display(const bool& display) {

    _muscle_display = display;
    if (!surfaces.empty()) {
        surfaces[surf_MUSCLE].enabled = _ligament_display;
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

void Skin::set_feedback_damping(const float& s) {
    _feedback_damping = s;
}

void Skin::set_feedback_softness(const float& s) {
    _feedback_softness = s;
}

//******************** GETTERS ********************//

AABB Skin::get_aabb() const {
    return aabb;
}

PoolVector<Face3> Skin::get_faces(uint32_t p_usage_flags) const {
    return PoolVector<Face3>();
}

float Skin::ligament_strength() const {
    return _ligament_strength;
}

float Skin::tensor_frequency() const {
    return _tensor_frequency;
}

float Skin::tensor_mult_min() const {
    return _tensor_mult_min;
}

float Skin::tensor_mult_max() const {
    return _tensor_mult_max;
}

Ref<Material> Skin::main_material() const {
    return _main_material;
}

Ref<Material> Skin::fiber_material() const {
    return _fiber_material;
}

Ref<Material> Skin::tensor_material() const {
    return _tensor_material;
}

Ref<Material> Skin::ligament_material() const {
    return _ligament_material;
}

Ref<Material> Skin::muscle_material() const {
    return _muscle_material;
}

int Skin::type() const {
    if (!dots && _soft_skin_path == "") {
        return 0;
    } else if (_soft_skin_path != "") {
        return 1;
    } else {
        return 2;
    }
}

String Skin::soft_skin_path() const {
    return _soft_skin_path;
}

bool Skin::main_display() const {
    return _main_display;
}

bool Skin::fiber_display() const {
    return _fiber_display;
}

bool Skin::tensor_display() const {
    return _tensor_display;
}

bool Skin::ligament_display() const {
    return _ligament_display;
}

bool Skin::muscle_display() const {
    return _muscle_display;
}

float Skin::feedback_damping() const {
    return _feedback_damping;
}

float Skin::feedback_softness() const {
    return _feedback_softness;
}

const Vector3& Skin::feedback() const {
    return _feedback;
}

const Vector3& Skin::delta_position() const {
    return _delta_position;
}

const Vector3& Skin::local_motion_orientation() const {
    return _local_motion_orientation;
}

Vector3 Skin::local_gravity() const {
    return _local_gravity;
}

void Skin::_bind_methods() {

    ClassDB::bind_method(D_METHOD("render_skin", "delta"), &Skin::update);
    ClassDB::bind_method(D_METHOD("parse", "path"), &Skin::parse);
    ClassDB::bind_method(D_METHOD("grid", "divx", "divy"), &Skin::grid);

    ClassDB::bind_method(D_METHOD("set_type", "type"), &Skin::set_type);
    ClassDB::bind_method(D_METHOD("type"), &Skin::type);

    ClassDB::bind_method(D_METHOD("set_soft_skin_path", "path"), &Skin::set_soft_skin_path);
    ClassDB::bind_method(D_METHOD("soft_skin_path"), &Skin::soft_skin_path);

    ClassDB::bind_method(D_METHOD("set_ligament_strength", "s"), &Skin::set_ligament_strength);
    ClassDB::bind_method(D_METHOD("ligament_strength"), &Skin::ligament_strength);

    ClassDB::bind_method(D_METHOD("set_tensor_frequency", "s"), &Skin::set_tensor_frequency);
    ClassDB::bind_method(D_METHOD("tensor_frequency"), &Skin::tensor_frequency);
    ClassDB::bind_method(D_METHOD("set_tensor_mult_min", "s"), &Skin::set_tensor_mult_min);
    ClassDB::bind_method(D_METHOD("tensor_mult_min"), &Skin::tensor_mult_min);
    ClassDB::bind_method(D_METHOD("set_tensor_mult_max", "s"), &Skin::set_tensor_mult_max);
    ClassDB::bind_method(D_METHOD("tensor_mult_max"), &Skin::tensor_mult_max);

    ClassDB::bind_method(D_METHOD("set_main_material", "material"), &Skin::set_main_material);
    ClassDB::bind_method(D_METHOD("main_material"), &Skin::main_material);
    ClassDB::bind_method(D_METHOD("set_fiber_material", "material"), &Skin::set_fiber_material);
    ClassDB::bind_method(D_METHOD("fiber_material"), &Skin::fiber_material);
    ClassDB::bind_method(D_METHOD("set_tensor_material", "material"), &Skin::set_tensor_material);
    ClassDB::bind_method(D_METHOD("tensor_material"), &Skin::tensor_material);
    ClassDB::bind_method(D_METHOD("set_ligament_material", "material"), &Skin::set_ligament_material);
    ClassDB::bind_method(D_METHOD("ligament_material"), &Skin::ligament_material);
    ClassDB::bind_method(D_METHOD("set_muscle_material", "material"), &Skin::set_muscle_material);
    ClassDB::bind_method(D_METHOD("muscle_material"), &Skin::muscle_material);

    ClassDB::bind_method(D_METHOD("set_main_display", "display"), &Skin::set_main_display);
    ClassDB::bind_method(D_METHOD("main_display"), &Skin::main_display);
    ClassDB::bind_method(D_METHOD("set_fiber_display", "display"), &Skin::set_fiber_display);
    ClassDB::bind_method(D_METHOD("fiber_display"), &Skin::fiber_display);
    ClassDB::bind_method(D_METHOD("set_tensor_display", "display"), &Skin::set_tensor_display);
    ClassDB::bind_method(D_METHOD("tensor_display"), &Skin::tensor_display);
    ClassDB::bind_method(D_METHOD("set_ligament_display", "display"), &Skin::set_ligament_display);
    ClassDB::bind_method(D_METHOD("ligament_display"), &Skin::ligament_display);
    ClassDB::bind_method(D_METHOD("set_muscle_display", "display"), &Skin::set_muscle_display);
    ClassDB::bind_method(D_METHOD("muscle_display"), &Skin::muscle_display);

    ClassDB::bind_method(D_METHOD("set_local_gravity", "g"), &Skin::set_local_gravity);
    ClassDB::bind_method(D_METHOD("local_gravity"), &Skin::local_gravity);

    ClassDB::bind_method(D_METHOD("set_feedback_damping", "s"), &Skin::set_feedback_damping);
    ClassDB::bind_method(D_METHOD("feedback_damping"), &Skin::feedback_damping);
    ClassDB::bind_method(D_METHOD("set_feedback_softness", "s"), &Skin::set_feedback_softness);
    ClassDB::bind_method(D_METHOD("feedback_softness"), &Skin::feedback_softness);

    ClassDB::bind_method(D_METHOD("feedback"), &Skin::feedback);
    ClassDB::bind_method(D_METHOD("delta_position"), &Skin::delta_position);
    ClassDB::bind_method(D_METHOD("local_motion_orientation"), &Skin::local_motion_orientation);

    ADD_GROUP("Configuration", "conf_");

    ADD_PROPERTY(PropertyInfo(Variant::INT,
            "conf_type", PROPERTY_HINT_ENUM,
            "undefined,custom,grid"),
            "set_type",
            "type");

    ADD_PROPERTY(PropertyInfo(Variant::STRING,
            "conf_softskin file", PROPERTY_HINT_NONE),
            "set_soft_skin_path",
            "soft_skin_path");

    //    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3,
    //            "conf_gravity", PROPERTY_HINT_NONE),
    //            "set_local_gravity",
    //            "local_gravity");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_damping", PROPERTY_HINT_RANGE,
            "0.0,1.0,0.00001"),
            "set_feedback_damping",
            "feedback_damping");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_softness", PROPERTY_HINT_RANGE,
            "0.0,1.0,0.00001"),
            "set_feedback_softness",
            "feedback_softness");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_ligament_strength", PROPERTY_HINT_RANGE,
            "0.0,2.0,0.00001"),
            "set_ligament_strength", "ligament_strength");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_tensor_frequency", PROPERTY_HINT_RANGE,
            "-10.0,10.0,0.00001"),
            "set_tensor_frequency", "tensor_frequency");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_tensor_mult_min", PROPERTY_HINT_RANGE,
            "0.0,2.0,0.00001"),
            "set_tensor_mult_min", "tensor_mult_min");

    ADD_PROPERTY(PropertyInfo(Variant::REAL,
            "conf_tensor_mult_max", PROPERTY_HINT_RANGE,
            "0.0,4.0,0.00001"),
            "set_tensor_mult_max", "tensor_mult_max");

    ADD_GROUP("Display", "display_");


    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_main", PROPERTY_HINT_NONE),
            "set_main_display",
            "main_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_fiber", PROPERTY_HINT_NONE),
            "set_fiber_display",
            "fiber_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_tensor", PROPERTY_HINT_NONE),
            "set_tensor_display",
            "tensor_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_ligament", PROPERTY_HINT_NONE),
            "set_ligament_display",
            "ligament_display");

    ADD_PROPERTY(PropertyInfo(Variant::BOOL,
            "display_muscle", PROPERTY_HINT_NONE),
            "set_muscle_display",
            "muscle_display");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_main_material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_main_material", "main_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_fiber_material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_fiber_material", "fiber_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_tensor material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_tensor_material", "tensor_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_ligament_material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_ligament_material", "ligament_material");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT,
            "display_muscle material", PROPERTY_HINT_RESOURCE_TYPE,
            "SpatialMaterial,ShaderMaterial"),
            "set_muscle_material", "muscle_material");

}