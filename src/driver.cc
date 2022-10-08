#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path_image.hpp"

int main() {
  const size_t kWidth = 5;
  const size_t kHeight = 2;
  ElevationDataset e_dataset(
      "/home/vagrant/src/mp-mountain-paths-riyaj5246/example-data/"
      "ex_input_data/prompt_5w_2h.dat",
      kWidth,
      kHeight);

  GrayscaleImage grayscale(e_dataset);
  grayscale.ToPpm("trial_file");

  PathImage path_image(grayscale, e_dataset);
  path_image.ToPpm("path_file");

  return 0;
}