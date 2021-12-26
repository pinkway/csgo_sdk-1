#pragma once

namespace sdk::detail {
	/* wrapper for std::array which contains math operators to create vectors/angles etc */
	template < typename _value_t, std::size_t _size, typename _derived_t >
		requires std::is_arithmetic_v< _value_t > && ( _size >= 2u )
	struct array_wrapper_t {
	protected:
		struct is_base_of_t {
			static constexpr std::false_type test( ... );

			/* allow operations between 2 same _value_t containers */
			template < std::size_t _other_size, typename _other_derived_t >
			static constexpr std::true_type test( array_wrapper_t< _value_t, _other_size, _other_derived_t > );
		};

		template < typename _other_t >
		using is_base_of = decltype( is_base_of_t::test( std::declval< _other_t >( ) ) );

		std::array< _value_t, _size > m_elements{};
	public:
		ALWAYS_INLINE constexpr array_wrapper_t( ) = default;

		template < typename... _args_t >
			requires ( sizeof...( _args_t ) <= _size )
		ALWAYS_INLINE constexpr array_wrapper_t( const _args_t&... args ) : m_elements{ args... } {}

		ALWAYS_INLINE constexpr _value_t& at( const std::size_t i ) { return m_elements.at( i ); }

		ALWAYS_INLINE constexpr _value_t at( const std::size_t i ) const { return m_elements.at( i ); }

		ALWAYS_INLINE constexpr auto begin( ) { return m_elements.begin( ); }

		ALWAYS_INLINE constexpr auto begin( ) const { return m_elements.begin( ); }

		ALWAYS_INLINE constexpr auto end( ) { return m_elements.end( ); }

		ALWAYS_INLINE constexpr auto end( ) const { return m_elements.end( ); }

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t >
		ALWAYS_INLINE _derived_t& operator -=( const _rhs_t rhs ) {
			for ( auto& element : m_elements )
				element -= rhs;

			return static_cast< _derived_t& >( *this );
		}

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t >
		ALWAYS_INLINE _derived_t& operator +=( const _rhs_t rhs ) {
			for ( auto& element : m_elements )
				element += rhs;

			return static_cast< _derived_t& >( *this );
		}

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t >
		ALWAYS_INLINE _derived_t& operator *=( const _rhs_t rhs ) {
			for ( auto& element : m_elements )
				element *= rhs;

			return static_cast< _derived_t& >( *this );
		}

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t >
		ALWAYS_INLINE _derived_t& operator /=( const _rhs_t rhs ) {
			for ( auto& element : m_elements )
				element /= rhs;

			return static_cast< _derived_t& >( *this );
		}

		template < std::size_t _rhs_size, typename _rhs_derived_t >
		ALWAYS_INLINE _derived_t& operator -=(
			const array_wrapper_t< _value_t, _rhs_size, _rhs_derived_t >& rhs
		) {
			constexpr auto k_min_size = std::min( _size, _rhs_size );
			for ( std::size_t i{}; i < k_min_size; ++i )
				at( i ) -= rhs.at( i );

			return static_cast< _derived_t& >( *this );
		}

		template < std::size_t _rhs_size, typename _rhs_derived_t >
		ALWAYS_INLINE _derived_t& operator +=(
			const array_wrapper_t< _value_t, _rhs_size, _rhs_derived_t >& rhs
		) {
			constexpr auto k_min_size = std::min( _size, _rhs_size );
			for ( std::size_t i{}; i < k_min_size; ++i )
				at( i ) += rhs.at( i );

			return static_cast< _derived_t& >( *this );
		}

		template < std::size_t _rhs_size, typename _rhs_derived_t >
		ALWAYS_INLINE _derived_t& operator *=(
			const array_wrapper_t< _value_t, _rhs_size, _rhs_derived_t >& rhs
		) {
			constexpr auto k_min_size = std::min( _size, _rhs_size );
			for ( std::size_t i{}; i < k_min_size; ++i )
				at( i ) *= rhs.at( i );

			return static_cast< _derived_t& >( *this );
		}

		template < std::size_t _rhs_size, typename _rhs_derived_t >
		ALWAYS_INLINE _derived_t& operator /=(
			const array_wrapper_t< _value_t, _rhs_size, _rhs_derived_t >& rhs
		) {
			constexpr auto k_min_size = std::min( _size, _rhs_size );
			for ( std::size_t i{}; i < k_min_size; ++i )
				at( i ) /= rhs.at( i );

			return static_cast< _derived_t& >( *this );
		}

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t > || is_base_of< _rhs_t >::value
		ALWAYS_INLINE _derived_t operator -( const _rhs_t& rhs ) const {
			auto ret = *this;

			return ret -= rhs;
		}

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t > || is_base_of< _rhs_t >::value
		ALWAYS_INLINE _derived_t operator +( const _rhs_t& rhs ) const {
			auto ret = *this;

			return ret += rhs;
		}

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t > || is_base_of< _rhs_t >::value
		ALWAYS_INLINE _derived_t operator *( const _rhs_t& rhs ) const {
			auto ret = *this;

			return ret *= rhs;
		}

		template < typename _rhs_t >
			requires std::is_arithmetic_v< _rhs_t > || is_base_of< _rhs_t >::value
		ALWAYS_INLINE _derived_t operator /( const _rhs_t& rhs ) const {
			auto ret = *this;

			return ret /= rhs;
		}

		ALWAYS_INLINE constexpr bool operator ==(
			const array_wrapper_t< _value_t, _size, _derived_t >& rhs
		) const {
			for ( std::size_t i{}; i < _size; ++i )
				if ( at( i ) != rhs.at( i ) )
					return false;

			return true;
		}

		ALWAYS_INLINE constexpr std::compare_three_way_result_t< _value_t, _value_t > operator <=>(
			const array_wrapper_t< _value_t, _size, _derived_t >& rhs
		) const {
			return std::lexicographical_compare_three_way(
				m_elements.begin( ), m_elements.end( ),
				rhs.m_elements.begin( ), rhs.m_elements.end( ),
				std::_Synth_three_way{}
			);
		}

		ALWAYS_INLINE constexpr std::size_t size( ) const { return _size; }
	};
}