// FileToHeader.cpp : Defines the entry point for the application.
// By Haydn V. Harach

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>

int main(int argc, char* argv[]) {

  using namespace std;

  // Make sure we actually have an arg to use.
  if (argc < 2) {
    cerr << "Not enough arguments.\n";
    return 2;
  }

  vector<string> inputs;
  vector<string> outputs;

  // Go through the list of arguments
  {
    bool isInput = false;
    bool isOutput = false;
    for (int whicharg = 1; whicharg < argc; ++whicharg) {
      string thisarg(argv[whicharg]);
      if (thisarg == "-i") {
        isInput = true;
        isOutput = false;
        continue;
      }
      if (thisarg == "-o") {
        isInput = false;
        isOutput = true;
        continue;
      }
      if (isInput) {
        inputs.push_back(thisarg);
        continue;
      }
      if (isOutput) {
        outputs.push_back(thisarg);
        continue;
      }
      // If we made it this far, it's a bad arg.
      cerr << "Bad argument. Inputs should follow '-i' and outputs should follow '-o'." << endl;
      return 3;
    }
    if (inputs.size() != outputs.size()) {
      cerr << "Number of inputs and outputs must match!" << endl;
      return 4;
    }
  }

  // Go through each file indicated
  for (size_t i = 0; i < inputs.size(); ++i) {
    //cout << "Writing file '" << inputs[i] << "' to '" << outputs[i] << "'...   ";

    // Turn argv[i] into a file path.
    filesystem::path mypath(inputs[i]);
    if (!filesystem::is_regular_file(mypath)) {
      cerr << "Not a regular file." << endl;
      continue;
    }

    // Open the file and get its size.
    ifstream infile(mypath, ios::in | ios::binary);
    if (!infile.is_open()) {
      cerr << "Can't open input file." << endl;
      continue;
    }

    // Read the file data.
    size_t size = filesystem::file_size(mypath);
    vector<char> filedata(size);
    if (!infile.read(filedata.data(), size)) {
      cerr << "Failed to read input file." << endl;
      continue;
    }

    // Open the output file.
    string outpath = outputs[i];
    ofstream outfile(outpath, ios::out);
    if (!outfile.is_open()) {
      cerr << "Can't open output file." << endl;
      continue;
    }

    // Get the filename we'll be working with and turn any '.' or ' ' into '_'.
    string filename = mypath.filename().u8string();
    for (char& c : filename) {
      if (c == '.' || c == ' ') { c = '_'; }
    }

    // Write the data to the output file.
    outfile << "#ifndef FILE_" << filename << "_H\n";
    outfile << "#define FILE_" << filename << "_H\n";
    outfile << "#define FILE_" << filename << "_SIZE " << filedata.size() << "\n";
    outfile << "static const unsigned char FILE_" << filename << "_DATA[] = {\n";

    int chars_written_this_line = 0;
    const int MAX_CHARS_PER_LINE = 79;
    for (size_t i = 0; i < filedata.size(); ++i) {

      int chars_needed = 1;
      unsigned char thisdata = (unsigned char)filedata[i];
      if (thisdata >= 10) chars_needed++;
      if (thisdata >= 100) chars_needed++;
      if (i + 1 < filedata.size()) chars_needed++;
      if (chars_written_this_line + chars_needed > MAX_CHARS_PER_LINE) {
        outfile << '\n';
        chars_written_this_line = 0;
      }

      outfile << (int)thisdata;
      if (i + 1 < filedata.size()) { outfile << ","; }
      chars_written_this_line += chars_needed;
    }
    outfile << "\n};\n";
    outfile << "#endif // FILE_" << filename << "_H\n";
    //cout << "done!" << endl;
  }

  return 0;
}
