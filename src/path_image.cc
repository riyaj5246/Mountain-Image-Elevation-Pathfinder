#include "path_image.hpp"

#include <fstream>
#include <iostream>

PathImage::PathImage(const GrayscaleImage& image,
                     const ElevationDataset& dataset):
    width_(image.Width()),
    height_(image.Height()),
    path_image_(image.GetImage()) {
  GreedyAlgorithm(dataset);
  CalculateBestPath();
}

size_t PathImage::Width() const { return width_; }
size_t PathImage::Height() const { return height_; }
unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }
const std::vector<Path>& PathImage::Paths() const { return paths_; }
const std::vector<std::vector<Color>>& PathImage::GetPathImage() const {
  return path_image_;
}

void PathImage::ToPpm(const std::string& name) const {
  std::ofstream file(name, std::ios::out);
  file << "P3\n" << width_ << ' ' << height_ << '\n' << kMaxColorValue << '\n';
  if (!file.is_open()) {
    throw std::runtime_error("could not open ppm file");
  }
  for (size_t h = 0; h < height_; ++h) {
    for (size_t w = 0; w < width_; ++w) {
      file << path_image_.at(h).at(w).Red() << ' '
           << path_image_.at(h).at(w).Green() << ' '
           << path_image_.at(h).at(w).Blue() << ' ';
    }
    file << '\n';
  }
}

void PathImage::GreedyAlgorithm(const ElevationDataset& dataset) {
  int upper = -1;
  int lower = -1;
  int middle = -1;
  for (size_t p = 0; p < height_; ++p) {
    Path path(width_, p);
    size_t row = p;
    ModifyPath(path, p, 0, 0);
    for (size_t k = 0; k < width_ - 1; ++k) {
      if (row != 0) {
        upper = abs(dataset.DatumAt(row, k) - dataset.DatumAt(row - 1, k + 1));
      }
      if (row != height_ - 1) {
        lower = abs(dataset.DatumAt(row, k) - dataset.DatumAt(row + 1, k + 1));
      }
      middle = abs(dataset.DatumAt(row, k) - dataset.DatumAt(row, k + 1));
      if (row == 0) {
        if (lower < middle) {
          row = ModifyPath(path, row + 1, k + 1, lower);  // lower
        } else {
          row = ModifyPath(path, row, k + 1, middle);  // middle
        }
      } else if (row == height_ - 1) {
        if (upper < middle) {
          row = ModifyPath(path, row - 1, k + 1, upper);  // upper
        } else {
          row = ModifyPath(path, row, k + 1, middle);  // middle
        }
      } else {
        if (upper < middle && upper < lower) {
          row = ModifyPath(path, row - 1, k + 1, upper);  // upper
        } else if (lower <= upper && lower < middle) {
          row = ModifyPath(path, row + 1, k + 1, lower);  // middle
        } else if (middle <= lower && middle <= upper) {
          row = ModifyPath(path, row, k + 1, middle);  // lower
        }
      }
    }
    paths_.push_back(path);
  }
}

size_t PathImage::ModifyPath(Path& path,
                             size_t row,
                             size_t col,
                             int increment) {
  path.SetLoc(col, row);
  path.IncEleChange((unsigned int)increment);
  const int kRed = 252;
  const int kGreen = 25;
  const int kBlue = 63;
  Color c(kRed, kGreen, kBlue);
  path_image_.at(row).at(col) = c;
  return row;
}

void PathImage::CalculateBestPath() {
  unsigned int index = 0;
  unsigned int best_increment_val = paths_.at(0).EleChange();
  for (unsigned int i = 0; i < height_; ++i) {
    if (paths_.at(i).EleChange() < best_increment_val) {
      index = i;
      best_increment_val = paths_.at(i).EleChange();
    }
  }
  const int kRed = 31;
  const int kGreen = 253;
  const int kBlue = 13;
  Color c(kRed, kGreen, kBlue);
  Path best_path = paths_.at(index);
  for (unsigned int j = 0; j < width_; ++j) {
    path_image_.at(best_path.GetPath().at(j)).at(j) = c;
  }
}