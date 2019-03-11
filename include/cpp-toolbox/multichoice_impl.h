
#pragma once

class invalid_choice_value : public boost::program_options::validation_error
{
public:
    invalid_choice_value(const std::string& bad_value)
        : boost::program_options::validation_error(
              boost::program_options::validation_error::invalid_option_value)
    {
        set_substitute("value", bad_value);
    }
};

template<typename T>
class multichoice_value : public boost::program_options::typed_value<T>
{
public:
    multichoice_value(const std::vector<T>& choices = {})
        : boost::program_options::typed_value<T>(0)
    {
        for (auto c : choices) {
            std::ostringstream oss;
            oss << c;
            _choices.push_back(oss.str());
        }
    }

    virtual void xparse(boost::any& value_store,
                        const std::vector<std::string>& new_tokens) const
    {

        if (std::find(_choices.begin(), _choices.end(), new_tokens[0]) ==
            _choices.end()) {
            boost::throw_exception(invalid_choice_value(new_tokens[0]));
        }
        boost::program_options::typed_value<T>::xparse(value_store, new_tokens);
    }

    std::string formatChoices() const
    {
        return "NANA";
        //        return SUT::join(_choices, "|");
    }

private:
    std::vector<std::string> _choices;
};
