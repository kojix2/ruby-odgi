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

jemalloc_lib = File.expand_path('jemalloc/lib/libjemalloc.a', __dir__)

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

def patch_cmake_static_jemalloc
  file = 'odgi/CMakeLists.txt'
  return unless File.file?(file)

  orig = "#{file}.orig"
  orig = nil if File.exist?(orig)

  content = File.read(file)

  jemalloc_declaration = <<~CMAKE
    add_library(jemalloc STATIC IMPORTED)
    set_target_properties(jemalloc PROPERTIES IMPORTED_LOCATION "${JEMALLOC_LIBRARY}")
  CMAKE

  return if content.include?('add_library(jemalloc STATIC IMPORTED)')

  content = jemalloc_declaration + "\n" + content
  File.write(orig, File.read(file)) if orig
  File.write(file, content)
  puts "Inserted static jemalloc declaration into: #{file}"
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

namespace :odgi do
  task :patch_cmake do
    patch_cmake_versions
  end

  task :restore_cmake do
    restore_cmake_versions
  end

  task :patch_cmake_static_jemalloc do
    patch_cmake_static_jemalloc
  end

  desc 'Building odgi'
  task build: ['jemalloc:build', :patch_cmake, :patch_cmake_static_jemalloc] do
    jemalloc_root_dir = File.expand_path('jemalloc', __dir__)
    jemalloc_lib_dir = File.join(jemalloc_root_dir, 'lib')
    jemalloc_include_dir = File.join(jemalloc_root_dir, 'include/jemalloc')
    jemalloc_lib_file = File.join(jemalloc_lib_dir, 'libjemalloc.a')

    Dir.chdir('odgi') do
      sh "cmake -H. -Bbuild -DJEMALLOC_LIBRARY=#{jemalloc_lib_file} -DJEMALLOC_INCLUDE_DIR=#{jemalloc_include_dir}"
      sh "cmake --build build -- -j #{Etc.nprocessors}"
    end
  end
end

task compile: 'odgi:build'
task test: :compile
