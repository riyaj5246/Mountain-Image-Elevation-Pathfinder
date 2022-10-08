#include "grayscale_image.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset):
    width_(dataset.Width()), height_(dataset.Height()) {
  double min = dataset.MinEle();
  double max = dataset.MaxEle();
  int shade_of_gray = 0;
  std::cout << "reaches here" << std::endl;
  std::vector<Color> row_color;
  for (size_t h = 0; h < height_; ++h) {
    row_color.clear();
    for (size_t w = 0; w < width_; ++w) {
      if (min == max) {
        shade_of_gray = 0;
      } else {
        shade_of_gray = (int)std::round(((double)dataset.DatumAt(h, w) - min) /
                                        (max - min) * (double)kMaxColorValue);
        // std::cout << shade_of_gray << std::endl;
      }
      Color c(shade_of_gray, shade_of_gray, shade_of_gray);
      row_color.push_back(c);
    }
    image_.push_back(row_color);
  }
  // std::cout << "reaches here pt 2" << std::endl;
}

GrayscaleImage::GrayscaleImage(const std::string& filename,
                               size_t width,
                               size_t height):
    width_(width), height_(height) {
  ElevationDataset dataset(filename, width_, height_);
  double min = dataset.MinEle();
  double max = dataset.MaxEle();
  int shade_of_gray = 0;

  std::vector<Color> row_color;
  for (size_t h = 0; h < height_; ++h) {
    row_color.clear();
    for (size_t w = 0; w < width_; ++w) {
      if (min == max) {
        shade_of_gray = 0;
      } else {
        shade_of_gray = (int)std::round(((double)dataset.DatumAt(h, w) - min) /
                                        (max - min) * (double)kMaxColorValue);
      }
      Color c(shade_of_gray, shade_of_gray, shade_of_gray);
      row_color.push_back(c);
    }
    image_.push_back(row_color);
  }
}

size_t GrayscaleImage::Width() const { return width_; }

size_t GrayscaleImage::Height() const { return height_; }

unsigned int GrayscaleImage::MaxColorValue() const { return kMaxColorValue; }

const Color& GrayscaleImage::ColorAt(int row, int col) const {
  return image_.at(row).at(col);
}

const std::vector<std::vector<Color>>& GrayscaleImage::GetImage() const {
  return image_;
}

void GrayscaleImage::ToPpm(const std::string& name) const {
  // "/home/vagrant/src/mp-mountain-paths-riyaj5246/obj"
  std::ofstream file(name, std::ios::out);
  file << "P3\n" << width_ << ' ' << height_ << '\n' << kMaxColorValue << '\n';
  if (!file.is_open()) {
    throw std::runtime_error("could not open ppm file");
  }
  for (size_t h = 0; h < height_; ++h) {
    for (size_t w = 0; w < width_; ++w) {
      file << image_.at(h).at(w).Red() << ' ' << image_.at(h).at(w).Green()
           << ' ' << image_.at(h).at(w).Blue() << ' ';
    }
    file << '\n';
  }
}
