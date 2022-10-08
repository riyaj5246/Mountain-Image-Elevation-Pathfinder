#include "elevation_dataset.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

ElevationDataset::ElevationDataset(const std::string& file_name,
                                   size_t width,
                                   size_t height):
    width_(width), height_(height) {
  std::fstream ifs(file_name);
  if (ifs.bad()) {
    throw std::runtime_error("file not opening - unrecoverable");
  }
  std::vector<int> all_nums;
  int num = 0;
  while (ifs.good()) {
    ifs >> num;
    if (ifs.fail()) {
      if (ifs.peek() != EOF) {
        throw std::runtime_error("formatted-read error");
      }
      ifs.clear();
      ifs.ignore(1, '\n');
    } else {
      all_nums.push_back(num);
    }
  }
  if (all_nums.size() != height_ * width_) {
    std::cout << "num size : " << all_nums.size() << " " << height_ * width_
              << std::endl;
    throw std::runtime_error("number of values doesn't match width * height");
  }

  max_ele_ = all_nums.at(0);
  min_ele_ = all_nums.at(0);
  int counter = 0;
  std::vector<int> row_builder;

  for (unsigned int h = 0; h < height_; ++h) {
    row_builder.clear();
    for (unsigned int w = 0; w < width_; ++w) {
      row_builder.push_back(all_nums.at(counter));
      if (all_nums.at(counter) > max_ele_) max_ele_ = all_nums.at(counter);
      if (all_nums.at(counter) < min_ele_) min_ele_ = all_nums.at(counter);
      counter++;
    }
    data_.push_back(row_builder);
  }
}

std::vector<std::vector<int>> ElevationDataset::GetData() { return data_; }
int ElevationDataset::MaxEle() const { return max_ele_; }
int ElevationDataset::MinEle() const { return min_ele_; }
size_t ElevationDataset::Width() const { return width_; }
size_t ElevationDataset::Height() const { return height_; }

int ElevationDataset::DatumAt(size_t row, size_t col) const {
  return data_.at(row).at(col);
}
