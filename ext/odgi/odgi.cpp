#include <rice/rice.hpp>
#include <rice/stl.hpp>
#include "odgi-api.h"

#include "version.hpp"
#include "odgi.hpp"

using namespace Rice;

// Wrapper to convert const shared_ptr to non-const for Rice compatibility
ograph_t odgi_load_graph_wrapper(const char *filename) {
    const ograph_t const_graph = odgi_load_graph(filename);
    return ograph_t(const_graph);
}

extern "C" void Init_odgi()
{
    Module rb_mODGI = define_module("ODGI");

    Class rb_cHandle = define_class_under<handlegraph::handle_t>(rb_mODGI, "Handle");
    Class rb_cEdge = define_class_under<handlegraph::edge_t>(rb_mODGI, "Edge");
    Class rb_cPathHandle = define_class_under<handlegraph::path_handle_t>(rb_mODGI, "PathHandle");
    Class rb_cStepHandle = define_class_under<handlegraph::step_handle_t>(rb_mODGI, "StepHandle");

    Class rb_cGraph = define_class_under<odgi::graph_t>(rb_mODGI, "Graph")
                          .define_constructor(Constructor<odgi::graph_t>())
                          .define_method("has_node", &odgi::graph_t::has_node)
                          .define_method("is_deleted", &odgi::graph_t::is_deleted)
                          .define_method("get_handle", &odgi::graph_t::get_handle)
                          .define_method("get_id", &odgi::graph_t::get_id)
                          .define_method("get_is_reverse", &odgi::graph_t::get_is_reverse)
                          .define_method("flip", &odgi::graph_t::flip)
                          .define_method("get_length", &odgi::graph_t::get_length)
                          .define_method("get_sequence", &odgi::graph_t::get_sequence)
                          .define_method("get_node_count", &odgi::graph_t::get_node_count)
                          .define_method("min_node_id", &odgi::graph_t::min_node_id)
                          .define_method("max_node_id", &odgi::graph_t::max_node_id)
                          .define_method("set_id_increment", &odgi::graph_t::set_id_increment)
                          .define_method("increment_node_ids", &odgi::graph_t::increment_node_ids)
                          .define_method("get_degree", &odgi::graph_t::get_degree)
                          .define_method("forward", &odgi::graph_t::forward)
                          .define_method("edge_handle", &odgi::graph_t::edge_handle)
                          // .define_method("traverse_edge_handle", &odgi::graph_t::traverse_edge_handle)
                          .define_method("id_to_rank", &odgi::graph_t::id_to_rank)
                          .define_method("rank_to_id", &odgi::graph_t::rank_to_id)
                          .define_method("handle_to_rank", &odgi::graph_t::handle_to_rank)
                          .define_method("rank_to_handle", &odgi::graph_t::rank_to_handle)
                          .define_method("has_path", &odgi::graph_t::has_path)
                          .define_method("get_path_handle", &odgi::graph_t::get_path_handle)
                          .define_method("get_path_name", &odgi::graph_t::get_path_name)
                          // .define_method("get_step_count", &odgi::graph_t::get_step_count)
                          .define_method("get_path_count", &odgi::graph_t::get_path_count)
                          .define_method("steps_of_handle", &odgi::graph_t::steps_of_handle)
                          // .define_method("get_step_count", &odgi::graph_t::get_step_count)
                          .define_method("get_handle_of_step", &odgi::graph_t::get_handle_of_step)
                          .define_method("get_path", &odgi::graph_t::get_path)
                          .define_method("path_begin", &odgi::graph_t::path_begin)
                          .define_method("path_end", &odgi::graph_t::path_end)
                          .define_method("path_back", &odgi::graph_t::path_back)
                          .define_method("path_front_end", &odgi::graph_t::path_front_end)
                          .define_method("is_path_front_end", &odgi::graph_t::is_path_front_end)
                          .define_method("is_path_end", &odgi::graph_t::is_path_end)
                          .define_method("get_next_step", &odgi::graph_t::get_next_step)
                          .define_method("get_previous_step", &odgi::graph_t::get_previous_step)
                          .define_method("get_path_handle_of_step", &odgi::graph_t::get_path_handle_of_step)
                          .define_method("get_ordinal_rank_of_step", &odgi::graph_t::get_ordinal_rank_of_step)
                          .define_method("is_empty", &odgi::graph_t::is_empty)
                          // .define_method("for_each_step_in_path", &odgi::graph_t::for_each_step_in_path)
                          .define_method("get_is_circular", &odgi::graph_t::get_is_circular)
                          .define_method("set_circularity", &odgi::graph_t::set_circularity)
                          // .define_method("create_handle", &odgi::graph_t::create_handle)
                          // .define_method("create_handle", &odgi::graph_t::create_handle)
                          .define_method("destroy_handle", &odgi::graph_t::destroy_handle)
                          // .define_method("create_edge", &odgi::graph_t::create_edge)
                          // .define_method("destroy_edge", &odgi::graph_t::destroy_edge);
                          .define_method("clear", &odgi::graph_t::clear)
                          .define_method("clear_paths", &odgi::graph_t::clear_paths)
                          .define_method("swap_handles", &odgi::graph_t::swap_handles)
                          .define_method("apply_ordering", &odgi::graph_t::apply_ordering)
                          .define_method("optimize", &odgi::graph_t::optimize)
                          .define_method("is_optimized", &odgi::graph_t::is_optimized)
                          // .define_method("reassign_node_ids", &odgi::graph_t::reassign_node_ids)
                          .define_method("apply_path_ordering", &odgi::graph_t::apply_path_ordering)
                          .define_method("apply_path_ordering", &odgi::graph_t::apply_path_ordering)
                          .define_method("apply_orientation", &odgi::graph_t::apply_orientation)
                          // .define_method("divide_handle", &odgi::graph_t::divide_handle)
                          .define_method("combine_handles", &odgi::graph_t::combine_handles)
                          .define_method("destroy_path", &odgi::graph_t::destroy_path)
                          .define_method("create_path_handle", &odgi::graph_t::create_path_handle, Return().takeOwnership())
                          .define_method("prepend_step", &odgi::graph_t::prepend_step)
                          .define_method("append_step", &odgi::graph_t::append_step)
                          .define_method("insert_step", &odgi::graph_t::insert_step)
                          .define_method("set_step", &odgi::graph_t::set_step)
                          .define_method("rewrite_segment", &odgi::graph_t::rewrite_segment)
                          .define_method("display", &odgi::graph_t::display)
                          .define_method("to_gfa", &odgi::graph_t::to_gfa)
                          .define_method("get_magic_number", &odgi::graph_t::get_magic_number)
                          // .define_method("serialize_members", &odgi::graph_t::serialize_members)
                          // .define_method("deserialize_members", &odgi::graph_t::deserialize_members)
                          .define_method("set_number_of_threads", &odgi::graph_t::set_number_of_threads)
                          .define_method("get_number_of_threads", &odgi::graph_t::get_number_of_threads)
                          .define_method("copy", &odgi::graph_t::copy)
                          // .define_method("destroy_path_handle_records", &odgi::graph_t::destroy_path_handle_records)
                          .define_method("create_step", &odgi::graph_t::create_step, Return().takeOwnership())
                          .define_method("destroy_step", &odgi::graph_t::destroy_step)
                          .define_method("link_steps", &odgi::graph_t::link_steps)
                          .define_method("decrement_rank", &odgi::graph_t::decrement_rank)
                          .define_method("set_handle_sequence", &odgi::graph_t::set_handle_sequence)
                          .define_method("get_node_rank", &odgi::graph_t::get_node_rank);

    Module rb_mFFI = define_module_under(rb_mODGI, "FFI");
    // Class rb_cFFIGraph = define_class_under<graph_t>(rb_mFFI, "Graph");
    // Class rb_cFFIHandle = define_class_under<handle_t>(rb_mFFI, "Handle");
    // Class rb_cFFIPathHandle = define_class_under<path_handle_t>(rb_mFFI, "PathHandle");
    // Class rb_cFFIStepHandle = define_class_under<step_handle_t>(rb_mFFI, "StepHandle");
    // Class rb_cFFIEdgeHandle = define_class_under<edge_t>(rb_mFFI, "EdgeHandle");

    rb_mFFI
        .define_singleton_function("odgi_version", &odgi_version)
        .define_singleton_function("odgi_long_long_size", &odgi_long_long_size)
        .define_singleton_function("odgi_handle_i_size", &odgi_handle_i_size)
        .define_singleton_function("odgi_step_handle_i_size", &odgi_step_handle_i_size)
        .define_singleton_function("odgi_load_graph", &odgi_load_graph_wrapper)
        .define_singleton_function("odgi_free_graph", &odgi_free_graph)
        .define_singleton_function("odgi_get_node_count", &odgi_get_node_count)
        .define_singleton_function("odgi_max_node_id", &odgi_max_node_id)
        .define_singleton_function("odgi_min_node_id", &odgi_min_node_id)
        .define_singleton_function("odgi_get_path_count", &odgi_get_path_count)
        // .define_singleton_function("odgi_for_each_path_handle", &odgi_for_each_path_handle)
        // .define_singleton_function("odgi_for_each_handle", &odgi_for_each_handle)
        // .define_singleton_function("odgi_follow_edges", &odgi_follow_edges)
        .define_singleton_function("odgi_edge_first_handle", &odgi_edge_first_handle)
        .define_singleton_function("odgi_edge_second_handle", &odgi_edge_second_handle)
        .define_singleton_function("odgi_has_node", &odgi_has_node)
        .define_singleton_function("odgi_get_sequence", &odgi_get_sequence)
        .define_singleton_function("odgi_get_id", &odgi_get_id)
        .define_singleton_function("odgi_get_is_reverse", &odgi_get_is_reverse)
        .define_singleton_function("odgi_get_length", &odgi_get_length)
        .define_singleton_function("odgi_has_path", &odgi_has_path)
        .define_singleton_function("odgi_path_is_empty", &odgi_path_is_empty)
        .define_singleton_function("odgi_get_path_handle", &odgi_get_path_handle)
        .define_singleton_function("odgi_get_step_count", &odgi_get_step_count)
        .define_singleton_function("odgi_get_handle_of_step", &odgi_get_handle_of_step)
        .define_singleton_function("odgi_get_path", &odgi_get_path)
        .define_singleton_function("odgi_path_begin", &odgi_path_begin)
        .define_singleton_function("odgi_path_end", &odgi_path_end)
        .define_singleton_function("odgi_path_back", &odgi_path_back)
        .define_singleton_function("odgi_step_path_id", &odgi_step_path_id)
        .define_singleton_function("odgi_step_is_reverse", &odgi_step_is_reverse)
        .define_singleton_function("odgi_step_prev_id", &odgi_step_prev_id)
        .define_singleton_function("odgi_step_prev_rank", &odgi_step_prev_rank)
        .define_singleton_function("odgi_step_next_id", &odgi_step_next_id)
        .define_singleton_function("odgi_step_next_rank", &odgi_step_next_rank)
        .define_singleton_function("odgi_step_eq", &odgi_step_eq)
        .define_singleton_function("odgi_path_front_end", &odgi_path_front_end)
        .define_singleton_function("odgi_get_next_step", &odgi_get_next_step)
        .define_singleton_function("odgi_get_previous_step", &odgi_get_previous_step)
        .define_singleton_function("odgi_has_edge", &odgi_has_edge)
        .define_singleton_function("odgi_is_path_front_end", &odgi_is_path_front_end)
        .define_singleton_function("odgi_is_path_end", &odgi_is_path_end)
        .define_singleton_function("odgi_has_next_step", &odgi_has_next_step)
        .define_singleton_function("odgi_has_previous_step", &odgi_has_previous_step)
        .define_singleton_function("odgi_get_path_handle_of_step", &odgi_get_path_handle_of_step)
        // .define_singleton_function("odgi_for_each_step_in_path", &odgi_for_each_step_in_path)
        // .define_singleton_function("odgi_for_each_step_on_handle", &odgi_for_each_step_on_handle)
        .define_singleton_function("odgi_get_path_name", &odgi_get_path_name);

    // version.hpp
    Module rb_mVersion = define_module_under(rb_mFFI, "Version");
    rb_mVersion
        .define_singleton_function("get_version", &odgi::Version::get_version)
        .define_singleton_function("get_release", &odgi::Version::get_release)
        .define_singleton_function("get_codename", &odgi::Version::get_codename)
        .define_singleton_function("get_short", &odgi::Version::get_short);
}
