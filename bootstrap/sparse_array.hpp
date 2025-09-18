/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - r-type_bs                                                          *
 * Description     -                                                                    *
 *     sparse_array                                                                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include <iostream>

template <typename Component> // You can also mirror the definition of std :: vector , that takes an additional allocator .
class sparse_array {
    public:
        using value_type = Component; // optional component type
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type>; // optionally add your allocator template here .
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

    public:export PATH="$HOME/bin:$PATH"
        sparse_array(); // You can add more constructors .
        sparse_array(sparse_array const &); // copy constructor
        sparse_array (sparse_array &&) noexcept ; // move constructor
        ~ sparse_array();
        sparse_array & operator=(sparse_array const &); // copy assignment operator
        sparse_array & operator=(sparse_array &&) noexcept; // move assignment operator
        reference_type operator [](size_t idx);
        const_reference_type operator [](size_t idx) const;
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;
        size_type size() const;
        reference_type insert_at(size_type pos, Component const &);
        reference_type insert_at(size_type pos, Component &&);
        template<class ... Params>
        reference_type emplace_at(size_type pos, Params &&...); // optional
        void erase(size_type pos);
        size_type get_index( value_type const &) const;

    private:
        container_t _data;
};





