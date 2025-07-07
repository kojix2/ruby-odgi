require 'bundler/gem_tasks'
require 'rake/testtask'

Rake::TestTask.new(:test) do |t|
  t.libs << 'test'
  t.libs << 'lib'
  t.test_files = FileList['test/**/*_test.rb']
end

require 'rake/extensiontask'

task build: :compile

Rake::ExtensionTask.new('odgi') do |ext|
  ext.lib_dir = 'lib/odgi'
end

jemalloc_shared_lib_ext = RUBY_PLATFORM =~ /darwin/ ? 'dylib' : 'so'
jemalloc_lib = File.expand_path("jemalloc/lib/libjemalloc.#{jemalloc_shared_lib_ext}", __dir__)

file jemalloc_lib do
  Dir.chdir('jemalloc') do
    sh './autogen.sh'
    sh 'CFLAGS="-fPIC" ./configure --disable-initial-exec-tls'
    sh "make -j #{Etc.nprocessors}"
  end
end

namespace :jemalloc do
  desc 'Build jemalloc if needed'
  task build: jemalloc_lib
end

def find_cmake_files
  require 'find'
  files = []
  Find.find('odgi') do |path|
    files << path if path =~ /CMakeLists\.txt$/ || path =~ /\.cmake$/
  end
  files
end

def patch_cmake_versions
  find_cmake_files.each do |file|
    next unless File.file?(file)

    orig = "#{file}.orig"
    next if File.exist?(orig)

    content = File.read(file)
    patched = content.gsub(/cmake_minimum_required\s*\(\s*VERSION\s+[^)]+\)/i, 'cmake_minimum_required(VERSION 3.5)')
    next unless content != patched

    File.write(orig, content)
    File.write(file, patched)
    puts "Patched: #{file}"
  end
end

def restore_cmake_versions
  find_cmake_files.each do |file|
    orig = "#{file}.orig"
    next unless File.exist?(orig)

    File.write(file, File.read(orig))
    File.delete(orig)
    puts "Restored: #{file}"
  end
end

def patch_sdsl_lite
  # Patch louds_tree.hpp after sdsl-lite build
  louds_tree_files = Dir.glob('odgi/build/sdsl-lite-prefix/src/sdsl-lite*/include/sdsl/louds_tree.hpp')

  louds_tree_files.each do |file|
    next unless File.exist?(file)

    orig = "#{file}.orig"
    next if File.exist?(orig) # Already patched

    content = File.read(file)
    # Fix the specific lines in swap function
    patched = content.gsub(/util::swap_support\(m_bv_select1, tree\.m_select1,/, 'util::swap_support(m_bv_select1, tree.m_bv_select1,')
                     .gsub(/util::swap_support\(m_bv_select0, tree\.m_select0,/, 'util::swap_support(m_bv_select0, tree.m_bv_select0,')

    next unless content != patched

    File.write(orig, content) # Backup original
    File.write(file, patched)
    puts "Patched sdsl-lite: #{file}"
  end
end

def restore_sdsl_lite
  louds_tree_files = Dir.glob('odgi/build/sdsl-lite-prefix/src/sdsl-lite*/include/sdsl/louds_tree.hpp')

  louds_tree_files.each do |file|
    orig = "#{file}.orig"
    next unless File.exist?(orig)

    File.write(file, File.read(orig))
    File.delete(orig)
    puts "Restored sdsl-lite: #{file}"
  end
end

# Define odgi shared library as a file task (platform-specific extension)
shared_lib_ext = RUBY_PLATFORM =~ /darwin/ ? 'dylib' : 'so'
odgi_shared_lib = File.expand_path("odgi/lib/libodgi.#{shared_lib_ext}", __dir__)

file odgi_shared_lib => [jemalloc_lib] do
  patch_cmake_versions

  jemalloc_root_dir = File.expand_path('jemalloc', __dir__)
  jemalloc_lib_dir = File.join(jemalloc_root_dir, 'lib')
  jemalloc_include_dir = File.join(jemalloc_root_dir, 'include')

  linker_flags = "-L#{jemalloc_lib_dir}"

  Dir.chdir('odgi') do
    sh "cmake -H. -Bbuild -DCMAKE_EXE_LINKER_FLAGS='#{linker_flags}' -DCMAKE_SHARED_LINKER_FLAGS='#{linker_flags}' -DCMAKE_CXX_FLAGS='-I#{jemalloc_include_dir}'"

    # Apply sdsl-lite patch before build
    patch_sdsl_lite

    sh "cmake --build build -- -j #{Etc.nprocessors}"
  end
end

namespace :odgi do
  task :patch_cmake do
    patch_cmake_versions
  end

  task :restore_cmake do
    restore_cmake_versions
  end

  task :patch_sdsl do
    patch_sdsl_lite
  end

  task :restore_sdsl do
    restore_sdsl_lite
  end

  desc 'Building odgi'
  task build: odgi_shared_lib
end

task compile: 'odgi:build'
task test: :compile
