#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <set>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, k;
    std::cin >> n >> k;
    std::vector<std::vector<std::pair<unsigned int, unsigned int> > > edges(n);
    for (unsigned int i = 0; i < k; ++i) {
        unsigned int amount;
        std::cin >> amount;
        for (unsigned int j = 0; j < amount; ++j) {
            unsigned int from, to;
            std::cin >> from >> to;
            --from;
            --to;
            edges[from].emplace_back(to, i);
            edges[to].emplace_back(from, i);
        }
    }

    constexpr unsigned int kDistanceMax = (1 << 30);
    std::vector<std::vector<unsigned int> > distances(n,
                                                      std::vector<unsigned int>(k, kDistanceMax));
    std::queue<std::pair<unsigned int, unsigned int> > update_queue;
    std::set<std::pair<unsigned int, unsigned int> > to_update;
    for (unsigned int i = 0; i < k; ++i) {
        distances[0][i] = 1;
        update_queue.emplace(0, i);
        to_update.emplace(0, i);
    }
    while (!to_update.empty()) {
        std::pair<unsigned int, unsigned int> current = update_queue.front();
        update_queue.pop();
        to_update.erase(current);
        for (const std::pair<unsigned int, unsigned int>& edge : edges[current.first]) {
            unsigned int new_distance =
                distances[current.first][current.second] + (current.second == edge.second ? 0 : 1);
            if (new_distance < distances[edge.first][edge.second]) {
                distances[edge.first][edge.second] = new_distance;
                if (to_update.find(edge) == to_update.end()) {
                    to_update.insert(edge);
                    update_queue.push(edge);
                }
            }
        }
    }

    unsigned int ans = kDistanceMax;
    for (unsigned int i = 0; i < k; ++i) {
        ans = std::min(ans, distances[n - 1][i]);
    }
    if (ans == kDistanceMax) {
        std::cout << -1;
    } else {
        std::cout << ans;
    }
    return 0;
}