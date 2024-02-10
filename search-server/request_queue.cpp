#include "request_queue.h"

using namespace std;

RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server)
{
}

vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
    auto result = search_server_.FindTopDocuments(raw_query, status);
    PushRequest(result);
    return result;
}
vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
    auto result = search_server_.FindTopDocuments(raw_query);
    PushRequest(result);
    return result;
}
int RequestQueue::GetNoResultRequests() const {
    int counter = 0;
    for (auto it = requests_.begin(); it != requests_.end(); ++it) {
        if (it->is_empty) {
            counter++;
        }
    }
    return counter;
}

void RequestQueue::PushRequest(const vector<Document>& result) {
    QueryResult query_result;
    query_result.result = result;
    if (result.empty()) {
        query_result.is_empty = true;
    }
    else {
        query_result.is_empty = false;
    }
    requests_.push_back(query_result);
    if (requests_.size() > min_in_day_) {
        requests_.pop_front();
    }
}
