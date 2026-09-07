    }

    // strip the program name
    vals.second = (esp < commandline.length() - 1) ? commandline.substr(esp + 1) : std::string{};
    ltrim(vals.second);
    return vals;
}