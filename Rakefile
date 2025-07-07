require 'bundler/gem_tasks'
require 'rake/testtask'

Rake::TestTask.new(:test) do |t|
  t.libs << 'test'
  t.libs << 'lib'
  t.test_files = FileList['test/**/*_test.rb']
end

require 'rake/extensiontask'

# Vendor directory setup
VENDOR_DIR = File.expand_path('vendor', __dir__)
VENDOR_LIB_DIR = File.join(VENDOR_DIR, 'lib')

jemalloc_shared_lib_ext = RUBY_PLATFORM =~ /darwin/ ? 'dylib' : 'so'
File.expand_path("jemalloc/lib/libjemalloc.#{jemalloc_shared_lib_ext}", __dir__)
jemalloc_vendor_lib = File.join(VENDOR_LIB_DIR, "libjemalloc.#{jemalloc_shared_lib_ext}")

# Build jemalloc and move to vendor
file jemalloc_vendor_lib do
  # Ensure vendor directory exists
  FileUtils.mkdir_p(VENDOR_LIB_DIR)

  # Build jemalloc
  Dir.chdir('jemalloc') do
    sh './autogen.sh'
    sh 'CFLAGS="-fPIC" ./configure --disable-initial-exec-tls'
    sh "make -j #{Etc.nprocessors}"
  end

  # Copy jemalloc files to vendor directory
  jemalloc_lib_dir = File.expand_path('jemalloc/lib', __dir__)

  # Copy the actual library file (libjemalloc.so.2)
  actual_lib = File.join(jemalloc_lib_dir, "libjemalloc.#{jemalloc_shared_lib_ext}.2")
  vendor_actual_lib = File.join(VENDOR_LIB_DIR, "libjemalloc.#{jemalloc_shared_lib_ext}.2")
  FileUtils.cp(actual_lib, vendor_actual_lib)

  # Create symlinks in vendor directory
  Dir.chdir(VENDOR_LIB_DIR) do
    FileUtils.ln_sf("libjemalloc.#{jemalloc_shared_lib_ext}.2", "libjemalloc.#{jemalloc_shared_lib_ext}")
  end

  puts "Moved jemalloc library to vendor: #{jemalloc_vendor_lib}"
end

namespace :jemalloc do
  desc 'Build jemalloc and move to vendor'
  task build: jemalloc_vendor_lib
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

def patch_cmake_jemalloc
  cmake_file = 'odgi/CMakeLists.txt'
  return unless File.exist?(cmake_file)

  jemalloc_backup = "#{cmake_file}.jemalloc_backup"
  return if File.exist?(jemalloc_backup)

  content = File.read(cmake_file)
  
  # Remove or comment out the JEMALLOC_LINK_LIBRARIES setting
  patched = content.gsub(/^(\s*set\s*\(\s*JEMALLOC_LINK_LIBRARIES\s+"jemalloc"\s*\))/, '# \1 # Patched by Rakefile')
  
  if content != patched
    File.write(jemalloc_backup, content)
    File.write(cmake_file, patched)
    puts "Patched jemalloc linking in: #{cmake_file}"
  end
end

def restore_cmake_jemalloc
  cmake_file = 'odgi/CMakeLists.txt'
  jemalloc_backup = "#{cmake_file}.jemalloc_backup"
  return unless File.exist?(jemalloc_backup)

  File.write(cmake_file, File.read(jemalloc_backup))
  File.delete(jemalloc_backup)
  puts "Restored jemalloc linking in: #{cmake_file}"
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

# Define odgi shared library paths
shared_lib_ext = RUBY_PLATFORM =~ /darwin/ ? 'dylib' : 'so'
odgi_source_lib = File.expand_path("odgi/lib/libodgi.#{shared_lib_ext}", __dir__)
odgi_vendor_lib = File.join(VENDOR_LIB_DIR, "libodgi.#{shared_lib_ext}")

# Build odgi using vendor jemalloc and move to vendor
file odgi_vendor_lib => [jemalloc_vendor_lib] do
  patch_cmake_versions
  patch_cmake_jemalloc

  # Use vendor jemalloc
  jemalloc_include_dir = File.expand_path('jemalloc/include', __dir__)
  jemalloc_lib_file = jemalloc_vendor_lib

  Dir.chdir('odgi') do
    # Set rpath to use $ORIGIN for relative path resolution and override JEMALLOC_LINK_LIBRARIES
    rpath_setting = "'$ORIGIN'"
    sh "cmake -H. -Bbuild -DJEMALLOC_LIBRARY=#{jemalloc_lib_file} -DJEMALLOC_INCLUDE_DIR=#{jemalloc_include_dir} -DJEMALLOC_LINK_LIBRARIES=#{jemalloc_lib_file} -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON -DCMAKE_INSTALL_RPATH=#{rpath_setting} -DCMAKE_BUILD_RPATH=#{rpath_setting}"

    # Apply sdsl-lite patch before build
    patch_sdsl_lite

    sh "cmake --build build -- -j #{Etc.nprocessors}"
  end

  # Move to vendor directory
  FileUtils.cp(odgi_source_lib, odgi_vendor_lib)
  puts "Moved odgi library to vendor: #{odgi_vendor_lib}"
end

namespace :odgi do
  task :patch_cmake do
    patch_cmake_versions
  end

  task :restore_cmake do
    restore_cmake_versions
  end

  task :patch_jemalloc do
    patch_cmake_jemalloc
  end

  task :restore_jemalloc do
    restore_cmake_jemalloc
  end

  task :patch_sdsl do
    patch_sdsl_lite
  end

  task :restore_sdsl do
    restore_sdsl_lite
  end

  desc 'Build odgi using vendor jemalloc and move to vendor'
  task build: odgi_vendor_lib
end

# Vendor tasks
namespace :vendor do
  desc 'Clean vendor directory'
  task :clean do
    # Restore patches before cleaning
    restore_cmake_versions
    restore_cmake_jemalloc
    restore_sdsl_lite
    
    if Dir.exist?(VENDOR_DIR)
      FileUtils.rm_rf(VENDOR_DIR)
      puts "Cleaned vendor directory: #{VENDOR_DIR}"
    end
  end
end

# Modify the extension task to use vendor libraries
Rake::ExtensionTask.new('odgi') do |ext|
  ext.lib_dir = 'lib/odgi'

  # Configure to use vendor libraries
  ext.config_options << "--with-odgi-lib=#{VENDOR_LIB_DIR}"
  ext.config_options << "--with-jemalloc-lib=#{VENDOR_LIB_DIR}"
end

# Update compile task to depend on vendor libraries
task compile: [odgi_vendor_lib, jemalloc_vendor_lib]
task build: :compile
task test: :compile
