 */
template<typename Type>
meta_associative_container::meta_associative_container(Type &container)
    : key_only_container{is_key_only_meta_associative_container_v<Type>},
      key_type_fn{&meta_associative_container_proxy<Type>::key_type},
      mapped_type_fn{&meta_associative_container_proxy<Type>::mapped_type},
      value_type_fn{&meta_associative_container_proxy<Type>::value_type},