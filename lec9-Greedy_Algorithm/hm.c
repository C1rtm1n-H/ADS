#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double price;
    double dist;
} Station;

int compareStations(const void* a, const void* b) {
    Station* s1 = (Station*)a;
    Station* s2 = (Station*)b;
    if (s1->dist < s2->dist) return -1;
    if (s1->dist > s2->dist) return 1;
    return 0;
}

int main() {
    double C_max, Dist, D_avg;
    int N;

    if (scanf("%lf %lf %lf %d", &C_max, &Dist, &D_avg, &N) != 4) {
        return 1;
    }

    // 为N个加油站和1个虚拟终点站分配空间
    Station* stations = (Station*)malloc((N + 1) * sizeof(Station));

    for (int i = 0; i < N; i++) {
        if (scanf("%lf %lf", &stations[i].price, &stations[i].dist) != 2) {
            free(stations);
            return 1;
        }
    }

    // 将终点视为一个距离为Dist，价格为0的虚拟加油站
    stations[N].price = 0.0;
    stations[N].dist = Dist;

    // 对所有站点（包括虚拟终点）按距离排序
    qsort(stations, N + 1, sizeof(Station), compareStations);

    if (stations[0].dist != 0) {
        printf("The maximum travel distance = 0.00\n");
        free(stations);
        return 0;
    }

    double current_gas = 0.0;
    double total_cost = 0.0;
    int current_station_idx = 0;
    double max_dist_per_tank = C_max * D_avg;

    while (stations[current_station_idx].dist < Dist) {
        double current_pos = stations[current_station_idx].dist;
        double max_reach_from_current = current_pos + max_dist_per_tank;

        int next_station_idx = -1;
        int found_cheaper = 0;

        // 在可达范围内寻找下一个目标
        for (int i = current_station_idx + 1; i <= N && stations[i].dist <= max_reach_from_current; i++) {
            // 修正后的情况A: 找到第一个（也就是最近的）比当前便宜的站
            if (stations[i].price < stations[current_station_idx].price) {
                next_station_idx = i;
                found_cheaper = 1;
                break; // 立刻确定目标，不再向后找
            }
        }

        // 如果没有找到更便宜的站，再在可达范围内找最便宜的“贵”站
        if (!found_cheaper) {
            double min_expensive_price = 1e9;
            for (int i = current_station_idx + 1; i <= N && stations[i].dist <= max_reach_from_current; i++) {
                if (stations[i].price < min_expensive_price) {
                    min_expensive_price = stations[i].price;
                    next_station_idx = i;
                }
            }
        }

        // 如果可达范围内没有任何加油站
        if (next_station_idx == -1) {
            double farthest_reach = current_pos + max_dist_per_tank;
            printf("The maximum travel distance = %.2f\n", farthest_reach);
            free(stations);
            return 0;
        }

        // 根据找到的目标执行行动
        if (found_cheaper) { // 情况A: 去最近的便宜站
            double dist_to_next = stations[next_station_idx].dist - current_pos;
            double needed_gas = dist_to_next / D_avg;
            if (current_gas < needed_gas) {
                total_cost += (needed_gas - current_gas) * stations[current_station_idx].price;
                current_gas = 0;
            } else {
                current_gas -= needed_gas;
            }
        } else { // 情况B: 去最便宜的贵站
            double gas_to_add = C_max - current_gas;
            total_cost += gas_to_add * stations[current_station_idx].price;
            double dist_to_next = stations[next_station_idx].dist - current_pos;
            current_gas = C_max - dist_to_next / D_avg;
        }
        current_station_idx = next_station_idx;
    }
    
    printf("%.2f\n", total_cost);
    free(stations);
    return 0;
}