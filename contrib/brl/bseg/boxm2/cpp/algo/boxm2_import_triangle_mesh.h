#ifndef boxm2_import_triangle_mesh_h_
#define boxm2_import_triangle_mesh_h_

#include <imesh/imesh_mesh.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/io/boxm2_cache.h>

bool boxm2_import_triangle_mesh(boxm2_scene_sptr scene, boxm2_cache_sptr cache, imesh_mesh const& mesh);

#endif