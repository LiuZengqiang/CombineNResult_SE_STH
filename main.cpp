// Combine n result(.scv)
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

using namespace std;

void addResult(vector<string> &in_file_path);

void process(float percent);

void saveResult(string output_file, vector<string> &result, int cnt);

int main() {
//    vector<int> data_list = {1024, 512, 256};
    vector<string> input_file_paths;
    string pre = "/home/sth/CLionProjects/SolarEnergy_Chier/OutputFiles/ExperimentTimes_output/2048/0_";
    string suf = "_receiver_after_smooth.csv";
    for (int i = 0; i < 1024; i++) {
        input_file_paths.push_back(pre + to_string(i) + suf);
    }


    addResult(input_file_paths);


    process(1.0);
    return 0;
}

void process(float percent) {
    int num = static_cast<int>(percent / 0.1f);
    cout << "[";
    for (int i = 0; i < num; i++) {
        cout << "-";
    }
    cout << ">";
    for (int i = num; i < 10; i++) {
        cout << " ";
    }
    cout << static_cast<int>(percent * 100) << "%" << endl;
}

void addResult(vector<string> &input_file_path) {

    assert(input_file_path.empty() == false);
    vector<string> result;
    for (int i = 0; i < input_file_path.size(); i++) {

        fstream input_file(input_file_path[i]);
        if (!input_file.good()) {
            cout << "open \"" << input_file_path[i] << "\" fail." << endl;
            return;
        }

        stringstream input_stream;
        input_stream << input_file.rdbuf();
        input_file.close();
        string str;
        if (i == 0) {
            while (getline(input_stream, str)) {
                if (str.length() >= 1) {
                    result.push_back(str);
                }
            }
        } else {
            int r = 0;
            while (getline(input_stream, str)) {
                if (r && str.length() >= 1) {
                    int index = str.find_last_of(',');
                    float val_1 = atof(str.substr(index + 1).c_str());
                    index = result[r].find_last_of(',');
                    float val_2 = atof(result[r].substr(index + 1).c_str());
                    float val_sum = val_1 + val_2;
                    result[r] = result[r].substr(0, index + 1) + to_string(val_sum);
                }
                r++;
            }

        }

        saveResult("/home/sth/CLionProjects/SolarEnergy_Chier/OutputFiles/ExperimentTimes_output/2048_result/0_0_" +
                   to_string(i) + "_receiver_after_smooth.csv", result, i + 1);

        if (i % 10 == 0) {
            process(static_cast<float>(i) / input_file_path.size());
        }
    }

    process(1.0f);
}

void saveResult(string output_file_path, vector<string> &result, int cnt) {
    ofstream out_file(output_file_path);
    if (!out_file.good()) {
        cout << "open \"" << output_file_path << "\" fail." << endl;
        return;
    }

    for (int i = 0; i < result.size(); i++) {
        if (i == 0) {
            out_file << result[i];
            out_file << endl;
        } else {
            int index = result[i].find_last_of(',');
            string temp_str = result[i].substr(0, index + 1);
            float val = atof(result[i].substr(index + 1).c_str());
            val /= cnt;
            stringstream ss;
            ss << val;
            string suf;
            ss >> suf;
            temp_str = temp_str + suf;
            out_file << temp_str;
            out_file << endl;
        }
    }
    out_file.close();
}
