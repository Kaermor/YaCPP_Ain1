#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server) :
    search_server_(search_server),
    empty_request_count_(0),
    current_mins_(0)
{
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query
                                                   , DocumentStatus status) {
    const auto result = search_server_.FindTopDocuments(raw_query, status);
    UpdateRequests(result.empty());
    return result;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    const auto result = search_server_.FindTopDocuments(raw_query);
    UpdateRequests(result.empty());
    return result;
}

int RequestQueue::GetNoResultRequests() const {
    return empty_request_count_;
}

void RequestQueue::UpdateRequests(bool is_empty){
    if(current_mins_ == min_in_day_){
        if(requests_.front().pustota){
            --empty_request_count_;
        }
        requests_.pop_front();
        --current_mins_;
    }
    query_result_.pustota = false;
    if(is_empty){
        query_result_.pustota = true;
        ++empty_request_count_;
    }
    requests_.push_back(query_result_);
    ++current_mins_;
}
