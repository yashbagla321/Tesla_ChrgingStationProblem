#include <iostream>
#include "network.h"
#include "distance.cpp"
#include <vector>
#include "dijkstra.cpp"


const int N = 303;        //num of stations
int max_range = 320;      //max chaege range of the car
int speed = 105;          // car speed
int inf = 99999999;

int main(int argc, char** argv)
{
    //Creating a distance Cost Matrix between stations
    double dist[N][N];
    for(int i = 0; i < N; i++)
    {
        for(int j = i; j < N; j++)
        {
            double d = distance(network[i].lat, network[i].lon, network[j].lat, network[j].lon);
            if(d > max_range) {
                d = inf;
            }

            dist[i][j] = d;
            dist[j][i] = d;
        }
    }

    //Creating a Time cost matrix between the stations to acoount for charging speed as well
    double time[N][N];
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            time[i][j] = ((dist[i][j])/network[i].rate) + (dist[i][j]/speed);
            if(dist[i][j] > max_range)
            {
                time[i][j] = inf;
            }
        }
    }

    // cmd input
    // std::string strt_statn;
    // std::cout << "Enter start charger name :" << std::endl;
    // getline(cin, strt_statn);

    // std::string end_statn;
    // std::cout << "Enter end charger name :" << std::endl;
    // getline(cin, end_statn);

    if (argc != 3)
    {
        cout << "Error: requires initial and final supercharger names";
        return -1;
    }

    std::string strt_statn = argv[1];
    std::string end_statn = argv[2];

    int ss, es;
    while(ss < network.size() && network[ss].name != strt_statn) ++ss;
    while(es < network.size() && network[es].name != end_statn) ++es;
    //  ss = 297;
    // es = 49;
    // shortest path calculation
    std::vector<int> path = dijkstra(time, ss, es);
    int pathlen = path.size();
    double range[pathlen];
    double charge_time[pathlen];

    double last_node_range = max_range;
    range[0] = max_range;
    charge_time[0] = 0;
    for (int i = 1; i < pathlen -1; i++) 
    {
        double remaining_range = last_node_range - dist[path[i-1]][path[i]];
        double range_needed = dist[path[i]][path[i+1]];

        if (range_needed > remaining_range) 
        {
            last_node_range = range_needed;
        } 
        else 
        {
            last_node_range = remaining_range;
        }


        double extra_range = last_node_range - remaining_range;

        range[i] = last_node_range;
        charge_time[i] = extra_range/network[path[i]].rate;

        // debugger
        // cout << network[i-1].name << " -> " << network[i].name << "\t prev dist: " << dist[path[i-1]][path[i]] << "\t dist: " << range_needed << " rr: ";
        // cout << remaining_range << " fr: " << last_node_range << " ER: " << extra_range << " RATE: ";
        // cout <<  network[i].rate << " CT: " << charge_time[i] << endl;
    }

    double total_time = 0;
    cout << network[path[0]].name << ", ";

    for (int i = 1; i < pathlen -1; i++)
    {
        row s = network[path.at(i)];
        cout << s.name << ", " << charge_time[i] << ", ";
        total_time = total_time + charge_time[i];
    }
    cout << network[path[pathlen -1]].name << endl;

    // cout <<" >>> TOTAL TIME: "<< total_time << endl;

    return 0;
}

