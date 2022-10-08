#ifndef ELEVATION_DATASET_H
#define ELEVATION_DATASET_H

#include <cstdlib>  // for size_t
#include <iostream>
#include <string>
#include <vector>

class ElevationDataset {
public:
  ElevationDataset(const std::string& file_name, size_t width, size_t height);
  std::vector<std::vector<int>> GetData();
  int MaxEle() const;
  int MinEle() const;
  size_t Width() const;
  size_t Height() const;
  int DatumAt(size_t row, size_t col) const;
  // write behavior declarations here; define in elevation_dataset.cc.
private:
  size_t width_;
  size_t height_;
  std::string file_name_;
  std::vector<std::vector<int>> data_;
  int max_ele_;
  int min_ele_;
};

#endif