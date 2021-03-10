//  PROJECT IDENTIFIER: 5949F553E20B650AB0FB2266D3C0822B13D248B0
//  graph.hpp
//  project4-pokemon
//
//  Created by Samyukta Iyengar on 6/16/20.
//  Copyright © 2020 Samyukta Iyengar. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <set>
#include <iterator>
#include <cmath>
#include <climits>
#include <iomanip>
#include <cassert>

using namespace std;

// point data struct
struct Point {
    int x = 0;
    int y = 0;
    bool visited = false;
};

// graph class
class Graph {
public:
    // constructor
    Graph(const vector<Point> &points_in, const string &mode_in) {
        points = points_in;
        mode = mode_in;
    }
    
    // MST FUNCTIONS BELOW
    
    // creates mst using points
    bool create_MST() {
        mst.clear();
        mst.resize(points.size());
        size_t index = 0;
        mst[0].weight = 0;
        double dist = 0;
        for (index = 0; index < points.size(); index++) {
            int parent = -1;
            double min = INT_MAX;
            for (size_t i = 0; i < points.size(); i++) {
                if (!mst[i].visited) {
                    if (mst[i].weight < min) {
                        min = mst[i].weight;
                        parent = static_cast<int>(i);
                    }
                }
            }
            if (parent == -1) {
                break;
            }
            mst[(size_t)parent].visited = true;
            for (size_t i = 0; i < points.size(); i++) {
                if (!mst[i].visited) {
                    dist = calculate_distance_MST(points[(size_t)parent], points[i]);
                    if (dist < 0) {
                        continue;
                    }
                    if (mst[i].weight > dist) {
                        mst[i].weight = dist;
                        mst[i].parent = parent;
                    }
                }
            }
        }
        for (size_t i = 0; i < mst.size(); i++) {
            if (!mst[i].visited) {
                MST_possible = false;
                return false;
            }
        }
        mst_weight = 0;
        for (size_t i = 0; i < mst.size(); i++) {
            if (mst[i].weight != INT_MAX) {
                mst_weight += mst[i].weight;
            }
        }
        return true;
    }
    
    // prints required mst output
    void print_MST_output() {
        cout << mst_weight << "\n";
        for (size_t i = 0; i < mst.size(); i++) {
            if (mst[i].parent == -1) {
                continue;
            }
            if (mst[i].weight != INT_MAX) {
                if (static_cast<int>(i) < mst[i].parent) {
                    cout << i << " " << mst[i].parent << "\n";
                }
                else {
                    cout << mst[i].parent << " " << i << "\n";
                }
            }
        }
    }
    
    // returns if mst is possible so program can exit 1 if it isn't
    bool get_MST_possibility() {
        return MST_possible;
    }
    
    // FASTTP FUNCTIONS BELOW
    
    void create_FASTTSP2() {
        // initialization
        best_path.push_back(0); // push 0 into best path vector
        points[0].visited = true; // set that point to visited
        best_path.push_back(1);
        points[1].visited = true;
        best_path.push_back(2);
        points[2].visited = true;
        double current_distance = 0; // current distance and min distance var declatation
        double min_distance = INT_MAX;
        size_t min_index = 0; // min index so we know which point is closest to the one already in there
        for (size_t i = 0; i < points.size(); i++) { // find the min index
            if (points[i].visited) {
                continue;
            }
            current_distance = calculate_distance(points[0], points[i], true);
            if (current_distance < min_distance) {
                min_distance = current_distance;
                min_index = i;
            }
        }
        // at this point you know what point is the closest to 0, push that into your path
        best_path.push_back(min_index); // push min index into best path
        points[min_index].visited = true; // set that point to visited
        // selecting k
        size_t k = 0;
        while (best_path.size() != points.size()) { // loop through and continue adding into path vector
            for (size_t i = 0; i < points.size(); i++) { // go through points vector and find the first one that hasn't been visited yet, set that equal to k
                if (!points[i].visited) {
                    k = i;
                    break;
                }
            } // now you have your k
            // finding i and j in path vector
            min_distance = INT_MAX;
            min_index = 0;
            for (size_t i = 0; i < best_path.size() - 1; i++) {
                // calculate your current distance with the given equation
                // set it equal to current distance
                current_distance = calculate_distance(points[best_path[i]], points[k], true) + calculate_distance(points[best_path[i + 1]], points[k], true) - calculate_distance(points[best_path[i]], points[best_path[i + 1]], true);
                if (current_distance < min_distance) {
                    min_distance = current_distance;
                    min_index = i;
                } // get min index so you know where to insert 
            }
            // you have your index that k needs to be inserted into
            // insertion
            best_path.insert(best_path.begin() + (int)min_index + 1, k);
            points[k].visited = true;
        }
        // calculate distance
        total_fast_tsp_distance = 0;
        for (size_t i = 0; i < best_path.size() - 1; i++) {
            total_fast_tsp_distance += calculate_distance(points[best_path[i]], points[best_path[i + 1]], true);
        }
        total_fast_tsp_distance += calculate_distance(points[best_path[best_path.size() - 1]], points[best_path[0]], true);
    }
    
    // creates fasttsp from points
    // using nearest arbitrary insertion
    void create_FASTTSP() {
        best_path.push_back(0);
        points[0].visited = true;
        best_path.push_back(1);
        points[1].visited = true;
        best_path.push_back(2);
        points[2].visited = true;
        while (best_path.size() != points.size()) {
            for (size_t i = 0; i < points.size(); i++) {
                if (points[i].visited) {
                    continue;
                }
                add_to_arc(i);
                points[i].visited = true;
            }
        }
        total_fast_tsp_distance = 0;
        for (size_t i = 0; i < best_path.size() - 1; i++) {
            total_fast_tsp_distance += calculate_distance(points[best_path[i]], points[best_path[i + 1]], true);
        }
        total_fast_tsp_distance += calculate_distance(points[best_path[best_path.size() - 1]], points[best_path[0]], true);
    }
    
    // prints formatted fasttstp output
    void print_FASTTSP_output() {
        cout << total_fast_tsp_distance << "\n";
        for (size_t i = 0; i < best_path.size(); i++) {
            cout << best_path[i] << " ";
        }
        cout << "\n";
    }
    
    // OPTTSP FUNCTIONS BELOW
    
    // creates opttsp from points
    void create_OPTTSP() {
        create_FASTTSP();
        total_opt_tsp_distance = total_fast_tsp_distance;
        vector<size_t> path_copy = best_path;
        build_OPTTSP_distance_matrix(best_path);
        genperms(path_copy, 1);
    }
    
    // builds distance matrix for opttsp
    void build_OPTTSP_distance_matrix(const vector<size_t>& path) {
        distance_matrix.resize(path.size());
        for (size_t i = 0; i < distance_matrix.size(); ++i) {
            distance_matrix[i].resize(path.size());
            for (size_t j = 0; j < distance_matrix[i].size(); ++j) {
                distance_matrix[i][j] = calculate_distance(points[i], points[j], true);
            }
        }
    }
    
    // creates opt mst, used in promising
    bool create_OPT_MST(size_t path_length, size_t remaining, size_t* remaining_path) {
        opt_mst.clear();
        opt_mst.resize(path_length);
        size_t index = 0;
        opt_mst[remaining_path[0]].weight = 0;
        double dist = 0;
        for (index = 0; index < remaining; index++) {
            int parent = -1;
            double min = INT_MAX;
            for (size_t i = 0; i < remaining; i++) {
                if (!opt_mst[remaining_path[i]].visited) {
                    if (opt_mst[remaining_path[i]].weight < min) {
                        min = opt_mst[remaining_path[i]].weight;
                        parent = static_cast<int>(remaining_path[i]);
                    }
                }
            }
            if (parent == -1) {
                break;
            }
            opt_mst[(size_t)parent].visited = true;
            for (size_t i = 0; i < remaining; i++) {
                if (!opt_mst[remaining_path[i]].visited) {
                    dist = distance_matrix[(size_t)parent][remaining_path[i]];
                    if (opt_mst[remaining_path[i]].weight > dist) {
                        opt_mst[remaining_path[i]].weight = dist;
                        opt_mst[remaining_path[i]].parent = parent;
                    }
                }
            }
        }
        opt_mst_weight = 0;
        for (size_t i = 0; i < opt_mst.size(); i++) {
            if (opt_mst[i].weight != INT_MAX) {
                opt_mst_weight += opt_mst[i].weight;
            }
        }
        return true;
    }
    
    // prints formatted opttsp output
    void print_OPTTSP_output() {
        cout << total_opt_tsp_distance << "\n";
        for (size_t i = 0; i < best_path.size(); i++) {
            cout << best_path[i] << " ";
        }
        cout << "\n";
    }
    
private:
    // edge struct
    struct Edge {
        Point start;
        Point end;
        int source;
        int dest;
        double distance;
        bool operator() (const Edge &e1, const Edge &e2) {
            return e1.distance > e2.distance;
        }
    };
    
    // mst details
    struct SpanningTreeNodeDetails {
        double weight = INT_MAX;
        bool visited = false;
        int parent = -1;
        SpanningTreeNodeDetails() {}
        SpanningTreeNodeDetails(double w, bool v, int p)
        : weight(w), visited(v), parent(p) {}
    };
    
    vector<SpanningTreeNodeDetails> mst;
    double mst_weight = 0;
    
    vector<SpanningTreeNodeDetails> opt_mst;
    double opt_mst_weight = 0;
    
    // vector of graph points
    vector<Point> points;
    
    // mode type
    string mode;
    
    // vector that contains edges
    vector<vector<Edge>> edges;
    
    // vector that contains source nodes
    vector<int> parent;
    
    // best path initially calculated by FASTTSP
    vector<size_t> best_path;
    
    // distance (BC)
    double total_fast_tsp_distance = 0;
    
    // distance (OPTTSP)
    double total_opt_tsp_distance = 0;
    
    // Distance matrix for OPTTSP
    vector <vector<double>> distance_matrix;
    
    // mst possible
    bool MST_possible = true;
    
    // HELPER FUNCTIONS BELOW
    
    // distance calculator that considers land/water/coast/crossability (A)
    double calculate_distance_MST(const Point &p1, const Point &p2) {
        if (!is_crossable(p1, p2)) {
            return -1;
        }
        double p1x = (double)p1.x;
        double p1y = (double)p1.y;
        double p2x = (double)p2.x;
        double p2y = (double)p2.y;
        double x_diff = p1x - p2x;
        double y_diff = p1y - p2y;
        return sqrt((x_diff * x_diff) + (y_diff * y_diff));
    }
    
    // calculates distance between two points without considering crossability (BC)
    double calculate_distance(const Point &p1, const Point &p2, bool allow_all = false) {
        if (!allow_all && !is_crossable(p1, p2)) {
            return -1;
        }
        double p1x = (double)p1.x;
        double p1y = (double)p1.y;
        double p2x = (double)p2.x;
        double p2y = (double)p2.y;
        double x_diff = p1x - p2x;
        double y_diff = p1y - p2y;
        return sqrt((x_diff * x_diff) + (y_diff * y_diff));
    }
    
    // checks if point is on water
    bool is_water(const Point &p) {
        if (p.x < 0 && p.y < 0) {
            return true;
        }
        return false;
    }
    
    // checks if point is on border between land and water
    bool is_coast(const Point &p) {
        if (p.x == 0 && p.y == 0) {
            return true;
        }
        if (p.y == 0 && p.x < 0) {
            return true;
        }
        if (p.x == 0 && p.y < 0) {
            return true;
        }
        return false;
    }
    
    // checks if point is on land
    bool is_land(const Point &p) {
        if (!is_water(p) && !is_coast(p)) {
            return true;
        }
        return false;
    }
    
    // checks if it's possible to cross between p1 and p2
    bool is_crossable(const Point &p1, const Point &p2) {
        if (is_water(p1) && is_land(p2)) {
            return false;
        }
        if (is_water(p2) && is_land(p1)) {
            return false;
        }
        return true;
    }
    
    // calculate distance for nearest abitrary insertion
    void add_to_arc(size_t point_in) {
        double current_tour_distance = 0;
        double min_tour_distance = INT_MAX;
        size_t min_index_j = 0;
        for (size_t i = 0; i < best_path.size() - 1; i++) {
            current_tour_distance = calculate_distance(points[best_path[i]], points[point_in], true) + calculate_distance(points[best_path[i + 1]], points[point_in], true) - calculate_distance(points[best_path[i]], points[best_path[i + 1]], true);
            if (current_tour_distance < min_tour_distance) {
                min_tour_distance = current_tour_distance;
                min_index_j = i + 1;
            }
        }
        best_path.insert(best_path.begin() + (int)min_index_j, point_in);
    }
    
    // genperms for part C
    template<typename T>
    void genperms(vector<T>& path, size_t perm_length) {
        if (path.size() == perm_length) {
            double path_weight = 0;
            for (size_t i = 0; i < perm_length; ++i) {
                if (i + 1 == perm_length) {
                    path_weight += calculate_distance(points[path[path.size() - 1]], points[path[0]], true);
                }  else {
                    path_weight += calculate_distance(points[path[i]], points[path[i + 1]], true);
                }
            }
            if (path_weight < total_opt_tsp_distance) {
                total_opt_tsp_distance = path_weight;
                best_path = path;
            }
            return;
        }
        if (!promising(path, perm_length)) {
            return;
        }
        for (size_t i = perm_length; i < path.size(); ++i) {
            swap(path[perm_length], path[i]);
            genperms(path, perm_length + 1);
            swap(path[i], path[perm_length]);
        }
    }
    
    // promising function for part C
    template<typename T>
    bool promising(vector<T>& path, size_t perm_length) {
        size_t* remaining_path = &path[perm_length];
        size_t remaining_size = path.size() - perm_length;
        // Create a MST for the unused portion of the path, i.e. the portion
        // after permLength. This will be used to compute the lower bound
        // for branch and bound.
        opt_mst_weight = 0;
        if (!create_OPT_MST(path.size(), remaining_size, remaining_path)) {
            return false;
        }
        double back_distance = 0;
        double front_distance = 0;
        double min_back_distance = INT_MAX;
        double min_front_distance = INT_MAX;
        // Compute the minimum front and back distances from the last node in
        // path and the remaining path. This will be used to compute the lower
        // bound for branch and bound.
        for (size_t i = 0; i < remaining_size; i++) {
            back_distance = distance_matrix[remaining_path[i]][path.back()];
            front_distance = distance_matrix[remaining_path[i]][path.front()];
            
            if (min_back_distance > back_distance) {
                min_back_distance = back_distance;
            }
            
            if (min_front_distance > front_distance) {
                min_front_distance = front_distance;
            }
        }
        // Calculate the current path length for the active portion of the permutation
        // i.e. the portion before permLength.
        double current_path_length = 0;
        double optimized_path_length = 0;
        for (size_t i = 0; i < perm_length - 1; i++) {
            optimized_path_length += distance_matrix[path[i]][path[i + 1]];
            current_path_length += calculate_distance(points[path[i]], points[path[i + 1]], true);
            assert(optimized_path_length == current_path_length);
        }
        if (opt_mst_weight + min_back_distance + min_front_distance + current_path_length > total_opt_tsp_distance) {
            return false;
        }
        return true;
    }
};

#endif /* graph_hpp */
