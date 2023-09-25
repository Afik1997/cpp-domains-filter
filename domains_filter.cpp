#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <string_view>

using namespace std;

class Domain 
{
public:
    Domain(string_view name)
        :name_(move(ReverseDomainName(name)))
    {}

    bool operator==(const Domain& other) const
    {
        return name_ == other.name_;
    }

    bool IsSubdomain(const Domain& other) const
    {
        if (other.name_.size() > name_.size())
        {
            return false;
        }
        string this_domain_name{ &name_.front(), other.name_.size() };

        return this_domain_name == other.name_;
    }

    string_view GetName() const
    {
        return name_;
    }
private:
    string name_;

    static string ReverseDomainName(string_view name)
    {
        string reverse_name{name};

        reverse(reverse_name.begin(), reverse_name.end());
        reverse_name.push_back('.');
        return reverse_name;
    }
};

class DomainChecker 
{
public:
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last)
        :banned_domains(first, last)
    {
        sort(banned_domains.begin(), banned_domains.end(), Comparator);

        banned_domains.erase(unique(banned_domains.begin(), banned_domains.end(), [](const Domain& lhs, const Domain& rhs)
            {
                return rhs.IsSubdomain(lhs);
            }), banned_domains.end());
    }

    bool IsForbidden(const Domain& other)
    {
        auto it = upper_bound(banned_domains.begin(), banned_domains.end(), other, Comparator);

        if (it == banned_domains.begin())
        {
            return false;
        }
        --it;
        return other.IsSubdomain(*it);
    }
private:
    vector<Domain> banned_domains;

    static bool Comparator(const Domain& lhs, const Domain& rhs)
    {
        return lexicographical_compare(
            lhs.GetName().begin(), lhs.GetName().end(),
            rhs.GetName().begin(), rhs.GetName().end());
    }
};

template <typename Number>
vector<Domain> ReadDomains(istream& input, Number num)
{
    vector<Domain> result;

    for (Number i = 0; i < num; ++i)
    {
        string domain_name;

        getline(input, domain_name);

        result.push_back(Domain(domain_name));
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
