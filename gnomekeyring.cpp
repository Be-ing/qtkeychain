#include "gnomekeyring_p.h"

const char* GnomeKeyring::GNOME_KEYRING_DEFAULT = NULL;

bool GnomeKeyring::isSupported()
{
    const GnomeKeyring& keyring = instance();
    return keyring.isLoaded() &&
           keyring.NETWORK_PASSWORD &&
           keyring.find_password &&
           keyring.store_password &&
           keyring.delete_password;
}

GnomeKeyring::gpointer GnomeKeyring::store_network_password( const gchar* keyring, const gchar* display_name,
                                               const gchar* user, const gchar* server, const gchar* password,
                                               OperationDoneCallback callback, gpointer data, GDestroyNotify destroy_data )
{
    if ( !isSupported() )
        return 0;
    return instance().store_password( instance().NETWORK_PASSWORD,
                                      keyring, display_name, password, callback, data, destroy_data,
                                      "user", user, "server", server, static_cast<char*>(0) );
}

GnomeKeyring::gpointer GnomeKeyring::find_network_password( const gchar* user, const gchar* server,
                                              OperationGetStringCallback callback, gpointer data, GDestroyNotify destroy_data )
{
    if ( !isSupported() )
        return 0;
    return instance().find_password( instance().NETWORK_PASSWORD,
                                     callback, data, destroy_data,
                                     "user", user, "server", server, static_cast<char*>(0) );
}

GnomeKeyring::gpointer GnomeKeyring::delete_network_password( const gchar* user,
                                                       const gchar* server,
                                                       OperationDoneCallback callback,
                                                       gpointer data,
                                                       GDestroyNotify destroy_data )
{
    if ( !isSupported() )
        return 0;
    return instance().delete_password( instance().NETWORK_PASSWORD,
                                       callback, data, destroy_data,
                                       "user", user, "server", server, static_cast<char*>(0) );
}

GnomeKeyring::GnomeKeyring()
    : QLibrary("gnome-keyring", 0)
{
    static const PasswordSchema schema = {
        ITEM_NETWORK_PASSWORD,
        {{ "user",   ATTRIBUTE_TYPE_STRING },
         { "server", ATTRIBUTE_TYPE_STRING },
         { 0,     static_cast<AttributeType>( 0 ) }}
    };

    NETWORK_PASSWORD = &schema;
    find_password =	reinterpret_cast<find_password_fn*>( resolve( "gnome_keyring_find_password" ) );
    store_password = reinterpret_cast<store_password_fn*>( resolve( "gnome_keyring_store_password" ) );
    delete_password = reinterpret_cast<delete_password_fn*>( resolve( "gnome_keyring_delete_password" ) );
}

GnomeKeyring& GnomeKeyring::instance() {
    static GnomeKeyring keyring;
    return keyring;
}