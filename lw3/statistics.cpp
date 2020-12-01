#include "http_server.h"

void write_statistics(std::string ip, std::string ext) {
    pthread_mutex_lock(&content_mutex);
    if (content_map.find(ip) == content_map.end()) {
        content_map[ip][ext] = 1;
    }
    else {
        content_map[ip][ext]++;
    }
    pthread_mutex_unlock(&content_mutex);
}

void save_statistics(void) {
    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);

    std::string file = "statistics/";
    file += std::ctime(&end_time);
    file[file.length()-1] = '.';
    file += "txt";

    std::ofstream fout(file);
    for (const auto& ip : content_map) {
        fout << ip.first << " {\n";
        for (const auto& fmt : ip.second) {
            fout << "\t" << fmt.first << ": " << fmt.second << ",\n";
        }
        fout << "}\n";
    }
    fout.close();
}
