#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>

class MyException: public std::exception
{
public:
    explicit MyException(const char* message)
            : msg_(message) {}

    explicit MyException(std::string  message)
            : msg_(std::move(message)) {}

    ~MyException() noexcept override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return msg_.c_str();
    }

protected:
    std::string msg_;
};

class IDataIn{
public:
    using vec_pair_int_int = std::vector<std::pair<int,int> >;
    IDataIn() = default;
    virtual ~IDataIn() = default;
    virtual int set_n() = 0;
    virtual vec_pair_int_int set_vec_pair_int_int() = 0;
};

class DataInCin: public IDataIn{
private:
    int set_n() override {
        int n;
        while(true){
            std::cout << "Enter n(n > 2): ";
            std::cin >> n;
            if(n > 2){
                break;
            }else{
                std::cout << "Wrong n\n";
            }
        }
        return n;
    }
    vec_pair_int_int set_vec_pair_int_int() override{
        vec_pair_int_int vec_pair;
        while (true){
            int var_first, var_second;
            std::cout << "Enter you counts:\n1 -> ";
            std::cin >> var_first;
            std::cout << "2 -> ";
            std::cin >> var_second;
            vec_pair.emplace_back(var_first,var_second);
            std::cout << "Do you want to finish (y/n): ";
            char end;
            std::cin >> end;
            if(end == 'y'){
                break;
            }
        }
        return vec_pair;
    }
};

class Counts{
private:
    using vec_pair_int_int = std::vector<std::pair<int,int> >;
    vec_pair_int_int counts_;
    int n_;
public:
    Counts() = default;
    Counts(const Counts &other){
        counts_ = other.counts_;
        n_ = other.n_;
    }
    Counts &operator= (const Counts &other){
        if(&other == this){
            return *this;
        }
        Counts var(other);
        std::swap(var, *this);
        return *this;
    }
    Counts(Counts &&other) noexcept {
        counts_ = std::forward<vec_pair_int_int>(other.counts_);
        n_ = std::forward<int>(other.n_);
    }
    Counts &operator= (Counts &&other) noexcept {
        if(&other == this){
            return *this;
        }
        Counts var(other);
        std::swap(var, *this);
        return *this;
    }
    void set_counts(const vec_pair_int_int &counts){
        counts_ = counts;
    }
    void set_counts(vec_pair_int_int &&counts){
        counts_ = std::forward<vec_pair_int_int>(counts);
    }
    void set_n(int n){
        n_ = n;
    }
    [[nodiscard]] vec_pair_int_int get_counts() const{
        return counts_;
    }

    [[nodiscard]] int get_n() const{
        return n_;
    }

    friend std::ostream &operator<< (std::ostream &out, const Counts &other){
        for(const auto &i: other.counts_){
            out << i.first << "   " << i.second << std::endl;
        }
        out << std::endl;
        return out;
    }

    void work(){
        if(counts_.empty()){
            throw MyException("Vector empty");
        }
        std::sort(counts_.begin(), counts_.end(),
                  [](const auto &i, const auto &k){if(i.first < k.first) return  true; return  false;});
        int count = 1;
        vec_pair_int_int var;
        var.push_back(counts_[0]);
        int temp = counts_[0].second;
        for(vec_pair_int_int::size_type i = 1; i < counts_.size(); ++i){
            if(temp == counts_[i].second){
                ++count;
                if(count == n_){
                    var.push_back(counts_[i]);
                    count = 0;
                }
            } else{
                if(i != 1) {
                    var.push_back(counts_[i - 1]);
                }
                var.push_back(counts_[i]);
                count = 1;
            }
            temp = counts_[i].second;
        }
        var.push_back(counts_[counts_.size()-1]);
        counts_ = std::move(var);
    }
};

int main() {
    try {
        std::unique_ptr<IDataIn> data_in = std::make_unique<DataInCin>();
        auto counts_var = std::make_unique<Counts>();
        counts_var->set_counts(data_in->set_vec_pair_int_int());
        counts_var->set_n(data_in->set_n());
        std::cout << *counts_var;
        counts_var->work();
        std::cout << *counts_var;
    }catch (const MyException &e){
        std::cout << e.what() << std::endl;
    }catch (const std::exception &e){
        std::cout << e.what() << std::endl;
    }
}
