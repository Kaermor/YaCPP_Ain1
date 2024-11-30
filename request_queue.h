#pragma once

#include "search_server.h"

#include <deque>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        const auto result = search_server_.FindTopDocuments(raw_query
                                                            , document_predicate);
        UpdateRequests(result.empty());
        return result;
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        bool pustota;

    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
    int empty_request_count_;
    int current_mins_;
    QueryResult query_result_;

    void UpdateRequests(bool is_empty);
};
