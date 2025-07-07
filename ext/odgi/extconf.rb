# frozen_string_literal: true

require 'mkmf-rice'

ODGI_DIR = Pathname(__dir__) / '../../odgi'
odgi_library_dir = (ODGI_DIR / 'lib').to_s

# if macOS
if RUBY_PLATFORM =~ /darwin/
  (`brew --prefix libomp`.strip + '/include').tap do |dir|
    find_header 'omp.h', dir
  end
end

# Add all necessary include directories
include_dirs = [
  ODGI_DIR / 'deps/libhandlegraph/src/include',
  ODGI_DIR / 'deps/hopscotch-map/include',
  ODGI_DIR / 'deps/DYNAMIC/include',
  ODGI_DIR / 'deps/sparsepp/sparsepp',
  ODGI_DIR / 'deps/flat_hash_map',
  ODGI_DIR / 'deps/atomicbitvector/include',
  ODGI_DIR / 'src'
]

include_dirs.each do |dir|
  dir_s = dir.to_s
  $INCFLAGS << " -I#{dir_s}"
end

# Verify headers exist
(ODGI_DIR / 'deps/libhandlegraph/src/include').tap do |dir|
  find_header 'handlegraph/types.hpp', dir
  find_header 'handlegraph/iteratee.hpp', dir
  find_header 'handlegraph/util.hpp', dir
  find_header 'handlegraph/handle_graph.hpp', dir
  find_header 'handlegraph/path_handle_graph.hpp', dir
  find_header 'handlegraph/mutable_handle_graph.hpp', dir
  find_header 'handlegraph/mutable_path_handle_graph.hpp', dir
  find_header 'handlegraph/mutable_path_mutable_handle_graph.hpp', dir
  find_header 'handlegraph/deletable_handle_graph.hpp', dir
  find_header 'handlegraph/mutable_path_deletable_handle_graph.hpp', dir
  find_header 'handlegraph/serializable_handle_graph.hpp', dir
end

(ODGI_DIR / 'deps/hopscotch-map/include').tap do |dir|
  find_header 'tsl/hopscotch_map.h', dir
end

(ODGI_DIR / 'deps/DYNAMIC/include').tap do |dir|
  find_header 'dynamic.hpp', dir
end

(ODGI_DIR / 'deps/sparsepp/sparsepp').tap do |dir|
  find_header 'spp.h', dir
end

(ODGI_DIR / 'deps/flat_hash_map').tap do |dir|
  find_header 'bytell_hash_map.hpp', dir
end

(ODGI_DIR / 'deps/atomicbitvector/include').tap do |dir|
  find_header 'atomic_bitvector.hpp', dir
end

# Check for odgi-api.h in the gem's odgi/src directory
find_header 'odgi-api.h', (ODGI_DIR / 'src').to_s

find_library('odgi', nil, odgi_library_dir)

# Link custom jemalloc built with --disable-initial-exec-tls
jemalloc_lib_dir = File.expand_path('../../jemalloc/lib', __dir__)
$LDFLAGS << " -L#{jemalloc_lib_dir} -ljemalloc -Wl,-rpath,#{jemalloc_lib_dir}"

create_makefile('odgi/odgi')
