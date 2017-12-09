#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
  /*for(int i = 0; i < argc; ++i){
    cout << argv[i] << endl;
  }
  cout << "----" << endl;
  */
  string filename;
  if(argc > 1){
    filename = argv[1];
    cout << "Using file " << filename << endl;
  }
  else{
    cout << "No filename given." << endl;
    return 1;
  }

  // Parsing command line parameters
  int numeric_columns = 0;

  for (int i = 2; i < argc; ++i){
    //cout << "Considering " << argv[i] << endl;
    if(argv[i] == string("-numeric")){
      //cout << "Found numeric" << endl;
      if(argc > i){
        numeric_columns = atoi(argv[i+1]);
        cout << numeric_columns << " numeric columns" << endl;
      }
      else{
        cout << "The option '-numeric' requires to specify the number of numeric columns." << endl;
        return 1;
      }
    }
  }

  vector< vector<double> > data;

  // Read csv into 2d vector
  vector<double> valueline;
  ifstream fin(filename);

  size_t lastindex = filename.find_last_of("."); 
  filename = filename.substr(0, lastindex);

  string item;

  for (string line; getline(fin, line); )
  {
    istringstream in(line);

    while (getline(in, item, ','))
    {
      valueline.push_back(atof(item.c_str()));
    }

    data.push_back(valueline);
    valueline.clear();
  }

  // Process data
  cout << "number of rows: " << data.size() << endl;
  cout << "number of columns: " << data[0].size()-1 << endl;

  vector<int> categories = vector<int>(data[0].size()-1, 1);

  map<int, int> catdict;
  map<int, map<int, int> > catcodes;
  int currentcode = numeric_columns; 

  for(int i = 0; i < categories.size(); ++i){
    catdict[i] =  (i < numeric_columns) ? 0 : 1;
    cout << catdict[i];
  }
  cout << endl;

  string datastring = "";
  ofstream output_file;
  output_file.open(filename + "_ffmGen.txt");
  for (int i = 0; i < data.size(); ++i){
    if( i%100000 == 0)
      cout << i << " processed" << endl;
    datastring = to_string(static_cast<int>(data[i][0]));

    for(int j = 1; j < data[i].size(); ++j){
      if(catdict[j] == 0){ // numeric column
        datastring = datastring + " " + to_string(i) + ":" + to_string(i) + ":" + to_string(data[i][j]);
      }
      else{ // categoric column
        if(catcodes.find(j) == catcodes.end()){
          catcodes[j][data[i][j]] = ++currentcode;
        }
        else if(catcodes[j].find(data[i][j]) == catcodes[j].end()){
          catcodes[j][data[i][j]] = ++currentcode;
        }
        const int code = catcodes[j][data[i][j]];
        datastring = datastring + " " + to_string(j-1) + ":" + to_string(code) + ":1";
      }
    }
    output_file << datastring << endl;
  }
  output_file.close();

  return 0;
}


