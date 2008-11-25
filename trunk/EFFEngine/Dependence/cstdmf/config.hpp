/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef CONFIG_HPP
#define CONFIG_HPP



/**
 * This define is used to control the conditional compilation of features 
 * that will be removed from the client builds provided to the public.
 * Examples of these would be the in-game Python consol and the Watcher Nub interface.
 *
 * If CONSUMER_CLIENT_BUILD is equal to zero then the array of development features should be compiled in.
 * If CONSUMER_CLIENT_BUILD is equal to one then development and maintenance only features should be excluded.
 */
#define CONSUMER_CLIENT_BUILD						0
#if defined( MF_SERVER ) && defined( CONSUMER_CLIENT_BUILD ) && CONSUMER_CLIENT_BUILD != 0
#warning "The CONSUMER_CLIENT_BUILD macro should not be used when building the server. Disabling and continuing build."
#undef CONSUMER_CLIENT_BUILD
#define CONSUMER_CLIENT_BUILD						0
#endif


/**
 * By setting one of the following FORCE_ENABLE_ defines to one then support for the
 * corresponding feature will be compiled in even on a consumer client build.
 */
#define FORCE_ENABLE_DEBUG_KEY_HANDLER				0
#define FORCE_ENABLE_DPRINTF						0
#define FORCE_ENABLE_PYTHON_TELNET_SERVICE			0
#define FORCE_ENABLE_WATCHERS						0
#define FORCE_ENABLE_DOG_WATCHERS					0
#define FORCE_ENABLE_ACTION_QUEUE_DEBUGGER			0
#define FORCE_ENABLE_DRAW_PORTALS					0
#define FORCE_ENABLE_DRAW_SKELETON					0
#define FORCE_ENABLE_CULLING_HUD					0


#define ENABLE_DEBUG_KEY_HANDLER		(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_DEBUG_KEY_HANDLER)
#define ENABLE_DPRINTF					(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_DPRINTF)
#define ENABLE_PYTHON_TELNET_SERVICE	(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_PYTHON_TELNET_SERVICE)
#define ENABLE_WATCHERS					(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_WATCHERS)
#define ENABLE_DOG_WATCHERS				(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_DOG_WATCHERS)
#define ENABLE_ACTION_QUEUE_DEBUGGER	(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_ACTION_QUEUE_DEBUGGER)
#define ENABLE_DRAW_PORTALS				(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_DRAW_PORTALS)
#define ENABLE_DRAW_SKELETON			(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_DRAW_SKELETON)
// Turn off the culling hud as it has alignment issues in debug mode
#define ENABLE_CULLING_HUD				0
//#define ENABLE_CULLING_HUD				(!CONSUMER_CLIENT_BUILD || FORCE_ENABLE_CULLING_HUD)


#endif // CONFIG_HPP
