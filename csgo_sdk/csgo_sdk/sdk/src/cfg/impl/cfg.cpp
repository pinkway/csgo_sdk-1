#include "../../../sdk.hpp"

namespace sdk {
	void c_cfg::save( const std::string_view name ) const {
		const auto path = std::filesystem::path{ SDK_CFG_ID_OBJECT } /= name;

		/* create a dir if it doesn't exists */
		std::filesystem::create_directory( SDK_CFG_ID_OBJECT );

		nlohmann::json json{};

		/* use a new object to verify our cfg when loading */
		auto& object = json[ SDK_CFG_ID_OBJECT ];

		for ( const auto& var : m_vars )
			var->save( object );

		auto str = json.dump( );

		/* encrypt our cfg so the user wouldn't know tf is stored in it */
		for ( auto& chr : str )
			/* there is no need for more complex algorithms, a simple xor is enough */
			chr ^= k_byte_xor;

		if ( std::ofstream file{ path, std::ios::out | std::ios::trunc } )
			file << str;
	}

	void c_cfg::load( const std::string_view name ) {
		const auto path = std::filesystem::path{ SDK_CFG_ID_OBJECT } /= name;

		/* create a dir if it doesn't exists */
		std::filesystem::create_directory( SDK_CFG_ID_OBJECT );

		std::string str{};
		if ( std::ifstream file{ path, std::ios::in } )
			file >> str;
		else
			return;

		if ( str.empty( ) )
			return;

		/* decrypt cfg */
		for ( auto& chr : str )
			chr ^= k_byte_xor;

		const auto json = nlohmann::json::parse( str );
		if ( !json.is_object( ) )
			return;

		/* verify the cfg file via trying to find our identity object */
		const auto object = json.find( SDK_CFG_ID_OBJECT );
		if ( object == json.end( ) )
			return;

		const auto& value = object.value( );

		for ( auto& var : m_vars )
			var->load( value );
	}
}