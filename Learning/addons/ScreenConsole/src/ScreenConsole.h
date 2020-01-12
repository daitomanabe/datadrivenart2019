#include "ofMain.h"

struct ScreenConsole {
    struct Line {
        Line(ScreenConsole *console)
        : os{""}
        , console{console} {};
        Line(Line &&) = default;
        ~Line()
        { console->push(os.str()); }
        
        Line &operator=(Line &&) = default;

        template <typename type>
        Line &operator<<(const type &v) {
            os << v;
            return *this;
        }
        std::ostringstream os;
        ScreenConsole *console;
    };
    
    ScreenConsole(std::size_t size = 30)
    : num_lines(size)
    {};
    
    void push(const std::string &str) {
        lines.push_back(str);
        treat();
    }
    void push(std::string &&str) {
        lines.push_back(std::move(str));
        treat();
    }
    void treat() {
        while(num_lines < lines.size()) lines.pop_front();
    }
    
    void clear() {
        lines.clear();
    }
    
    Line operator()(const std::string &tag) {
        Line line{this};
        if(tag != "") line << tag << ": ";
        return line;
    }
    
    std::string get() const {
        std::ostringstream os{""};
        std::for_each(lines.crbegin(),
                      lines.crend(),
                      [&os](const std::string &value) {
            os << "\n" << value;
        });
        return os.str();
    }
    
    void print(float x, float y) {
        ofDrawBitmapString(get(), x, y);
    }
    
    std::deque<std::string> lines;
    std::size_t num_lines{};
};
