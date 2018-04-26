#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

void process(const string filename, 
             int& currentcode, 
             const vector<int> categories, 
             map<int, int>& catdict, 
             map<int, map<int, int> >& catcodes,
             const vector< vector<float> >& data);

vector< vector<float> > read_csv(const string filename, const int n_samples);

int main(int argc, char *argv[]) 
{
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

  int n_train = 0;
  int n_valid = 0;
  int n_test = 0;

  bool b_valid = false;
  string filename_valid;
  bool b_test = false;
  string filename_test;

  for (int i = 2; i < argc; ++i){
    if(argv[i] == string("--numeric")){
      if(argc > i){
        numeric_columns = atoi(argv[i+1]);
        cout << numeric_columns << " numeric columns" << endl;
      }
      else{
        cout << "The option '--numeric' requires to specify the number of numeric columns." << endl;
        return 1;
      }
    }

    if(argv[i] == string("--valid")){
      if(argc > i){
        b_valid = true;
        filename_valid = argv[i+1];
        cout << "valid file specified" << endl;
      }
      else{
        cout << "The option '--valid' requires to specify a filename argument" << endl;
        return 1;
      }
    }

    if(argv[i] == string("--test")){
      if(argc > i){
        b_test = true;
        filename_test = argv[i+1];
        cout << "test file specified" << endl;
      }
      else{
        cout << "The option '--test' requires to specify a filename argument" << endl;
        return 1;
      }
    }

    if(argv[i] == string("--n_train")){
      if(argc > i){
        n_train = atoi(argv[i+1]);
      }
      else{
        cout << "The option '--n_train' requires to specify the number of training samples." << endl;
        return 1;
      }
    }

    if(argv[i] == string("--n_valid")){
      if(argc > i){
        n_valid = atoi(argv[i+1]);
      }
      else{
        cout << "The option '--n_valid' requires to specify the number of valid samples." << endl;
        return 1;
      }
    }

    if(argv[i] == string("--n_test")){
      if(argc > i){
        n_test = atoi(argv[i+1]);
      }
      else{
        cout << "The option '--n_test' requires to specify the number of test samples." << endl;
        return 1;
      }
    }
  }

  vector< vector<float> > data = read_csv(filename, n_train);

  size_t lastindex = filename.find_last_of("."); 
  filename = filename.substr(0, lastindex);

  vector< vector<float> > data_valid;
  vector< vector<float> > data_test;

  if(b_valid){
    data_valid = read_csv(filename_valid, n_valid);
    size_t lastindex = filename_valid.find_last_of("."); 
    filename_valid = filename_valid.substr(0, lastindex);
  }  
  if(b_test){
    data_test = read_csv(filename_test, n_test);
    size_t lastindex = filename_test.find_last_of("."); 
    filename_test = filename_test.substr(0, lastindex);
  }  


  // Process data
  cout << "number of rows: " << data.size() << endl;
  cout << "number of columns: " << data[0].size()-1 << endl;

  vector<int> categories = vector<int>(data[0].size()-1, 1);

  map<int, int> catdict;
  map<int, map<int, int> > catcodes;
  int currentcode = numeric_columns; 

  for(int i = 0; i < categories.size(); ++i)
  {
    catdict[i] =  (i < numeric_columns) ? 0 : 1;
    cout << catdict[i];
  }
  cout << endl;

  process(filename, currentcode, categories, catdict, catcodes, data);
  if(b_valid)
    process(filename_valid, currentcode, categories, catdict, catcodes, data_valid);
  if(b_test)
    process(filename_test, currentcode, categories, catdict, catcodes, data_test);
  
  cout << "finished" << endl;
  
  return 0;
}

void process(const string filename, 
             int& currentcode, 
             const vector<int> categories, 
             map<int, int>& catdict, 
             map<int, map<int, int> >& catcodes,
             const vector< vector<float> >& data)
{
  string datastring = "";
  ofstream output_file;
  output_file.open(filename + "_ffmGen.txt");

  for (int i = 0; i < data.size(); ++i){
    if( i%1000000 == 0)
      cout << i << " processed" << endl;
    datastring = to_string(static_cast<int>(data[i][0])); // string containing just label

    for(int j = 0; j < data[i].size()-1; ++j){
      if(catdict[j] == 0){ // numeric column
        datastring = datastring + " " + to_string(j) + ":" + to_string(j) + ":" + to_string(data[i][j+1]);
      }
      else{ // categoric column
        if(catcodes.find(j+1) == catcodes.end()){
          catcodes[j+1][data[i][j+1]] = ++currentcode;
        }
        else if(catcodes[j+1].find(data[i][j+1]) == catcodes[j+1].end()){
          catcodes[j+1][data[i][j+1]] = ++currentcode;
        }
        const int code = catcodes[j+1][data[i][j+1]];
        datastring = datastring + " " + to_string(j) + ":" + to_string(code) + ":1";
      }
    }
    output_file << datastring << endl;
  }
  output_file.close();
}

// Read csv into 2d vector
vector< vector<float> > read_csv(const string filename, const int n_samples) 
{ 
  vector< vector<float> > data(n_samples);
  string item;

  vector<float> valueline;
  ifstream fin(filename);

  int n_line = 0;
  for (string line; getline(fin, line); )
  {
    istringstream in(line);

    while (getline(in, item, ','))
    {
      valueline.push_back(atof(item.c_str()));
    }

    if(n_line >= n_samples)
      data.push_back(valueline);
    else
      data[n_line] = valueline;
    valueline.clear();

    ++n_line;
  }

  return data;
}


