// This header intentionally has no include guards.

template<class T>
class loader_wrapper {
public:
  static void traverse_system_xml(const xml_compiler& xml_compiler,
                                  const T& loader,
                                  const std::string& xml_file_name) {
    ptree_ptr ptree_ptr;
    std::string xml_file_path = xml_compiler.make_file_path(xml_compiler.system_xml_directory_, xml_file_name);
    xml_compiler.read_xml_(ptree_ptr, xml_file_path);
    if (ptree_ptr) {
      loader.traverse(xml_compiler.make_extracted_ptree(*ptree_ptr, xml_file_path));
    }
  }
};
