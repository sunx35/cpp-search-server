#pragma once
#include <iostream>

template <typename Iterator>
struct Page {
    Iterator begin;
    Iterator end;
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator range_begin, Iterator range_end, size_t page_size);

    auto begin() const;
    auto end() const;

private:
    std::vector<Page<Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const Page<Iterator>& page) {
    for (Iterator it = page.begin; it != page.end; ++it) {
        out << *it;
    }
    return out;
}

template <typename Iterator>
Paginator<Iterator>::Paginator(Iterator range_begin, Iterator range_end, size_t page_size) {
    // заполнить Pages
    Iterator page_begin = range_begin;
    Iterator page_end = next(page_begin, page_size);
    while (distance(page_begin, range_end) >= distance(page_begin, page_end)) {
        pages_.push_back({ page_begin, page_end });
        page_begin = page_end;
        page_end = next(page_begin, page_size);
    }
    // если еще осталось, еще добавляем последнюю страницу
    if (page_begin != range_end) {
        pages_.push_back({ page_begin, range_end });
    }
}

template <typename Iterator>
auto Paginator<Iterator>::begin() const {
    return pages_.begin();
}

template <typename Iterator>
auto Paginator<Iterator>::end() const {
    return pages_.end();
}