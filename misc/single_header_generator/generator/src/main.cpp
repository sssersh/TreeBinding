

/*!
 * \brief Main function
 */
int main(int argc, char* argv[])
{
//    logger_t::set_out_file("single_header_genearator.log");

    if(argc != 2)
    {
        LOG("Invalid number of single header generator arguments. Expected: 2, actually: ", argc);
        return -1;
    }

    try
    {
        generator_t generator = {
                argv[1]      ,
                "creolization",
                "include",
                "serializable_types.h",
                "single_include",
                "output.template"
        };
        generator.generate();
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cout << "Single header generator error: \n" << e.what();
        return -1;
    }
}
