/* This is skeleton code for the Two-Pass Multi-Way Merge Sort algorithm, you can make modifications as long as you meet 
   all question requirements*/  
// Currently, this program will just load the records into buffers (main memory) and print the records once the buffers 
// are filled. This process will continue until all of Emp.csv is read.

#include <bits/stdc++.h>
using namespace std;

//defines how many buffers are available in the Main Memory 
#define buffer_size 22

struct EmpRecord {
    int eid;
    string ename;
    int age;
    double salary;
};
EmpRecord buffers[buffer_size]; // this structure contains 22 buffers; available as your main memory.

// Grab a single block from the Emp.csv file. In theory, if a block was larger than
// one tuple, this function would return an array of EmpRecords (one block worth) 
// and the entire if-then-else statement would be wrapped in a loop that would read
// tuples in until the block size was hit.
EmpRecord Grab_Emp_Record(fstream &empin) {
    string line, word;
    EmpRecord  emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

// Print whatever is stored in the buffers of Main Memory 
// Can come in handy to see if you've sorted the records in your main memory properly.
void Print_Buffers(int cur_size) {
    for (int i = 0; i < cur_size; i++)
    {
        cout << i << " " << buffers[i].eid << " " << buffers[i].ename << " " << buffers[i].age << " " << buffers[i].salary << endl;
    }
}

// Sort the buffers in main memory based on 'eid' and then store those sorted records in
// a temporary file on disk (create a run and store it as a file on disk).
// You can change the return type and arguments as you see fit. 
void Sort_in_Main_Memory(int run_num){
    //cout << "Sorting Not Implemented" << endl;
    sort(buffers, buffers + buffer_size, [](const EmpRecord &a, const EmpRecord &b)
      { return a.eid < b.eid; });
  
  // Store the sorted records in a temporary file on disk
  string file_name = "run" + to_string(run_num) + ".csv";
  ofstream output_file(file_name);
  for (int i = 0; i < buffer_size; i++) {
    output_file << buffers[i].eid << "," << buffers[i].ename << "," << buffers[i].age << "," << buffers[i].salary << endl;
  }
  output_file.close();
}
int GetNumRecords(const string &file_name) {
  int num_records = 0;
  string line;
  ifstream input_file(file_name);
  while (getline(input_file, line)) {
    num_records++;
  }
  input_file.close();
  return num_records;
}
// Merges your M-1 runs (from disk) using the buffers in main memory and stores them in 
// a sorted file called 'EmpSorted.csv'(The Final Output File).
// You can change the return type and arguments as you see fit.
void Merge_Runs_in_Main_Memory(int run_counter, int emp_file_size){
  ifstream run_files[run_counter];
  EmpRecord cur_record;
  //opens run files
  
  for (int i = 0; i < run_counter; i++)
  {
    run_files[i].open("run" + to_string(i) + ".csv");
    //printf("File run%d.csv Opened\n",i);
  }
  
    ofstream output_file("EmpSorted.csv");

    for (int i = 0; i < emp_file_size; i++)
    {
        int low_eid = INT_MAX;
        int cur_file = -1;
        for (int j = 0; j < run_counter; j++){
        //checks if we have reached the end of the current file
        //and jumps to the next file 
            if(run_files[j].eof()){
                continue;
            }
        // checks for the lowest eid in the run_files
            string line;
            getline(run_files[j],line);
            stringstream stream(line);
            stream >> cur_record.eid;
            stream.ignore(); // ignore the comma after eid
            getline(stream, cur_record.ename, ',');
            stream >> cur_record.age;
            stream >> cur_record.salary;
            if(cur_record.eid < low_eid){
                low_eid = cur_record.eid;
                cur_file = j;
                }
        }
        //checks to make there are stil records to be read from the files
        if(cur_file == -1){
            break;
            printf("in break at index[%d]\n",i);
        }
        output_file << low_eid << "," << cur_record.ename << "," << cur_record.age << "," << cur_record.salary << endl; 
    }
    output_file.close();
}

int main() {
  // open file streams to read and write
  fstream input_file;
  int run_counter = 0;
  int emp_file_size = GetNumRecords("Emp.csv");
  input_file.open("Emp.csv", ios::in);
  // flags to check when relations are done being read
  bool flag = true;
  int cur_size = 0;
  
  /*First Pass: The following loop will read each block, put it into main_memory,
    sort them, and then will put write them to a temporary file (as a run) for 2nd pass */
  while (flag) {
      // FOR BLOCK IN RELATION EMP

      // grabs a block
      EmpRecord  single_EmpRecord  = Grab_Emp_Record(input_file);
      // checks if filestream is empty
      if (single_EmpRecord.eid == -1) {
          flag = false;
          Print_Buffers(cur_size); // The main_memory is not filled up but there are some leftover tuples that need to be sorted.
      }
      if(cur_size + 1 <= buffer_size){
          // Memory is not full. Store the current record into a buffer.
          buffers[cur_size] = single_EmpRecord ;
          cur_size += 1;
      }
      else{
          // Memory is now full. Sort the tuples in Main Memory and store them in a temporary file (runs)
          cout << "Main Memory is full. Time to sort and store sorted blocks in a temporary file" << endl;
          //Print_Buffers(buffer_size);
          //SortMain("Attributes You Want");
          Sort_in_Main_Memory(run_counter);
//========Creates 18 files with 22 people 15 people not in file the 
//========the other 15 are on buffers[] not acounting for those at the moment
          //printf("File Created run%d.csv\n",run_counter);
          run_counter++;
          // After sorting, start again. Clear memory and put the current tuple into main memory.
          cur_size = 0;
          buffers[cur_size] = single_EmpRecord;
          cur_size += 1;
      }
      
  }
  input_file.close();
  
  /* Implement 2nd Pass: Read the temporary sorted files (runs) and sort them as discussed in class. 
	 Write the final output (fully sorted Emp relation) to EmpSorted.csv*/

  // Uncomment when you are ready to store the sorted relation
   fstream sorted_file;  
   sorted_file.open("EmpSorted.csv", ios::out | ios::app);

  // Pseudocode
  //bool flag_sorting_done = false;
  //while(!flag_sorting_done){
  //==========Only returns 24 people and the 24th is the same name and age as the 23rd
  //==========the salary being passed from the created files into the sorted file are all 0  
      Merge_Runs_in_Main_Memory(run_counter, emp_file_size);
  //    break;
  //}
  
  // You can delete the temporary sorted files (runs) after you're done in order to keep things clean and tidy.
    for (int i = 0; i < run_counter; i++)
    {
      string file_name = "run" + to_string(i) + ".csv";
     remove(file_name.c_str());
   }
    
  return 0;
}
