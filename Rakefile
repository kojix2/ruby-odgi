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

namespace :jemalloc do
  desc 'Building jemalloc'
  task :build do
    Dir.chdir('jemalloc') do
      sh './autogen.sh'
      sh './configure --disable-initial-exec-tls'
      sh "make -j #{Etc.nprocessors}"
    end
  end
end

namespace :odgi do
  desc 'Building odgi'
  task :build do
    jemalloc_root_dir = File.expand_path('jemalloc', __dir__)
    jemalloc_lib_dir = File.join(jemalloc_root_dir, 'lib')
    jemalloc_include_dir = File.join(jemalloc_root_dir, 'include')
    jemalloc_lib_file = File.join(jemalloc_lib_dir, 'libjemalloc.a')
    Dir.chdir('odgi') do
      sh "cmake -H. -Bbuild -DJEMALLOC_LIBRARY=#{jemalloc_lib_file} -DJEMALLOC_INCLUDE_DIR=#{jemalloc_include_dir} -DCMAKE_LIBRARY_PATH=#{jemalloc_lib_dir}"
      sh "cmake --build build -- -j #{Etc.nprocessors}"
    end
  end
end
