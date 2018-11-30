/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "field_tag.h"

namespace kt {
//field_tag_t** field_tag_t::field_cache = new field_tag_t*[CACHE_SIZE];
std::vector<kt::field_tag_t*> kt::field_tag_t::field_vec_cache(CACHE_SIZE,NULL);

}