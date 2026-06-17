#include <TFile.h>
#include <TH1F.h>
#include <TRandom3.h>

#include <iostream>

int main() {
  std::cout << "--- Starting ROOT Data Visualization Program ---" << std::endl;

  // 1. Create a ROOT file to save our data. "RECREATE" overwrites it if it
  // exists.
  TFile* outputFile = new TFile("gaussian_data.root", "RECREATE");

  // 2. Create a 1D Histogram float object.
  // Arguments: (Internal Name, Display Title, Number of Bins, X-axis Min,
  // X-axis Max)
  TH1F* histogram =
      new TH1F("h_gaus", "Beautiful Gaussian Distribution;Value (X);Counts",
               100, -5.0, 5.0);

  // 3. Create a random number generator
  TRandom3 randGenerator(0);  // 0 means seed dynamically based on time

  std::cout << "Generating 50,000 data points..." << std::endl;
  for (int i = 0; i < 50000; ++i) {
    // Generate a random number tracking a Gaussian distribution (mean=0,
    // sigma=1)
    double value = randGenerator.Gaus(0.0, 1.0);
    histogram->Fill(value);
  }

  // 4. Save the histogram inside the ROOT file
  outputFile->Write();

  // 5. Clean up memory
  outputFile->Close();
  delete outputFile;

  std::cout << "Done! Saved data to 'gaussian_data.root'." << std::endl;
  return 0;
}