// This header intentionally has no include guards.

class xml_file_path {
public:
  class base_directory {
  public:
    enum type {
      system_xml,
      private_xml,
    };
  };

  xml_file_path(base_directory::type base_directory_type, const std::string& relative_path) :
    base_directory_type_(base_directory_type),
    relative_path_(relative_path)
  {}

  base_directory::type get_base_directory(void) const { return base_directory_type_; }
  const std::string& get_relative_path(void) const { return relative_path_; }

private:
  base_directory::type base_directory_type_;
  const std::string relative_path_;
};

typedef std::tr1::shared_ptr<xml_file_path> xml_file_path_ptr;
