require_relative 'test_helper'

class OdgiTest < Test::Unit::TestCase
  def test_that_it_has_a_version_number
    refute_nil ::ODGI::VERSION
  end

  def test_odgi_version
    assert_kind_of String, ODGI::FFI.odgi_version
  end

  def test_odgi_load_graph
    graph = ODGI::FFI.odgi_load_graph(
      File.join(File.dirname(__dir__), 'odgi', 'test', 'DRB1-3123_sorted.og')
    )
    assert_kind_of ODGI::Graph, graph
  end

  def setup
    @graph = ODGI::FFI.odgi_load_graph(
      File.join(File.dirname(__dir__), 'odgi', 'test', 'DRB1-3123_sorted.og')
    )
  end

  def test_odgi_has_node
    assert @graph.has_node(1)
    assert @graph.has_node(3000)
    assert_false @graph.has_node(4000)
  end
end
