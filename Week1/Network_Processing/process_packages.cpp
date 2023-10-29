#include <iostream>
#include <queue>
#include <vector>

struct Request {
    Request(int arrival_time, int process_time):
        arrival_time(arrival_time),
        process_time(process_time)
    {}

    int arrival_time;
    int process_time;
};

struct Response {
    Response(bool dropped, int start_time):
        dropped(dropped),
        start_time(start_time)
    {}

    bool dropped;
    int start_time;
};

class Buffer {
public:
    Buffer(int size):
        size_(size),
        finish_time_()
    {}

    Response Process(const Request &request) {
    // If the buffer is empty and the processor is not busy, process the packet immediately
        if (finish_time_.empty()) {
            finish_time_.push(request.arrival_time + request.process_time); // finish time for a packet = arrival time + processing time
            return Response(false, request.arrival_time);
        }

    // Find the earliest time when the processor becomes available
        int earliest_finish_time = finish_time_.front();

    // If the buffer is full, drop the packet
        if (finish_time_.size() >= size_) {
            return Response(true, -1);  // Packet dropped
        }

    // If the packet arrives before the processor is available, process it immediately
        if (request.arrival_time < earliest_finish_time) {
            finish_time_.push(earliest_finish_time + request.process_time); // the finish time of the incoming packet is = 
                                                                            //earliest finish time of current packet that is being processed +
                                                                            // the processing time required for our incoming packet
            return Response(false, earliest_finish_time);
        }

    // If the packet arrives after the processor becomes available, process it after the current finish time
        finish_time_.push(request.arrival_time + request.process_time);
        return Response(false, request.arrival_time);
    }

private:
    int size_;
    std::queue <int> finish_time_; // stores the finish times of the packets currently in buffer
};
 
std::vector <Request> ReadRequests() {
    std::vector <Request> requests;
    int count;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        int arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        requests.push_back(Request(arrival_time, process_time));
    }
    return requests;
}

std::vector <Response> ProcessRequests(const std::vector <Request> &requests, Buffer *buffer) {
    std::vector <Response> responses;
    for (int i = 0; i < requests.size(); ++i)
        responses.push_back(buffer->Process(requests[i]));
    return responses;
}

void PrintResponses(const std::vector <Response> &responses) {
    for (int i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].start_time) << std::endl;
}

int main() {
    int size;
    std::cin >> size;
    std::vector <Request> requests = ReadRequests();

    Buffer buffer(size);
    std::vector <Response> responses = ProcessRequests(requests, &buffer);

    PrintResponses(responses);
    return 0;
}
