#include <bits/stdc++.h>
#include <cctype>
#include <csv.h>
#include <filesystem>
#include <fstream>
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

void write_h_file(string filename, string map_name, int map_width,
                  int map_height, int layerCount) {
  string outfile_name = filename + ".h";
  cout << "Writing header " << outfile_name << "...\n";

  auto map_size = map_height * map_width;
  auto map_size_tiles = map_size * 4;
  // There are 4 tiles per logical tile
  // since blocks are 2x2 tiles

  string upper_map_name(map_name);
  transform(upper_map_name.begin(), upper_map_name.end(),
            upper_map_name.begin(), [](unsigned char c) { return toupper(c); });

  ofstream outfile;
  outfile.open(outfile_name, ios::out);
  if (!outfile.is_open()) {
    cerr << "File " << outfile_name << " could not be opened.\n";
    exit(1);
  }

  outfile << "#pragma once\n"
          << "\n"
          << "#include <stdint.h>\n"
          << "\n"
          << "#define " << upper_map_name << "_LEN " << map_size_tiles << "\n"
          << "#define " << upper_map_name << "_HEIGHT " << map_height << "\n"
          << "#define " << upper_map_name << "_WIDTH " << map_width << "\n"
          << "\n";

  for (int i = 0; i < layerCount; i++) {
    outfile << "extern const uint16_t " << upper_map_name << "_BG" << i << "["
            << map_size << "];\n";
  }

  outfile.close();
}

void write_c_file(string filename, string map_name, int map_width,
                  int map_height, vector<vector<vector<int>>> &tiledata) {
  string outfile_name = filename + ".c";
  cout << "Writing header " << outfile_name << "...\n";

  auto map_size = map_height * map_width;

  string upper_map_name(map_name);
  transform(upper_map_name.begin(), upper_map_name.end(),
            upper_map_name.begin(), [](unsigned char c) { return toupper(c); });

  ofstream outfile;
  outfile.open(outfile_name, ios::out);
  if (!outfile.is_open()) {
    cerr << "File " << outfile_name << " could not be opened.\n";
    exit(1);
  }

  outfile << "#include <stdint.h>\n"
          << "\n"
          << "#include \"" << filename << ".h\"\n"
          << "\n";

  for (size_t l = 0; l < tiledata.size(); l++) {
    auto &layer = tiledata[l];
    outfile << "const uint16_t " << upper_map_name << "_BG" << l << "["
            << map_size << "] = {\n  ";

    for (auto row : layer) {
      for (auto col : row) {
        outfile << "0x" << hex << (max(col - 1, 0)) << ", ";
      }
      outfile << "\n  ";
    }

    outfile << "};\n"
            << "\n";
  }

  outfile.close();
}

int main(int argc, char *argv[]) {

  auto infile = string(argv[1]);
  auto outname = string(argv[2]);

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

  cout << "Loaded file " << infile << ".\n";

  auto basename = filesystem::path(infile).stem().generic_string();

  auto map = doc.select_node("/map").node();
  auto height = atoi(map.attribute("height").value());
  auto width = atoi(map.attribute("width").value());

  cout << "Map " << basename << " height: " << height << ", width: " << width
       << "\n";

  auto layers = map.select_nodes("//layer");

  vector<vector<vector<int>>> tile_layers;

  for (auto l : layers) {
    auto layer = l.node();
    auto data = layer.select_node("data").node().text().as_string();

    auto tiledata = parse_tiledata(data);
    tile_layers.push_back(tiledata);
  }

  write_h_file(outname, basename, width, height, tile_layers.size());
  write_c_file(outname, basename, width, height, tile_layers);
}
