                              make_checked(p, s.size()));
    }
    if (prefix_size != 0) p[-1] = static_cast<Char>('-');
    using iterator = remove_reference_t<decltype(reserve(out, 0))>;
    auto data = buffer.data();
    out = write_padded<align::right>(out, specs, size, size, [=](iterator it) {
      return copy_str<Char>(data, data + size, it);
    });