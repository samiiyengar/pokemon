//  PROJECT IDENTIFIER: 5949F553E20B650AB0FB2266D3C0822B13D248B0
//  poke.cpp
//  project4-pokemon
//
//  Created by Samyukta Iyengar on 6/16/20.
//  Copyright © 2020 Samyukta Iyengar. All rights reserved.
//

#include "graph.hpp"

void print_helper() {
    cout << "Options:\n"
        << "Type -h or --help to receive a help message\n"
        << "Type -m or --mode to specify the desired mode\n"
    << "Possible modes include MST, FASTTSP, and OPTTSP\n";
}

bool check_valid_mode(const string &mode) {
    if (mode == "MST" || mode == "FASTTSP" || mode == "OPTTSP") {
        return true;
    } else {
        return false;
    }
}

// helper function that reads file info and populates data structures
void read_file_contents(vector<Point> &points) {
    string temp;
    getline(cin, temp);
    int number_pokemon = stoi(temp);
    
    for (int i = 0; i < number_pokemon; i++) {
        Point p;
        getline(cin, temp, ' ');
        p.x = stoi(temp);
        getline(cin, temp);
        p.y = stoi(temp);
        points.push_back(p);
    }
}

// main function
int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    cout << setprecision(2) << fixed;
    
    // if argc is less than min number of arguments needed
    if (argc < 2) {
        print_helper();
        return 1;
    }
    
    // verify command line args
    string arg = argv[1];
    string mode;
    if (arg == "-h" || arg == "--help") {
        print_helper();
        return 0;
    } else if (arg == "-m" || arg == "--mode") {
        mode = argv[2];
    }
    
    // check if mode name is valid, print helpful message and exit if not
    if (!check_valid_mode(mode)) {
        print_helper();
        return 1;
    }
    
    // read from file and populate points vector
    vector<Point> points;
    read_file_contents(points);
    Graph g(points, mode);
    
    // create graphs and print output 
    if (mode == "MST") {
        g.create_MST();
        if (g.get_MST_possibility() == false) {
            cout << "Cannot construct MST\n";
            exit(1);
        }
        g.print_MST_output();
    } else if (mode == "FASTTSP") {
        //g.create_FASTTSP();
        g.create_FASTTSP2();
        g.print_FASTTSP_output();
    } else if (mode == "OPTTSP") {
        g.create_OPTTSP();
        g.print_OPTTSP_output();
    }
}
