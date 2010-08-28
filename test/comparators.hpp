#ifndef DTL_COMPARATORS
#define DTL_COMPARATORS

class CaseInsensitive: public dtl::Compare<char> {
public:
    virtual bool impl(const char& a, const char& b) const {
        return tolower(a) == tolower(b);
    }
};

#endif // DTL_COMPARATORS
