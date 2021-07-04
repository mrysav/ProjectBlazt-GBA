#include <csv.h>
#include <filesystem>
#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <vector>

using namespace std;

void field_read(void *data, size_t len, void *ptr) {
  if (len < 1) {
    return;
  }

  auto tiles = (vector<vector<int>> *)ptr;
  auto &head = tiles->back();

  auto dat = atoi((char *)data);

  head.push_back(dat);
}

void record_end(int chr, void *ptr) {
  auto tiles = (vector<vector<int>> *)ptr;
  tiles->push_back(vector<int>());
}

vector<vector<int>> parse_tiledata(string csv_data) {
  auto buf = csv_data.c_str();
  auto len = csv_data.length();

  vector<vector<int>> tiledata;
  tiledata.push_back(vector<int>());

  struct csv_parser p;

  if (csv_init(&p, 0) != 0) {
    cerr << "Could not initialize csv parser!\n";
    exit(1);
  }

  auto bytes_read = csv_parse(&p, buf, len, field_read, record_end, &tiledata);
  if (bytes_read != len) {
    cout << "error\n";
    cout << "Error parsing CSV data: " << csv_strerror(csv_error(&p)) << "\n";
    exit(1);
  }
  // Last row didn't need to be added
  tiledata.pop_back();

  csv_fini(&p, field_read, record_end, &tiledata);
  csv_free(&p);

  return tiledata;
}

int main(int argc, char *argv[]) {

  auto infile = string(argv[1]);

  if (infile.find(".tmx") != infile.length() - 4) {
    cerr << infile << " is not a .tmx file.\n";
    return 1;
  }

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(infile.c_str());
  if (!result) {
    cerr << "Could not find or parse document: " << infile << "\n";
    return 1;
  }

  cout << "Loaded file\n";

  auto basename = filesystem::path(infile).stem().generic_string();

  auto map = doc.select_node("/map").node();
  auto height = atoi(map.attribute("height").value());
  auto width = atoi(map.attribute("width").value());

  cout << "Map " << basename << " height: " << height << ", width: " << width
       << "\n";

  auto layers = map.select_nodes("//layer");

  for (auto l : layers) {
    auto layer = l.node();
    auto tag = layer.attribute("name").value();
    auto data = layer.select_node("data").node().text().as_string();

    auto tiledata = parse_tiledata(data);

    cout << "height: " << tiledata.size() << ", width: " << tiledata[0].size()
         << "\n";
  }
}
