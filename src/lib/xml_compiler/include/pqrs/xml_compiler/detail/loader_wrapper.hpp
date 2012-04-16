// This header intentionally has no include guards.

template<class T>
class loader_wrapper {
public:
  static void traverse_system_xml(const xml_compiler& xml_compiler,
                                  const T& loader,
                                  const char* xml_file_name) {
    ptree_ptr ptree_ptr;
    xml_compiler.read_xml_(ptree_ptr,
                           xml_file_path(xml_file_path::base_directory::system_xml, xml_file_name));
    if (ptree_ptr) {
      loader.traverse(*ptree_ptr);
    }
  }

  static bool extract_include(T& loader,
                              const xml_compiler& xml_compiler,
                              const boost::property_tree::ptree::value_type& it) {
    ptree_ptr pt_ptr;
    xml_compiler.extract_include_(pt_ptr, it);
    if (pt_ptr) {
      if (! pt_ptr->empty()) {
        loader.traverse(*pt_ptr);
      }
      return true;
    }
    return false;
  }
};
