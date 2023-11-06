#include <iostream>
#include <cstdint>
#include <cmath>
#include <utility>
#include <list>
#include <vector>

class RootDecomposition {
public:
    enum Operation { SUM, XOR };

private:
    Operation operation_;
    int mod_;
    std::list<int> data_;
    std::vector<std::pair<int64_t, std::list<int>::iterator> > parts_;

    void fullBuild();

public:
    RootDecomposition(Operation operation, int mod, std::istream& instream);

    void insert(unsigned int index, int value);
    void erase(unsigned int index);
    void change(unsigned int index, int value);

    int at(unsigned int index) const;
    int compute(unsigned int left, unsigned int right) const;
    inline unsigned int size() const;
};

RootDecomposition::RootDecomposition(Operation operation, int mod, std::istream& instream)
    : operation_(operation), mod_(mod) {
    unsigned int size;
    std::cin >> size;
    int number;
    for (unsigned int i = 0; i < size; i++) {
        instream >> number;
        this->data_.push_back(number);
    }
    this->fullBuild();
}
void RootDecomposition::fullBuild() {
    unsigned int data_size = this->data_.size(), parts_size = sqrt(data_size) + 1;
    this->parts_.resize(parts_size);
    this->parts_[parts_size - 1] = {0, this->data_.end()};
    std::list<int>::iterator current = this->data_.begin();
    for (unsigned int i = 0, part = -1; i < data_size; i++, current++) {
        if (i % parts_size == 0) {
            this->parts_[++part] = {*current, current};
        } else {
            std::pair<int64_t, std::list<int>::iterator>& part_obj = this->parts_[part];
            if (this->operation_ == Operation::SUM) {
                part_obj.first += *current;
            } else {
                part_obj.first ^= *current;
            }
        }
    }
}

void RootDecomposition::insert(unsigned int index, int value) {
    unsigned int parts_size = this->parts_.size();
    unsigned int part = index / parts_size;
    index %= parts_size;
    std::list<int>::iterator inserted = this->parts_[part].second;
    bool on_split_point = (index == 0);
    while ((index--) != 0) {
        inserted++;
    }
    inserted = this->data_.insert(inserted, value);

    unsigned int data_size = this->data_.size();
    unsigned int new_part_size = sqrt(data_size) + 1;
    if (new_part_size != parts_size) {
        this->fullBuild();
        return;
    }
    for (; part < parts_size; part++) {
        std::pair<int64_t, std::list<int>::iterator>& part_obj = this->parts_[part];
        if (on_split_point) {
            part_obj.second = inserted;
        }
        if (this->operation_ == Operation::SUM) {
            part_obj.first += *inserted;
        } else {
            part_obj.first ^= *inserted;
        }

        if (part + 1 >= parts_size ||
            (this->parts_[part + 1].second == this->data_.end() && data_size % parts_size != 1)) {
            break;
        }

        inserted = this->parts_[part + 1].second;
        inserted--;
        if (this->operation_ == Operation::SUM) {
            part_obj.first -= *inserted;
        } else {
            part_obj.first ^= *inserted;
        }
        on_split_point = true;
    }
}
void RootDecomposition::erase(unsigned int index) {
    unsigned int parts_size = this->parts_.size();
    unsigned int part = index / parts_size;
    index %= parts_size;
    std::list<int>::iterator removed = this->parts_[part].second;
    while ((index--) != 0) {
        removed++;
    }

    unsigned int new_part_size = sqrt(this->data_.size() - 1) + 1;
    if (new_part_size != this->parts_.size()) {
        this->data_.erase(removed);
        this->fullBuild();
        return;
    }

    std::list<int>::iterator to_erase = removed;
    for (; part < this->parts_.size(); part++) {
        std::pair<int64_t, std::list<int>::iterator>& part_obj = this->parts_[part];
        if (removed == part_obj.second) {
            part_obj.second++;
        }
        if (this->operation_ == Operation::SUM) {
            part_obj.first -= *removed;
        } else {
            part_obj.first ^= *removed;
        }

        if (part + 1 >= this->parts_.size() || this->parts_[part + 1].second == this->data_.end()) {
            break;
        }
        removed = this->parts_[part + 1].second;
        if (this->operation_ == Operation::SUM) {
            part_obj.first += *removed;
        } else {
            part_obj.first ^= *removed;
        }
    }
    this->data_.erase(to_erase);
}
void RootDecomposition::change(unsigned int index, int value) {
    unsigned int parts_size = this->parts_.size();
    unsigned int part = index / parts_size;
    index %= parts_size;
    std::list<int>::iterator changed = this->parts_[part].second;
    while ((index--) != 0) {
        changed++;
    }

    std::pair<int64_t, std::list<int>::iterator>& part_obj = this->parts_[part];
    if (this->operation_ == Operation::SUM) {
        part_obj.first += value - *changed;
    } else {
        part_obj.first ^= *changed ^ value;
    }
    *changed = value;
}

int RootDecomposition::at(unsigned int index) const {
    unsigned int parts_size = this->parts_.size();
    unsigned int part = index / parts_size;
    index %= parts_size;
    std::list<int>::iterator current = this->parts_[part].second;
    while ((index--) != 0) {
        current++;
    }
    return (*current);
}
int RootDecomposition::compute(unsigned int left, unsigned int right) const {
    unsigned int parts_size = this->parts_.size();
    unsigned int left_part = left / parts_size;
    left %= parts_size;
    unsigned int left_index = left;
    std::list<int>::iterator left_it = this->parts_[left_part].second;
    while ((left--) != 0) {
        left_it++;
    }
    unsigned int right_part = right / parts_size;
    right %= parts_size;
    unsigned int right_index = right;
    std::list<int>::iterator right_it = this->parts_[right_part].second;
    while ((right--) != 0) {
        right_it++;
    }
    right_it++;

    if (left_part == right_part) {
        if (right_index - left_index <= left_index + parts_size - right_index - 1) {
            int64_t answer = 0;
            while (left_it != right_it) {
                if (this->operation_ == Operation::SUM) {
                    answer += *(left_it++);
                } else {
                    answer ^= *(left_it++);
                }
            }
            if (this->operation_ == Operation::SUM) {
                answer %= this->mod_;
                if (answer < 0) {
                    answer += this->mod_;
                }
            }
            return answer;
        }
        int64_t answer = this->parts_[left_part].first;
        while ((left_index--) != 0) {
            if (this->operation_ == Operation::SUM) {
                answer -= *(--left_it);
            } else {
                answer ^= *(--left_it);
            }
        }
        while ((++right_index) != parts_size) {
            if (this->operation_ == Operation::SUM) {
                answer -= *(right_it++);
            } else {
                answer ^= *(right_it++);
            }
        }
        if (this->operation_ == Operation::SUM) {
            answer %= this->mod_;
            if (answer < 0) {
                answer += this->mod_;
            }
        }
        return answer;
    }

    int64_t answer = 0;
    if (left_index < parts_size - left_index) {
        while ((left_index--) != 0) {
            if (this->operation_ == Operation::SUM) {
                answer -= *(--left_it);
            } else {
                answer ^= *(--left_it);
            }
        }
    } else {
        while (left_it != this->parts_[left_part + 1].second) {
            if (this->operation_ == Operation::SUM) {
                answer += *(left_it++);
            } else {
                answer ^= *(left_it++);
            }
        }
        left_part++;
    }

    while (left_part != right_part) {
        if (this->operation_ == Operation::SUM) {
            answer += this->parts_[left_part++].first;
        } else {
            answer ^= this->parts_[left_part++].first;
        }
    }

    if (right_index + 1 < parts_size - right_index - 1) {
        while (right_it != this->parts_[right_part].second) {
            if (this->operation_ == Operation::SUM) {
                answer += *(--right_it);
            } else {
                answer ^= *(--right_it);
            }
        }
    } else {
        if (this->operation_ == Operation::SUM) {
            answer += this->parts_[right_part].first;
        } else {
            answer ^= this->parts_[right_part].first;
        }
        while ((++right_index) != parts_size && right_it != this->data_.end()) {
            if (this->operation_ == Operation::SUM) {
                answer -= *(right_it++);
            } else {
                answer ^= *(right_it++);
            }
        }
    }
    if (this->operation_ == Operation::SUM) {
        answer %= this->mod_;
        if (answer < 0) {
            answer += this->mod_;
        }
    }
    return answer;
}
inline unsigned int RootDecomposition::size() const {
    return this->data_.size();
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    char op;
    std::cin >> op;
    int mod = 1;
    if (op == '+') {
        std::cin >> mod;
    }
    RootDecomposition solver(
        op == '+' ? RootDecomposition::Operation::SUM : RootDecomposition::Operation::XOR, mod,
        std::cin);

    unsigned int queries;
    std::cin >> queries;
    std::string command;
    unsigned int i, l, r;
    int x;
    for (unsigned int query = 0; query < queries; query++) {
        std::cin >> command;
        if (command == "insert") {
            std::cin >> i >> x;
            solver.insert(i - 1, x);
        }
        if (command == "erase") {
            std::cin >> i;
            solver.erase(i - 1);
        }
        if (command == "change") {
            std::cin >> i >> x;
            solver.change(i - 1, x);
        }
        if (command == "at") {
            std::cin >> i;
            std::cout << solver.at(i - 1) << "\n";
        }
        if (command == "compute") {
            std::cin >> l >> r;
            std::cout << solver.compute(l - 1, r - 1) << "\n";
        }
        if (command == "size") {
            std::cout << solver.size() << "\n";
        }
    }

    return 0;
}