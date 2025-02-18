#include "TTask.hpp"
#include <vector>

class TTaskScheduler {
public:
    TTaskScheduler() : vertices_count_(0) {}

    template<typename Func>
    size_t add(const Func&& func) {
        tasks_.push_back(TTask(func));
        rez_tasks_.push_back(Any());
        graph_.resize(++vertices_count_);

        return vertices_count_ - 1;
    }

    template<typename Func, typename T1>
    size_t add(const Func&& func, const T1& valueOne) {
        tasks_.push_back(TTask(func, valueOne));
        rez_tasks_.push_back(Any());
        graph_.resize(++vertices_count_);

        return vertices_count_ - 1;
    }

    template<typename Func, typename T1, typename T2>
    size_t add(const Func&& func, const T1& valueOne, const T2& valueTwo) {
        tasks_.push_back(TTask(func, valueOne, valueTwo));
        rez_tasks_.push_back(Any());
        graph_.resize(++vertices_count_);

        return vertices_count_ - 1;
    }

    void executeAll() {
        TopSort();
        for (int i = 0; i < top_sort_.size(); ++i) {
            rez_tasks_[top_sort_[i]] = tasks_[top_sort_[i]]();
        }
    }

    template<typename T>
    T getResult(size_t id) {
        return Any::any_cast<T>(rez_tasks_[id]);
    }

private:
    template<typename T>
    class FutureResultOfTask {
    public:
        FutureResultOfTask(const TTask& task, size_t id, std::vector<Any>& rez_tasks) 
                        : task_(task), id_(id), link_to_rez_tasks_(rez_tasks) {}

        operator T() const {
            return Any::any_cast<T>(link_to_rez_tasks_[id_]);
        }

    private:
        TTask task_;
        size_t id_;
        std::vector<Any>& link_to_rez_tasks_;
    };

public:
    template<typename T>
    FutureResultOfTask<T> getFutureResult(size_t id) {
        graph_.resize(vertices_count_ + 1);
        graph_[vertices_count_].push_back(id);

        return FutureResultOfTask<T>(tasks_[id], id, rez_tasks_);
    }

private:
    void DFS(int v, std::vector<bool>& visited) {
        visited[v] = true;
        for (int i : graph_[v]) {
            if (!visited[i]) {
                DFS(i, visited);
            }
        }
        top_sort_.push_back(v);
    }   

    void TopSort() {
        std::vector<bool> visited(vertices_count_, false);

        for (int i = 0; i < graph_.size(); ++i) {
            if (!visited[i]) {
                DFS(i, visited);
            }
        }
    }   

private:
    size_t vertices_count_;
    
    std::vector<TTask> tasks_;
    std::vector<Any> rez_tasks_;

    std::vector<std::vector<size_t> > graph_;
    std::vector<size_t> top_sort_;
};
