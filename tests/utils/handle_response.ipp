namespace tt_tests::utils
{

template<typename Handler, typename Response>
class handle_response_t
{
public:
    handle_response_t(Handler & handler)
        : m_handler
    {}

    void operator()(Response response)
    {
        std::ostrinngstream stream;
        stream << respose;
        auto result = handler.calc_crc32(stream);

        CHECK()
    }
}

} // tt_tests::utils
