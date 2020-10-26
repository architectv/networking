#include "http_server.h"

std::map<std::string, std::string> get_headers(std::string header_text) {
    std::map<std::string, std::string> headers;
    std::istringstream header_list{ header_text };
    std::string line;
    while (std::getline(header_list, line) && line != "\n") {
        auto name_end = line.find(':');

        auto name = line.substr(0, name_end);
        auto value = line.substr(name_end + 2, line.length() - name_end - 1);

        headers[name] = value;
    }
    return headers;
}

std::string get_start_response_line(
    std::string protocol, int status_code, std::string status_string) {
    auto line = protocol + " " + std::to_string(status_code)
        + " " + status_string;
    return line;
}

std::string response_format(
    std::string protocol, int status_code, std::string status_string,
    std::map<std::string, std::string> headers, std::string body) {
    std::string response;
    auto start_response = get_start_response_line(
        protocol, status_code, statuses.at(status_code));
    response += start_response + "\n";

    if (status_code != 200) {
        auto response_file = "static/" + std::to_string(status_code) + ".html";
        std::ifstream infile{ response_file };
        body = std::string{
            std::istreambuf_iterator<char>(infile),
            std::istreambuf_iterator<char>()
        };
        infile.close();
        headers["Content-Type"] = content_types.at(".html");
        headers["Content-Length"] = std::to_string(body.length());
    }

    std::string v[3] = {
    "Content-Type",
    "Content-Length",
    "Connection",
    };
    for (const auto& header : v) {
        if (headers.find(header) != headers.end()) {
            response += header + ": " + headers.at(header) + "\n";
        }
    }

    response += "\n" + body;
    printf("%s\n", start_response.c_str());
    return response;
}

std::tuple<std::string, std::string, std::string>
split_start_request_line(std::string line) {
    auto method_end = line.find(' ');
    auto path_end = line.find(' ', method_end + 1);

    auto method = line.substr(0, method_end);
    auto path = line.substr(method_end + 1, path_end - method_end - 1);
    auto protocol = line.substr(path_end + 1, line.length() - path_end - 1);
    if (path[0] == '/' && path.length() != 1) {
        path = path.substr(1, path.length() - 1);
    }
    else {
        path = "index.html";
    }
    return { method, path, protocol };
}

std::string get_content_type(std::string ext) {
    std::string content_type;
    if (content_types.find(ext) == content_types.end()) {
        content_type = "text/plain";
    }
    else {
        content_type = content_types.at(ext);
    }
    return content_type;
}

int check_host(std::map<std::string, std::string> headers) {
    int status = 200;
    if (headers.find("Host") == headers.end()) {
        status = 404;
    }
    else {
        auto host = headers.at("Host");
        auto port = std::to_string(PORT);
        if (host.compare("127.0.0.1:" + port) != 0
            && host.compare("localhost:" + port) != 0
            && host.compare("127.0.0.1:" + port + "\r") != 0
            && host.compare("localhost:" + port + "\r") != 0) {
            status = 404;
        }
    }
    return status;
}

std::string get_extension(std::string path) {
    auto dot_pos = path.find('.');
    auto ext = path.substr(dot_pos, path.length() - dot_pos);
    return ext;
}

std::string get_response(client_t client, std::string request) {
    std::string response;
    int status = 200;
    std::string status_string = "OK";
    std::map<std::string, std::string> response_headers;
    response_headers["Connection"] = "close";
    std::string body;

    auto line_end = request.find('\n');
    auto line = request.substr(0, line_end);
    printf("\n[%s]  request: %s\n", client.ip.c_str(), line.c_str());
    std::cout << request << std::endl;
    printf("[%s] response: ", client.ip.c_str());

    auto headers = get_headers(request.substr(line_end + 1, request.length() - line_end));
    status = check_host(headers);
    if (status != 200) {
        response = response_format(
            "HTTP/1.1", status, statuses.at(status),
            response_headers, body
        );
        return response;
    }

    auto [method, path, protocol] = split_start_request_line(line);
    if (method.compare("GET") != 0 || protocol.compare("HTTP/1.1") != 0) {
        status = 405;
        response = response_format(
            protocol, status, statuses.at(status),
            response_headers, body
        );
        return response;
    }

    std::ifstream infile{ path };
    if (!infile.good()) {
        status = 404;
        response = response_format(
            protocol, status, statuses.at(status),
            response_headers, body
        );
        return response;
    }
    body = std::string{
        std::istreambuf_iterator<char>(infile),
        std::istreambuf_iterator<char>()
    };
    infile.close();

    auto ext = get_extension(path);
    write_statistics(client.ip, ext);

    response_headers["Content-Type"] = get_content_type(ext);
    int idx = response_headers["Content-Type"].find("image");
    if (idx != std::string::npos) {
        body = string_to_hex(body);
    }
    response_headers["Content-Length"] = std::to_string(body.length());

    response = response_format(
        protocol, status, statuses.at(status),
        response_headers, body
    );

    return response;
}
