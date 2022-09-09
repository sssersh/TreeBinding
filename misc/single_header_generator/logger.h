//
// Created by sersh on 09.09.2022.
//

#ifndef _LOGGER_H_
#define _LOGGER_H_

namespace one_header_gen {

class logger_t
{
public:

    logger_t() = default;

    static void set_out_file(const std::string& log_file_name)
    {
        get_instance().out_file = decltype(out_file)(log_file_name);
        get_instance().out_stream = &get_instance().out_file;
    }

    template<typename T1, typename... Ts>
    static void write(const T1& arg1, const Ts& ...args)
    {
        get_instance() << arg1;
        write(args...);
    }

    template<typename T>
    static void write(const T& arg)
    {
        get_instance() << arg;
    }

private:

    static logger_t& get_instance()
    {
        static logger_t instance;
        return instance;
    }

    template<typename T>
    std::ostream& operator<<(T&& str)
    {
        *out_stream << str;
        return *out_stream;
    }

    std::ofstream out_file;
    std::ostream* out_stream {&std::cout};
};

#define LOG_FUNCITON_NAME_WIDTH 25

#define LOG(...) logger_t::write(                                           \
    "[",                                                                    \
    std::setw(LOG_FUNCITON_NAME_WIDTH / 2 + strlen(__FUNCTION__) / 2),      \
    __FUNCTION__ ,                                                          \
    std::string((LOG_FUNCITON_NAME_WIDTH - strlen(__FUNCTION__)) / 2, ' '), \
    "] ",                                                                   \
    __VA_ARGS__,                                                            \
    "\n"                                                                    \
    )

#endif // _LOGGER_H_

} // namespace one_header_gen
